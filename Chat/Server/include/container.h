#ifndef CONTAINER_H
#define CONTAINER_H

#include "tcpserver.h"
#include "tcpsocket.h"
#include <QObject>
#include <QThread>
class Container : public QObject {
    Q_OBJECT

    public:
    explicit Container(QObject *parent = nullptr);
    void dealNewUserConnection(const qintptr handle);
    void dealUserInfo(const QString username, const QString profile_picture_path);
    void dealUserMessage(const QString username, const QString profile_picture_path, const QString receiveUser, const QString message);
    void dealUserdisconected(const QString username);
    void dealBucketeFileUrl(const QString username, const QString bucketFileUrl);
    signals:
    void transferUserInfo(const QList<QString> usernames_list, const QList<QString> profile_picture_paths_list);
    void transferUserMessage(const QString username, const QString profile_picture_path, const QString receiveUser, const QString message);
    void removeUser(const QString username);
    void transferBucketFileUrl(const QString username, const QString bucketFileUrl);

    private:
    tcpServer *server;
    QMap<QThread *, tcpSocket *> threads;
    QList<QString> usernames_list;
    QList<QString> user_profile_picture_path_list;
};

#endif // CONTAINER_H
