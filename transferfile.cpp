#include "transferfile.h"


TransferFile::TransferFile(QHostAddress hostaddress,TransferMethod transfermethod,QString& absolutepath)
{
    currenthostaddress = hostaddress;
    FileInfo.filemethod = transfermethod;
    filewillbetransferring = new QFile(absolutepath);
    QFileInfo fi(absolutepath);  

    finished_flag = false;
    result = Result_wait;

    if(transfermethod == Method_putimg || transfermethod == Method_putconfig){
        FileInfo.filelength = filewillbetransferring->size();
        if(filewillbetransferring->exists() == false || filewillbetransferring->open(QFile::OpenModeFlag::ReadOnly) == false){
            qDebug("[ERROR] cant open file to upload: %s",fi.fileName().toLocal8Bit().data());
            //        deleteLater();
            result = Result_cantopen;
            return;
        }
    }else if(transfermethod == Method_getconfig){
        FileInfo.filelength = 0;
        if(filewillbetransferring->open(QFile::OpenModeFlag::WriteOnly) == false){
            qDebug("[ERROR] cant open cfg file : %s",absolutepath.toLocal8Bit().data());
            //        deleteLater();
            result = Result_cantopen;
            return;
        }
    }else if(transfermethod == Method_getimg){
        FileInfo.filelength = 0;
        if(filewillbetransferring->open(QFile::OpenModeFlag::WriteOnly) == false){
            qDebug("[ERROR] cant open img : %s",absolutepath.toLocal8Bit().data());
            //        deleteLater();
            result = Result_cantopen;
            return;
        }
    }


    memset(FileInfo.filename,0,sizeof(FileInfo.filename));
    strcpy_s(FileInfo.filename,fi.fileName().toUtf8().data());
    qDebug("filename is %s",FileInfo.filename);

//    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

    connect(this,&QTcpSocket::readyRead,this,&TransferFile::when_connection_hasnewdata);
    connect(this,&QTcpSocket::connected,this,&TransferFile::when_connection_established);
    connect(this,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&TransferFile::when_connection_error);


}

unsigned int TransferFile::GetResult(){
    return result;
}

unsigned int TransferFile::GetMethod(){
    return FileInfo.filemethod;
}

QString TransferFile::GetFilename(){
    return QString::fromLocal8Bit(FileInfo.filename);
}

void TransferFile::when_connection_established(){
//    tcpsocket->open(QTcpSocket::OpenModeFlag::ReadWrite);
    qint64 wc = write((char*)&FileInfo,sizeof (FileInfo));
    qDebug("write fileinfo total %d bytes",wc);
}

void TransferFile::when_connection_hasnewdata(){

    switch (FileInfo.filemethod) {
    case Method_getconfig:
    {
        if(filewillbetransferring->write(readAll()) == -1){
           qDebug("error occured in file writting: %d,%s",filewillbetransferring->error(),filewillbetransferring->errorString().toLocal8Bit().data());
           result = Result_filecrash;
        }else{
           qDebug("config file received");
           result = Result_finish;
        }
        filewillbetransferring->close();

    }
        break;
    case Method_getimg:
    {
        if(FileInfo.filelength == 0){
            FileInfo.filelength = 0;
            read((char*)&(FileInfo.filelength),sizeof(unsigned int));
            return;
        }else if(bytesAvailable() > 3){
            QByteArray payload = readAll();
            FileInfo.filelength -= (unsigned int)payload.length();
            filewillbetransferring->write(payload);
            qDebug("Reveiving file: %s, pack len:%d, left: %d",FileInfo.filename,payload.length(),FileInfo.filelength);
            if(FileInfo.filelength > 0){
                return;
            }else{
                filewillbetransferring->close();
                result = Result_finish;
            }
        }else{
            qDebug("Get img crashed!");
            result = Result_filecrash;
        }
    }
        break;
    case Method_putimg: case Method_putconfig:
    {
        if(bytesAvailable() != 3){
            qDebug("[WARNING] reply from machine (%d bytes) is not a standard format (3bytes)",bytesAvailable());
        }
        char reply[3];
        read(reply,3);
        if(strncmp(reply,"get",3) == 0){
            qDebug("file transferring...");
            if(write(filewillbetransferring->readAll()) == -1){
                qDebug("error occured in file transferring: %d,%s",error(),errorString().toLocal8Bit().data());
                result = Result_netcrash;
            }else{
                filewillbetransferring->close();
                return; // halt here , and wait for next reply
            }

        }else if(strncmp(reply,"got",3) == 0)
        {
            qDebug("file tranffered!");
            result = Result_finish;

        }else if(strncmp(reply,"bad",3) == 0){
            result = Result_netcrash;

            qDebug("[ERROR] file upload failed");
        }
    }
        break;
    default:
        qDebug("[ERROR]: Method unknown,crash");
        break;
    }

    disconnectFromHost();
    emit transfer_finished(result);
    finished_flag = 1;

}

void TransferFile::when_connection_error(QAbstractSocket::SocketError erno){
    qDebug("[WARNING]: connection encounter error: %d , and tcpsocket report: %s",erno,errorString().toLocal8Bit().data());
    finished_flag = 1;
    result = Result_netcrash;
    emit transfer_finished(result);

}


TransferFile::~TransferFile(){


//    disconnect(this,&QTcpSocket::readyRead,this,&TransferFile::when_connection_hasnewdata);
//    disconnect(this,&QTcpSocket::connected,this,&TransferFile::when_connection_established);
//    disconnect(tcpsocket,&QTcpSocket::disconnected,this,&TransferFile::deleteLater);
//    disconnect(this,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&TransferFile::when_connection_error);

    if(filewillbetransferring->isOpen())
        filewillbetransferring->close();
    free(filewillbetransferring);
}

void TransferFile::start(){
    if(result == Result_wait){
        qDebug("transfer begin!");
        connectToHost(currenthostaddress,2019,QTcpSocket::OpenModeFlag::ReadWrite);
    }
    else {
        emit transfer_finished(Result_cantopen);
        qDebug("transfer cant not be started because file is invalid");
    }

}



