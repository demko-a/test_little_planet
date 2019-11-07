#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

/**
  * @date ноябрь 2019
  * @author Демко А.Е.
  * @brief Класс для конвертации изображений.
  */

#include <QOpenGLWidget>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QOpenGLTexture;

/// @brief Класс для конвертации изображений.
///
/// В конструктор передается имя исходного файла.
/// Картинка загружается в текстуру, обрабатывается шейдерной программой,
/// после отрисовки сохраняется новое изображение, и программа завершает работу.
class ImageConverter : public QOpenGLWidget
{
public:
    ImageConverter(const QString& filename, QWidget* parent = 0);
    ~ImageConverter();
private:
    void saveResultImage();
    void initShaders();
    void loadTexture();

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    QString makeResultFilename(const QString& sourceFilename) const;
    QOpenGLFunctions* glFuncs();
private:
    QOpenGLShaderProgram* shaderProgram_;
    QOpenGLTexture* texture_;
    QString sourceFilename_;

    QSize sourceSize_;

    int vertexAttr_;
    int textureAttr_;
    int matrixUniform_;
    int textureUniform_;

};

#endif // IMAGECONVERTER_H
