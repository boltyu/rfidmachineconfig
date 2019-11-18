#include "scenselist.h"

ScenseList::ScenseList(QWidget *parent) : QWidget(parent)
{
    layout_adscense = new QVBoxLayout;
    setLayout(layout_adscense);

}

void ScenseList::InsertScense(int scensepos,QString& absolutepath,QString& displaytime){
    QWidget* widget = new QWidget;
    widget->setStyleSheet("QWidget{border:1px solid black;}");
    QVBoxLayout *newscense = new QVBoxLayout;
        QHBoxLayout *scenseinfo = new QHBoxLayout;
            QLabel* lll = new QLabel;
            lll->setText(QString("场景%1").arg(scensepos));
            lll->setStyleSheet("");
            QLineEdit* ttt = new QLineEdit;
            ttt->setText(absolutepath);
            ttt->setMaximumHeight(25);
            ttt->setReadOnly(true);
            QPushButton *moveup = new QPushButton("UP");
            connect(moveup,&QPushButton::clicked,this,&ScenseList::slot_scense_operation);
            moveup->setCheckable(true);
            moveup->setMinimumWidth(10);
            QPushButton *movedown = new QPushButton("DOWN");
            connect(movedown,&QPushButton::clicked,this,&ScenseList::slot_scense_operation);
            movedown->setCheckable(true);
            movedown->setMinimumWidth(10);
            QPushButton *remove = new QPushButton("X");
            remove->setCheckable(true);
            remove->setMinimumWidth(2);
            connect(remove,&QPushButton::clicked,this,&ScenseList::slot_scense_operation);
        QGraphicsView *imgpreview = new QGraphicsView;
            QGraphicsScene *imgscense = new QGraphicsScene;
            imgscense->addPixmap(QPixmap::fromImage(QImage(absolutepath)));
            imgpreview->setScene(imgscense);

    scenseinfo->addWidget(lll,1);
    scenseinfo->addWidget(ttt,4);
    scenseinfo->addWidget(moveup,1);
    scenseinfo->addWidget(movedown,1);
    scenseinfo->addWidget(remove,1);
    newscense->addLayout(scenseinfo);
    newscense->addWidget(imgpreview);
    widget->setLayout(newscense);
    layout_adscense->insertWidget(scensepos,widget,1);
}

void ScenseList::RemoveScense(int scensepos){
    QWidget *widget = (QWidget*)layout_adscense->itemAt(scensepos)->widget();
    widget->deleteLater();
}

void ScenseList::RemoveAll(){
    int scensecount = layout_adscense->count();

    for(int scensepos = 0;scensepos < scensecount; scensepos ++){
        qDebug("%d/%d",scensepos,scensecount);
        RemoveScense(scensepos);
    }
}

void ScenseList::SyncScense(QHostAddress &ch,TransferFile::TransferMethod transfermethod){
    currenthost = ch;
    currentmethod = transfermethod;
    transfercount = 0;
    if(count() > 0){
        QString imgfile = GetFilepathandname(transfercount);
        ts[transfercount] = new TransferFile(currenthost,currentmethod,imgfile);
        connect(ts[transfercount],&TransferFile::transfer_finished,this,&ScenseList::slot_sync_scense);
        ts[transfercount]->start();
    }
}

void ScenseList::slot_sync_scense(int result){
    disconnect(ts[transfercount],&TransferFile::transfer_finished,this,&ScenseList::slot_sync_scense);
    if(result == TransferFile::Result_finish){
        qDebug("scense%d transfered",transfercount);
        MarkSyncStatus(transfercount,true);
    }else{
        qDebug("transfer encounter error");
        MarkSyncStatus(transfercount,false);
    }

    if(currentmethod == TransferFile::Method_getimg){
        QGraphicsScene* gs = new QGraphicsScene();
        gs->addPixmap(QPixmap::fromImage(QImage(GetFilepathandname(transfercount))));
        ((QGraphicsView*)(layout_adscense->itemAt(transfercount)->widget()->layout()->itemAt(1)->widget()))->setScene(gs);
    }

    ts[transfercount]->deleteLater();
    transfercount ++;
    qDebug("%d/%d",transfercount,count());
    if(transfercount < count()){
        QString imgfile = GetFilepathandname(transfercount);
        ts[transfercount] = new TransferFile(currenthost,currentmethod,imgfile);
        connect(ts[transfercount],&TransferFile::transfer_finished,this,&ScenseList::slot_sync_scense);
        ts[transfercount]->start();
    }


}

void ScenseList::DownloadAllScense(QHostAddress &ch){

}
void ScenseList::slot_download_scense(int result){

}
void ScenseList::UploadAllScense(QHostAddress &currenthost){

}
void ScenseList::slot_upload_scense(int result){

}

int ScenseList::count(){
    return layout_adscense->count();
    count();
}

QString ScenseList::GetFilename(int index){
    QWidget *widget = (QWidget*)layout_adscense->itemAt(index)->widget();
    QVBoxLayout *currentscense = (QVBoxLayout*)widget->layout();
    QHBoxLayout *fileinfo = (QHBoxLayout*)currentscense->itemAt(0);
    QLineEdit *filepath = (QLineEdit*)fileinfo->itemAt(1)->widget();
    return filepath->text().right(filepath->text().length() - filepath->text().lastIndexOf('/',-1) - 1);
}

QString ScenseList::GetFilepathandname(int index){
    QWidget *widget = (QWidget*)layout_adscense->itemAt(index)->widget();
    QVBoxLayout *currentscense = (QVBoxLayout*)widget->layout();
    QHBoxLayout *fileinfo = (QHBoxLayout*)currentscense->itemAt(0);
    QLineEdit *filepath = (QLineEdit*)fileinfo->itemAt(1)->widget();
    return filepath->text();
}

void ScenseList::MarkSyncStatus(int index,bool status){
    if(status){
        layout_adscense->itemAt(index)->widget()->layout()->itemAt(0)->layout()->itemAt(0)->widget()->setStyleSheet("background:green");
    }
    else {
        layout_adscense->itemAt(index)->widget()->layout()->itemAt(0)->layout()->itemAt(0)->widget()->setStyleSheet("background:red");
    }
}

void ScenseList::slot_scense_operation(){
    int scensecnt = layout_adscense->count();
    int count = 0;
    int dir = 0;
    for(count =0; count < scensecnt; count ++){
        QHBoxLayout *scenseinfo = (QHBoxLayout*)layout_adscense->itemAt(count)->widget()->layout()->itemAt(0);
        QPushButton *btnup = (QPushButton*)scenseinfo->itemAt(2)->widget();
        QPushButton *btndown = (QPushButton*)scenseinfo->itemAt(3)->widget();
        QPushButton *btnremove = (QPushButton*)scenseinfo->itemAt(4)->widget();
        if(btnup->isChecked()){
            dir = -1;
            btnup->setChecked(false);
            break;
        }else if(btndown->isChecked()){
            dir = 1;
            btndown->setChecked(false);
            break;
        }else if(btnremove->isChecked()){
            btndown->setChecked(false);
            RemoveScense(count);
            for(int index = 0; index < scensecnt; index ++){
                ((QLabel*)(layout_adscense->itemAt(index)->widget()->layout()->itemAt(0)->layout()->itemAt(0)->widget()))->setText(QString("场景%1").arg(index>count?index-1:index));
            }
            return;
        }
    }

    if( (dir == -1 && count > 0) || (dir == 1 && count < (scensecnt -1)))
    {
        QWidget* currentscense = (QWidget*)layout_adscense->itemAt(count)->widget();
        layout_adscense->removeWidget(currentscense);
        layout_adscense->insertWidget(count+dir,currentscense,1);
        /*reIndex*/

    }

    for(int index = 0; index < scensecnt; index ++){
        ((QLabel*)(layout_adscense->itemAt(index)->widget()->layout()->itemAt(0)->layout()->itemAt(0)->widget()))->setText(QString("场景%1").arg(index));
    }

}
