#ifndef SCENSELIST_H
#define SCENSELIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "transferfile.h"
#include "tipsbox.h"
#define SCENSE_PATH "./Adscense/"


class ScenseList : public QWidget
{
    Q_OBJECT
public:
    explicit ScenseList(QWidget *parent = nullptr);
    void RemoveScense(int);
    void RemoveAll();
    void UploadAllScense(QHostAddress&);
    void DownloadAllScense(QHostAddress&);
    void SyncScense(QHostAddress&,TransferFile::TransferMethod);
    void InsertScense(int,QString&,QString&);
    QString GetFilename(int);
    QString GetFilepathandname(int);
    void MarkSyncStatus(int,bool);
    int count();
signals:

private slots:
    void slot_scense_operation();
    void slot_upload_scense(int);
    void slot_download_scense(int);
    void slot_sync_scense(int);
private:
    TransferFile *ts[128];
    TransferFile::TransferMethod currentmethod;
    QHostAddress currenthost;

    int transfercount;
    QVBoxLayout *layout_adscense;
};

#endif // SCENSELIST_H
