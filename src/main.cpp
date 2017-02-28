#include <QCoreApplication>
#include "server.h"
#include "dlnarenderer.h"

using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (a.arguments().count() <= 1) {
        qDebug() << "Usage: DLNA [file-1] ([file-2] [file-3] ...)";
        return 1;
    }

    Server server(a);
    return a.exec();
}
