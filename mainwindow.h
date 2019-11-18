#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "devicelist.h"
#include "scenselist.h"
#include "transferfile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QUdpSocket *udps;


public slots:
    void slot_Load_config();

private slots:
    void slot_adddevice();
    void slot_refreshlist();
    void slot_exitprogram();
    void slot_uploadsettings();
    void slot_exportsettings();
    void slot_importsettings();
    void slot_addscense();
    void slot_removescense();
    void slot_uploadscense();
    void slot_downloadscense();

    void slot_tab_changed(int index);

private:
    Ui::MainWindow *ui;

    void cookmenu();



    QMenu *devicelist;
        QAction *adddevice;
        QAction *refreshlist;
        QAction *scensesettings;
        QAction *uploadsettings;
        QAction *importsettings;
        QAction *exportsettings;
        QAction *exitprogram;

    QMenu *adscense;
        QAction *addscense;
        QAction *removescense;
        QAction *uploadscense;
        QAction *downloadscense;


};




#endif // MAINWINDOW_H
