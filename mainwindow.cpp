#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir qdir;
    qdir.mkdir("./Adscense");
    qdir.mkdir("./Settings");

    cookmenu();

    connect(ui->devicelist,&DeviceList::jump_to_config_page,this,&MainWindow::slot_Load_config);
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,&MainWindow::slot_tab_changed);

}

void MainWindow::slot_Load_config(){
    QString filename = QString("%1%2%3").arg(SETTINGS_PATH).arg(ui->devicelist->currenthost.toString()).arg(".ini");
    QSettings allsettings(filename,QSettings::Format::IniFormat);
    allsettings.beginGroup("NetWork");
    ui->lineEdit_address->setText(allsettings.value("address").toString());
    ui->lineEdit_netmask->setText(allsettings.value("netmask").toString());
    ui->lineEdit_gateway->setText(allsettings.value("gateway").toString());
    allsettings.endGroup();

    ui->scenselist->RemoveAll();

    int scensecount = 0;
    allsettings.beginGroup("AdScense");

    while(allsettings.contains(QString("scense%1").arg(scensecount))){
        QString filename = QString("./Adscense/%1").arg(allsettings.value(QString("scense%1").arg(scensecount)).toString());
        QString displaytime = allsettings.value("displaytime","300").toString();
        ui->scenselist->InsertScense(scensecount,filename,displaytime);
        scensecount++;
    }
    ui->scenselist->SyncScense(ui->devicelist->currenthost,TransferFile::Method_getimg);

    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::slot_tab_changed(int index){
    switch (index) {
    case tab_adscense:
    {
        adscense->setEnabled(true);

    }
        break;
    default:
        adscense->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_adsettings_clicked()
{
    ui->tabWidget->setCurrentIndex(tab_adscense);
}
