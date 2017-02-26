#include <QCoreApplication>
#include "server.h"
#include "dlnarenderer.h"

using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server xd;
    return a.exec();
}
