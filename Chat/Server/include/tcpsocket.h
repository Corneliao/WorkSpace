#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
class tcpSocket : public QObject {
    Q_OBJECT
    enum msgtype { userInfo, userMessage, userLogou, fileinfo, fileData };

    public:
    explicit tcpSocket(const qintptr handle);
    void thread_receiveData();
    void thread_sendUserInfo(const QList<QString> usernames_list, const QList<QString> user_profile_picture_path_list);
    void thread_sendUserMessage(const QString username, const QString profile_picture_path, const QString receiveUser, const QString message);
    void thread_sendBucketFileUrl(const QString username, const QString bucketFileUrl);
    void thread_userdisconnected();
    void thread_removeUser(const QString username);
    const QString username();
    signals:
    void thread_receiveUserInfo(const QString &username, const QString &profile_picture_path);
    void thread_receiveMessage(const QString username, const QString profile_picture_path, const QString receiveUser, const QString message);
    void thread_acceptUserdisconnected(const QString username);
    void thread_receiveBucketFileUrl(const QString, const QString bucketFileUrl);

    private:
    qintptr socketDescriptor;
    QTcpSocket *socket;
    QString t_userName;
};

#endif // TCPSOCKET_H
