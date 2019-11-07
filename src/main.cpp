#include <QApplication>
#include <QDebug>

#include "imageconverter.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    if (argc < 2)
    {
        qDebug() << "Error: recource is not specified";
        return  0;
    }
    ImageConverter converter(argv[1]);
    converter.show();
    return app.exec();
}
