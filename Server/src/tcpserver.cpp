#include "../include/tcpserver.h"

tcpServer::tcpServer(QObject *parent) : QTcpServer(parent) {
    if (this->listen(QHostAddress::Any, quint16(8888))) {
        qDebug() << "the server is  lisntening： " << " Server Address" << this->serverAddress() << "Server Port"
                 << this->serverPort() << "\n";
    } else {
        qDebug() << "the server  lisnten failed：" << this->serverError();
    }
}

void tcpServer::incomingConnection(const qintptr handle) {
    emit this->newUserConnection(handle);
}
