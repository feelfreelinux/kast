#include <QCoreApplication>
#include "Kast.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    if(app.arguments().count() <= 1)
        qFatal("Usage: kast <list of files to stream>");

    QStringList files = app.arguments();
    files.removeFirst();

    Kast kast(files);
    return app.exec();
}
