#ifndef SOAPACTION_H
#define SOAPACTION_H
#include <QObject>
#include <QtNetwork>

class SOAPActionManager : public QObject
{
    Q_OBJECT
public:
    explicit SOAPActionManager(QObject *parent = 0);
    void doAction(QString action, QString requestData, QUrl controlUrl);
private:
    QNetworkAccessManager *mgr;
private slots:
    void processData(QNetworkReply* reply);
};

#endif // SOAPACTION_H
