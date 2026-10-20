#ifndef LOCALWEBSERVER_H
#define LOCALWEBSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class LocalWebServer : public QObject {
    Q_OBJECT

public:
    explicit LocalWebServer(quint16 port = 8080, QObject* parent = nullptr);
    bool start();
    void stop();
    quint16 port() const { return m_port; }

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    void handleRequest(QTcpSocket* socket, const QString& rawRequest);
    QString generateHtmlPortal();
    QString generateBuildingsJson();
    QString generateChatJson(const QString& query);

    QTcpServer* m_server;
    quint16 m_port;
};

#endif // LOCALWEBSERVER_H
