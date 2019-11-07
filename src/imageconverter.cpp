#include "imageconverter.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QFileInfo>

ImageConverter::ImageConverter(const QString &filename, QWidget *parent) :
    QOpenGLWidget(parent)
  , shaderProgram_(new QOpenGLShaderProgram(this))
  , sourceFilename_(filename)
{}

ImageConverter::~ImageConverter()
{
    makeCurrent();
    delete texture_;
    doneCurrent();
}

void ImageConverter::saveResultImage()
{
    QImage resultImage = grabFramebuffer();
    QString resulFilename = makeResultFilename(sourceFilename_);
    if (resultImage.save(resulFilename))
        qDebug() << "Result image save to file " << resulFilename;
    else
        qDebug() << "Result image not save.";
}

void ImageConverter::initShaders()
{
    QOpenGLShader vShader(QOpenGLShader::Vertex);
    vShader.compileSourceFile( ":/shaders/shader.vert");

    QOpenGLShader fShader( QOpenGLShader::Fragment );
    fShader.compileSourceFile( ":/shaders/shader.frag" );

    shaderProgram_->addShader(&vShader);
    shaderProgram_->addShader(&fShader);

    if (!shaderProgram_->link())
    {
        qDebug() << " Error: Shader Program is not link.";
        return;
    }

    vertexAttr_ = shaderProgram_->attributeLocation( "vertexAttr" );
    textureAttr_ = shaderProgram_->attributeLocation( "textureAttr" );
    matrixUniform_ = shaderProgram_->uniformLocation( "matrixUniform" );
    textureUniform_ = shaderProgram_->uniformLocation( "textureUniform" );
}

void ImageConverter::loadTexture()
{
    QImage image(sourceFilename_);
    if (image.isNull())
    {
        qDebug() << "Error: Image not load";
        exit(-1);
    }

    sourceSize_ = image.size();
    texture_ = new QOpenGLTexture(image);
}

void ImageConverter::initializeGL()
{
    glFuncs()->glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    initShaders();
    loadTexture();
    resize(sourceSize_);
}

void ImageConverter::resizeGL(int w, int h)
{
    glFuncs()->glViewport( 0, 0, w, h);
}

void ImageConverter::paintGL()
{
    glFuncs()->glClear( GL_COLOR_BUFFER_BIT );

    if ( !shaderProgram_->bind() )
    {
        qDebug() << "Error: Shader Program is not bind";
        return;
    }

    float h = sourceSize_.height();
    float w = sourceSize_.width();
    GLfloat const triangleVertices[] = {
        0.f, 0.f, 0.f,
        0.f,  h, 0.0f,
        w, h, 0.f,
        w,  0.f,  0.f
    };

    GLfloat const textureCoord[] = {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f
    };

    QMatrix4x4 matrix;
    matrix.ortho(rect());

    shaderProgram_->setUniformValue(matrixUniform_, matrix);

    texture_->bind();

    shaderProgram_->setAttributeArray(vertexAttr_, triangleVertices, 3);
    shaderProgram_->setAttributeArray( textureAttr_, textureCoord, 2 );
    shaderProgram_->setUniformValue( textureUniform_, 0 );

    shaderProgram_->enableAttributeArray(vertexAttr_);
    shaderProgram_->enableAttributeArray(textureAttr_);

    glFuncs()->glDrawArrays(GL_QUADS, 0,4);

    shaderProgram_->disableAttributeArray(vertexAttr_);
    shaderProgram_->disableAttributeArray(textureAttr_);

    shaderProgram_->release();

    saveResultImage();
    exit(1);
}

QString ImageConverter::makeResultFilename(const QString &sourceFilename) const
{
    QFileInfo fi(sourceFilename);
    return QString("%1/%2%3.%4").arg(fi.path()).arg(fi.baseName()).arg("-littleplanet").arg(fi.completeSuffix());
}

QOpenGLFunctions* ImageConverter::glFuncs()
{
    return QOpenGLContext::currentContext()->functions();
}
