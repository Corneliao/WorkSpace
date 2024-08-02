#include "../include/tcpsocket.h"
tcpSocket::tcpSocket(const qintptr handle) {
    this->socket = new QTcpSocket(this);
    this->socketDescriptor = handle;
    this->socket->setSocketDescriptor(this->socketDescriptor);

    // connectios
    connect(this->socket, &QTcpSocket::readyRead, this, &tcpSocket::thread_receiveData, Qt::DirectConnection);
    connect(this->socket, &QTcpSocket::disconnected, this, &tcpSocket::thread_userdisconnected, Qt::DirectConnection);
}

void tcpSocket::thread_receiveData() {
    QDataStream stream(this->socket);
    int type;
    stream >> type;
    if (type == userInfo) {
        QString username;
        QString profile_picture_path;
        stream >> username >> profile_picture_path;
        this->t_userName = username;
        emit this->thread_receiveUserInfo(username, profile_picture_path);
    } else if (type == userMessage) {
        QString username;
        QString profile_picture_path;
        QString receiveUser;
        QString message;
        stream >> username >> profile_picture_path >> receiveUser >> message;
        emit this->thread_receiveMessage(username, profile_picture_path, receiveUser, message);
    } else if (type == fileinfo) {
        QString username;
        QString bucketName;
        stream >> username >> bucketName;
        emit this->thread_receiveBucketFileUrl(username, bucketName);
    }
}

void tcpSocket::thread_sendUserInfo(const QList<QString> usernames_list, const QList<QString> user_profile_picture_path_list) {

    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    int type = userInfo;
    stream << (int)type << usernames_list << user_profile_picture_path_list;
    this->socket->write(out);
}

void tcpSocket::thread_sendUserMessage(const QString username, const QString profile_picture_path, const QString receiveUser, const QString message) {
    if (this->t_userName == receiveUser) {
        QByteArray out;
        QDataStream stream(&out, QIODevice::WriteOnly);
        int type = userMessage;
        stream << (int)type << (QString)username << (QString)profile_picture_path << (QString)receiveUser << (QString)message;
        this->socket->write(out);
    }
}
void tcpSocket::thread_sendBucketFileUrl(const QString username, const QString bucketFileUrl) {
    if (username == this->t_userName) {
        return;
    }
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    int type = fileinfo;
    stream << (int)type << (QString)bucketFileUrl;
    this->socket->write(out);
}

void tcpSocket::thread_userdisconnected() {
    emit this->thread_acceptUserdisconnected(this->t_userName);
}

void tcpSocket::thread_removeUser(const QString username) {
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    int type = userLogou;
    stream << (int)type << (QString)username;
    this->socket->write(out);
}

const QString tcpSocket::username() {
    return this->t_userName;
}
