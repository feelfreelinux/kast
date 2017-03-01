#include <QCoreApplication>
#include "server.h"
#include "dlnarenderer.h"

using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (a.arguments().count() <= 1) {
        qDebug() << "Usage: kast <path of file to stream>";
        return 1;
    }

    Server server(a);
    return a.exec();
}
