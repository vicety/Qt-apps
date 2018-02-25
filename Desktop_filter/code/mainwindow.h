#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMenu>
#include<QCloseEvent>
#include<QSystemTrayIcon>
#include<QAction>
#include<QTimer>
#include<datacenter.h>
#include"filter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void InsertMessage(QString str);
    bool applySettings(QString basepath);
    QTimer timer;
    QTimer show_timer;
    DataCenter datacenter;
    Filter filter;
    QMenu menu;
    ~MainWindow();
private slots:
    void textBrowserAutoScroll();
    void _showInterface();
    void _hideInterface();
    void _nextImage();
    void _stopSwitching();
    void trayIconActivated(QSystemTrayIcon::ActivationReason activationReason);
    bool toNextImage();
    void setRemainingTime();
    void setRemainingTimeCounter();
    void setNewImageList(QStringList list);
    void setNewCurPath(QString basePath);

    void on_pushButton_setPath_clicked();

    void on_pushButton_applySettings_clicked();
    void on_pushButton_nextImage_clicked();

    void on_pushButton_autoFilter_clicked();
    void on_action_instructions_triggered();

protected:
    void closeEvent(QCloseEvent *event);
signals:
    void activate_show_timer(void);
    void manualNextImage(void);
private:
    bool searchIncludeSubfolder;
    void getFullPath(QString basePath);
    void initStyle();
    void findFile(QString basePath);
    QSystemTrayIcon* trayIcon;
    int noSuchImageCnt;
    bool suit[100000];
    void breakLoop();
    bool setDesktop(QString imagepath);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
