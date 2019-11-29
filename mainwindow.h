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
    enum tabpages{
        tab_network = 0,
        tab_btnsettings,
        tab_adscense
    };

public slots:
    void slot_Load_config();

private slots:
    void slot_adddevice();
    void slot_refreshlist();
    void slot_exitprogram();
    void slot_uploadsettings();
    void slot_rebootdevice();
    void slot_exportsettings();
    void slot_importsettings();
    void slot_addscense();
    void slot_removescense();
    void slot_uploadscense();
    void slot_downloadscense();

    void slot_tab_changed(int index);

    void on_adsettings_clicked();

private:
    Ui::MainWindow *ui;

    void cookmenu();



    QMenu *devicelist;
        QAction *adddevice;
        QAction *refreshlist;
        QAction *scensesettings;
        QAction *uploadsettings;
        QAction *rebootdevice;
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
