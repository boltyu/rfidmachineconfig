#include "tipsbox.h"

TipsBox::TipsBox(TIPS_TYPE tipstype,QString &titletext,QWidget *parent)
    :QDialog (parent)
{
    setModal(true);
    setWindowFlag(Qt::WindowType::ToolTip);
    QGridLayout *gl = new QGridLayout;
//        hlayout->SetMaximumSize;
    QLabel *tt = new QLabel;
    tt->setText(titletext);
    gl->addWidget(tt,0,0,1,-1);

    switch (tipstype) {
    case TIPS_CONFIG:{

    }break;
    case TIPS_SCENSE:{
        QWidget *a = new QWidget;
        QWidget *b = new QWidget;
        QLabel *al = new QLabel;
        QLabel *bl = new QLabel;
        al->setText(tr("成功"));
        bl->setText(tr("失败"));
        a->setStyleSheet("background:green");
        b->setStyleSheet("background:red");
        a->setMaximumSize(20,10);
        b->setMaximumSize(20,10);
        gl->addWidget(a,1,0,1,2);
        gl->addWidget(b,2,0,1,2);
        gl->addWidget(al,1,1,1,1);
        gl->addWidget(bl,2,1,1,1);
    }break;

    }

    QPushButton *btnok = new QPushButton(tr("确定"),this);
    gl->addWidget(btnok,3,0,-1,-1);

    setLayout(gl);

    connect(btnok,&QPushButton::clicked,this,&TipsBox::deleteLater);

    show();
}


TipsBox::~TipsBox(){
}
