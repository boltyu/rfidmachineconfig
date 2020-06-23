#include "devicelist.h"



DeviceList::DeviceList(QWidget *parent)
    :QListWidget (parent)
{
    udps = new QUdpSocket;
    udps->open(QUdpSocket::OpenModeFlag::ReadWrite);
    currenthost.clear();
    connect(udps,SIGNAL(readyRead()),this,SLOT(on_Reply_from_machine()));
    connect(this,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_Select_device(QListWidgetItem*)));
    transfer = nullptr;
}

DeviceList::~DeviceList(){
    disconnect(udps,SIGNAL(readyRead()),this,SLOT(on_Reply_from_machine()));
    disconnect(this,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_Select_device(QListWidgetItem*)));

    udps->close();
    free(udps);
    udps = nullptr;
}

void DeviceList::on_Select_device(QListWidgetItem *item){

    currenthost.setAddress(item->text());
    qDebug("\nconnecting to host...");
    QString configfile = QString("%1%2%3").arg(SETTINGS_PATH).arg(currenthost.toString()).arg(".ini");
    transfer = new TransferFile(currenthost,TransferFile::Method_getconfig,configfile);
    connect(transfer,&TransferFile::transfer_finished,this,&DeviceList::transfer_finished);
    transfer->start();
}

void DeviceList::transfer_finished(int result){
    disconnect(transfer,&TransferFile::transfer_finished,this,&DeviceList::transfer_finished);
    qDebug("finished! %d",result);
    switch (result) {
    case TransferFile::Result_finish:
        emit jump_to_config_page();
    break;
    default:
        qDebug("transfer failed");
        break;
    }
    transfer->deleteLater();
    transfer = nullptr;
}


void DeviceList::refreshlist(quint16 broadcastport){
    this->clear();
    char peoplewords[] = "people_request_your_ip_address";
    qDebug("query word is %s",peoplewords);
    qint64 wc = udps->writeDatagram(peoplewords,strlen(peoplewords),QHostAddress::Broadcast,broadcastport);
    qDebug("sent %d bytes!",wc);
}

void DeviceList::on_Reply_from_machine(){ // udp signal slot
    while(udps->hasPendingDatagrams()){
        QHostAddress a;quint16 p=0;
        char ipchars[100];
        udps->readDatagram(ipchars,100,&a,&p);
        if(a.protocol() == QAbstractSocket::NetworkLayerProtocol::IPv6Protocol)
        {
            a.setAddress(a.toIPv4Address());
        }
        if(strcmp(ipchars,"sentinel here") == 0){
            qDebug("%s from %s",ipchars,a.toString().toLocal8Bit().data());
            insertItem(0,a.toString());
        }
    }
}

void DeviceList::UploadConfig(){
    try {
        if(0){
            QString tt(tr("配置文件仍在上传中..."));
            TipsBox *tb = new TipsBox(TIPS_CONFIG,tt,this);
            tb->show();
        }
        else{
            QString filename = QString("%1%2%3").arg(SETTINGS_PATH).arg(currenthost.toString()).arg(".ini");
            ts[0] = new TransferFile(currenthost,TransferFile::Method_putconfig,filename);
            connect(ts[0],&TransferFile::transfer_finished,this,&DeviceList::slot_upload_config);
            ts[0]->start();
        }
    } catch (QException::exception e) {
        qDebug(e.what());
    }


}

void DeviceList::slot_upload_config(int result){
    disconnect(ts[0],&TransferFile::transfer_finished,this,&DeviceList::slot_upload_config);
    QString tt;
    if(ts[0]->GetMethod() == TransferFile::Method_putconfig){
        if(result == TransferFile::Result_finish){
            tt = tr("已上传至%1").arg(currenthost.toString());

        }else{
            qDebug("transfer encounter error ");
            tt = tr("上传错误%1").arg(ts[0]->GetResult());
        }
    }
    TipsBox *tb = new TipsBox(TIPS_CONFIG,tt,this);
    tb->show();
    ts[0]->deleteLater();
    ts[0] = nullptr;
}
