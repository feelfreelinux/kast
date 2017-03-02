#include <QCoreApplication>
#include "server.h"
#include "dlnarenderer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    if(app.arguments().count() <= 1)
        qFatal("Usage: kast <list of files to stream>");

    QStringList files = app.arguments();
    files.removeFirst();

    Server server(files);
    return app.exec();
}
