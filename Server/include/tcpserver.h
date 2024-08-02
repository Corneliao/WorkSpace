#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class tcpServer : public QTcpServer
{
    Q_OBJECT
public:
    tcpServer( QObject* parent = nullptr );

protected:
    void incomingConnection( const qintptr handle ) override;
signals:
    void newUserConnection( const qintptr handle );
};

#endif  // TCPSERVER_H
