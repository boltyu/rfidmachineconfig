#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::cookmenu()
{
    devicelist = new QMenu("设备列表",this);
        adddevice = new QAction("添加",devicelist);
        connect(adddevice,&QAction::triggered,this,&MainWindow::slot_adddevice);
        refreshlist = new QAction("刷新",devicelist);
        connect(refreshlist,&QAction::triggered,this,&MainWindow::slot_refreshlist);
        uploadsettings = new QAction(tr("upload"),devicelist);
        uploadsettings->setToolTip(tr("upload current settings to Rfid Machine"));
        connect(uploadsettings,&QAction::triggered,this,&MainWindow::slot_uploadsettings);
        exitprogram = new QAction(tr("exit"),devicelist);
        connect(exitprogram,&QAction::triggered,this,&MainWindow::slot_exitprogram);


    devicelist->addAction(adddevice);
    devicelist->addAction(refreshlist);
    devicelist->addSeparator();
    devicelist->addAction(uploadsettings);
    devicelist->addSeparator();
    devicelist->addAction(exitprogram);

    adscense = new QMenu("广告控制",this);
        addscense = new QAction("添加条目",adscense);
        connect(addscense,&QAction::triggered,this,&MainWindow::slot_addscense);
        removescense = new QAction("remove all",adscense);
        connect(removescense,&QAction::triggered,this,&MainWindow::slot_removescense);
        uploadscense = new QAction("上传图片");
        connect(uploadscense,&QAction::triggered,this,&MainWindow::slot_uploadscense);
        downloadscense = new QAction("下载图片");
        connect(downloadscense,&QAction::triggered,this,&MainWindow::slot_downloadscense);

    adscense->addAction(addscense);
    adscense->addAction(removescense);
    adscense->addSeparator();
    adscense->addAction(uploadscense);
    adscense->addAction(downloadscense);

    adscense->setEnabled(false);

    ui->menubar->addMenu(devicelist);
    ui->menubar->addMenu(adscense);
}

void MainWindow::slot_uploadscense(){
    ui->scenselist->SyncScense(ui->devicelist->currenthost,TransferFile::Method_putimg);
}

void MainWindow::slot_addscense()
{
    QFileInfo fff(QFileDialog::getOpenFileName(this, tr("选择图片"), "./", tr("Image Files (*.png *.jpg *.bmp)")));
    if(fff.isReadable()){
        QString absolutepath = fff.absoluteFilePath();
        QString displaytime = "300";
        ui->scenselist->InsertScense(ui->scenselist->count(),absolutepath,displaytime);
    }
    else
    {
        qDebug("Can't open %s for reading",fff.fileName().toLocal8Bit().data());
    }

}


void MainWindow::slot_removescense()
{
    ui->scenselist->RemoveAll();

}

void MainWindow::slot_adddevice()
{
//    QFile cfgfile(CONFIG_FILE_PATH);
}
void MainWindow::slot_refreshlist()
{
    ui->devicelist->refreshlist(1949);
    ui->stackedWidget->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);
}
void MainWindow::slot_exitprogram()
{
    exit(0);
}

void MainWindow::slot_uploadsettings()
{
    QString filename = QString("%1%2%3").arg(SETTINGS_PATH).arg(ui->devicelist->currenthost.toString()).arg(".ini");
    QString oldfilename = QString("%1%2").arg(filename).arg(SETTINGS_FILE_OLD);
    QFile cfgfile(filename);
    QFile::remove(oldfilename);
    if(!cfgfile.copy(oldfilename)){
        qDebug("[Error] failed to backup config file");
    }
    QFile newcfgfile(filename);
    newcfgfile.remove();

    if(ui->devicelist->transfer != nullptr){
        qDebug("[WARNING]: last transfer operation still running");
        return;
    }

    QSettings st(filename,QSettings::Format::IniFormat);

    st.beginGroup("NetWork");
    st.setValue("address",ui->lineEdit_address->text());
    st.setValue("netmask",ui->lineEdit_netmask->text());
    st.setValue("gateway",ui->lineEdit_gateway->text());

    st.endGroup();
    st.beginGroup("AdScense");
    qDebug("scount %d",ui->scenselist->count());
    for(int scensecount = 0; scensecount < ui->scenselist->count(); scensecount ++){
        QString filepath = ui->scenselist->GetFilename(scensecount);
        st.setValue(QString("scense%1").arg(scensecount),filepath);
    }
    st.endGroup();

    st.sync();

    ui->devicelist->UploadConfig();

}

void MainWindow::slot_downloadscense(){
//    ui->scenselist->DownloadAllScense(ui->devicelist->currenthost);
}

void MainWindow::slot_exportsettings(){

}

void MainWindow::slot_importsettings(){

}


