#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QListWidget>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QThread>
#include <QtWidgets>
#include <QtDebug>
#include <QMessageBox>
#include "transferfile.h"

#define SETTINGS_PATH "./Settings/"
#define SETTINGS_FILE_OLD "old"

class DeviceList : public QListWidget{
    Q_OBJECT
public:
    DeviceList(QWidget *parent = nullptr);
    ~DeviceList();
    void refreshlist(quint16 broadcastport);

    TransferFile *transfer;
    QUdpSocket *udps;
    QHostAddress currenthost;
    void UploadConfig();

public slots:
    void transfer_finished(int);

signals:
    void jump_to_config_page();
public slots:
    void on_Reply_from_machine();
    void on_Select_device(QListWidgetItem *item);
    void slot_upload_config(int);

private:
    TransferFile *ts[128];
    int transfercount;
};


#endif // DEVICELIST_H
