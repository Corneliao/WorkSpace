#include "../include/container.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include <qabstractsocket.h>
class App final {
    public:
    explicit App();
    ~App();

    private:
    QThread *thread;
    Container *container;
};
App::App() {
    this->thread = new QThread;
    this->container = new Container;
    this->container->moveToThread(thread);
    this->thread->start();
}

App::~App() {
    if (this->container == nullptr || this->thread == nullptr) {
        return;
    }
    this->thread->exit(0);
    this->thread->wait();
    this->thread->deleteLater();
    this->container->deleteLater();
    this->thread = nullptr;
    this->container = nullptr;
}

class quitApp : public QThread {
    // Q_OBJECT
    public:
    void run() override;
};

void quitApp::run() {
    qDebug() << "Please  Press  q  quit the application";

    while (true) {
        if (getchar() == 'q') {
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Set up code that uses the Qt event loop here.
    // Call a.quit() or a.exit() to quit the application.
    // A not very useful example would be including

    // near the top of the file and calling
    //  QTimer::singleShot( 2000, &a, &QCoreApplication::quit );
    // which quits the application after 5 seconds.

    // If you do not need a running Qt event loop, remove the call
    // to a.exec() or use the Non-Qt Plain C++ Application template.
    // App app;
    qDebug() << "--------------The Server is Start--------------\n";
    App app;
    quitApp *thread = new quitApp;
    QObject::connect(thread, &QThread::finished, &a, &QCoreApplication::quit, Qt::QueuedConnection);
    thread->start();
    return a.exec();
}
