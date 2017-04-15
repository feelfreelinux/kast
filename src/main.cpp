#include "Kast.h"

#include <QCoreApplication>


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    if(app.arguments().count() <= 1)
        qFatal("Usage: kast <list of files to stream>");

    QStringList files = app.arguments();
    files.removeFirst();

    Kast kast;
    for(auto file : files)
        kast.addItemToQueue(file);

    return app.exec();
}
