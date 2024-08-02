#include "../include/container.h"

Container::Container(QObject *parent) : QObject{ parent } {
    this->server = new tcpServer(this);
    connect(this->server, &tcpServer::newUserConnection, this, &Container::dealNewUserConnection, Qt::DirectConnection);
}

void Container::dealNewUserConnection(const qintptr handle) {
    QThread *thread = new QThread;
    tcpSocket *work = new tcpSocket(handle);
    work->moveToThread(thread);
    thread->start();
    this->threads.insert(thread, work);

    // deal userinfo
    connect(work, &tcpSocket::thread_receiveUserInfo, this, &Container::dealUserInfo, Qt::QueuedConnection);
    connect(this, &Container::transferUserInfo, work, &tcpSocket::thread_sendUserInfo, Qt::QueuedConnection);
    // deal usermessage
    connect(work, &tcpSocket::thread_receiveMessage, this, &Container::dealUserMessage, Qt::QueuedConnection);
    connect(this, &Container::transferUserMessage, work, &tcpSocket::thread_sendUserMessage, Qt::QueuedConnection);
    // user disconnected
    connect(work, &tcpSocket::thread_acceptUserdisconnected, this, &Container::dealUserdisconected, Qt::QueuedConnection);
    // remove user
    connect(this, &Container::removeUser, work, &tcpSocket::thread_removeUser, Qt::QueuedConnection);
    // deal bucketFileUrl
    connect(work, &tcpSocket::thread_receiveBucketFileUrl, this, &Container::dealBucketeFileUrl, Qt::QueuedConnection);
    connect(this, &Container::transferBucketFileUrl, work, &tcpSocket::thread_sendBucketFileUrl, Qt::QueuedConnection);
}

void Container::dealUserInfo(const QString username, const QString profile_picture_path) {
    this->usernames_list.append(username);
    this->user_profile_picture_path_list.append(profile_picture_path);
    emit this->transferUserInfo(this->usernames_list, this->user_profile_picture_path_list);
}

void Container::dealUserMessage(const QString username, const QString profile_picture_path, const QString receiveUser, const QString message) {
    emit this->transferUserMessage(username, profile_picture_path, receiveUser, message);
}

void Container::dealUserdisconected(const QString username) {
    foreach (auto userthread, this->threads.keys()) {
        if (username == this->threads.value(userthread)->username()) {
            userthread->exit(0);
            userthread->wait();
            userthread->deleteLater();
            this->threads.value(userthread)->deleteLater();
            this->threads.remove(userthread);
        }
    }
    for (int i = 0; i < this->usernames_list.length(); i++) {
        if (username == this->usernames_list.at(i)) {
            this->usernames_list.remove(i);
            this->user_profile_picture_path_list.remove(i);
        }
    }
    emit this->removeUser(username);
}

void Container::dealBucketeFileUrl(const QString username, const QString bucketFileUrl) {
    qDebug() << username << bucketFileUrl;
    emit this->transferBucketFileUrl(username, bucketFileUrl);
}
