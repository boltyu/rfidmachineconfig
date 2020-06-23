#ifndef TIPSBOX_H
#define TIPSBOX_H

#include <QDialog>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

enum TIPS_TYPE{
    TIPS_SCENSE,
    TIPS_CONFIG
};

class TipsBox : public QDialog
{
    Q_OBJECT
public:
    TipsBox(TIPS_TYPE,QString &,QWidget *parent);
    ~TipsBox();
};

#endif // TIPSBOX_H
