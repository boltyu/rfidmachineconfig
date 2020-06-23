#ifndef TRANSFERFILE_H
#define TRANSFERFILE_H


#include <QThread>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>


class TransferFile : public QTcpSocket{
    Q_OBJECT
public:

    enum TransferMethod{
        Method_getconfig = 0,
        Method_putconfig,
        Method_getimg,
        Method_putimg
    };
    enum TransferResult{
        Result_finish = 0,
        Result_cantopen,
        Result_netcrash,
        Result_filecrash,
        Result_wait
    };

    TransferFile(QHostAddress ,TransferMethod ,QString&);
    ~TransferFile();

    unsigned int GetResult();
    unsigned int GetMethod();
    QString GetFilename();
    void start();

signals:
    void transfer_finished(int);

public slots:
    void when_connection_hasnewdata();
    void when_connection_established();
    void when_connection_error(QAbstractSocket::SocketError);

private:
    struct FileInfo{
            char filename[1024];
            unsigned int filemethod;
            unsigned int filelength;
    }FileInfo;

    QFile* filewillbetransferring;
    QHostAddress currenthostaddress;
    unsigned int result;
    bool finished_flag;
};


#endif // TRANSFERFILE_H
