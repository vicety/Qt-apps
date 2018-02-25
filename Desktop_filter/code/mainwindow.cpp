#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<filter.h>
#include<datacenter.h>
#include<QTimer>
#include<windows.h>
#include<sstream>
#include<QDebug>
#include<QFileDialog>
#include<QProgressDialog>
#include<QMenu>
#include<QCloseEvent>
#include<QAction>
#include<qapplication.h>
#include<QTime>
#include<QDirIterator>
#include<QMessageBox>

/*
 * 目标
 * 1. 本地指定文件夹图片轮播 √
 * 2. 缩小到右下角 √
 * 3. 筛选适合分表率的图片 √
 * 4. 自动读取当前分辨率
 * 5. 筛选精度 √
 * 6. 记录历史输入
 * 7. 爬虫接入
 *
 *
 */


//Filter f; 不可以在没有QApplication的情况下初始化

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //filter = new Filter(this); //写成单例在这里的 extern 会出问题...

    trayIcon = new QSystemTrayIcon(QIcon(":/res/icon.ico"),this);
    setWindowTitle("Desktop-Filter v1.0");
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    filter.settingModified = 0;
    noSuchImageCnt = 0;

    trayIcon->setToolTip("just show something");

    menu.addAction("下一张",this,SLOT(_nextImage()));
    menu.addAction("停止切换",this,SLOT(_stopSwitching()));
    menu.addAction("显示界面",this,SLOT(show()));
    menu.addAction("隐藏界面",this,SLOT(hide()));
    menu.addAction("退出",qApp,SLOT(quit()));
    trayIcon->setContextMenu(&menu);

    ui->lineEdit_intervalTime->setText("15");
    //datacenter = DataCenter();
    //QAction connect
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));


    //
    connect(&timer,SIGNAL(timeout()),this,SLOT(toNextImage()));
    connect(&show_timer,SIGNAL(timeout()),this,SLOT(setRemainingTime()));
    connect(this,SIGNAL(activate_show_timer()),this,SLOT(setRemainingTimeCounter()));
    connect(this,SIGNAL(manualNextImage()),this,SLOT(toNextImage()));
    //connect(filter,SIGNAL(newImageList(QStringList)),this,SLOT(setNewImageList(QStringList)));
    connect(&filter,SIGNAL(newCurPath(QString)),this,SLOT(setNewCurPath(QString)));
    //connect(&timer,SIGNAL(destroyed(QObject*)),show_timer
    connect(ui->textBrowser_infoSection,SIGNAL(cursorPositionChanged()),this,SLOT(textBrowserAutoScroll()));
    trayIcon->show();
    initStyle();

}

MainWindow::~MainWindow()
{
    //delete filter;
    delete trayIcon;
    //delete menu;
    delete ui;
}
void MainWindow::InsertMessage(QString str){
    ui->textBrowser_infoSection->insertPlainText(str);
}
bool MainWindow::applySettings(QString basePath){
    datacenter.curImageList.clear();
    getFullPath(basePath);
    if(datacenter.curImageList.empty()){
        InsertMessage("当前目录下无图片\n");
        return false;
    }
    InsertMessage(QString("找到图片")+QString::number(datacenter.curImageCount)+QString("张\n"));
    if(!ui->checkBox_randomAccess->isChecked())datacenter.curImageIndex = 0;
    else datacenter.curImageIndex = qrand()%datacenter.curImageCount;
    if(!toNextImage()) return false;
    if(timer.isActive()) timer.stop();
    timer.start(ui->lineEdit_intervalTime->text().toInt()*1000);
    emit activate_show_timer();
    return true;
}
bool MainWindow::toNextImage(){
    int curIndex = datacenter.curImageIndex;
    qDebug()<<datacenter.curImageList.at(curIndex);
    if(suit[curIndex] && filter.filtrate(datacenter.curImageList.at(curIndex))){

        if(setDesktop(datacenter.curImageList.at(curIndex))){
            noSuchImageCnt = 0;
            //qDebug()<<datacenter.curImageList[curIndex];
            InsertMessage("当前图片: "+QString::number(datacenter.curImageIndex)+QString("/")+QString::number(datacenter.curImageCount)+QString("\n"));
            InsertMessage(datacenter.curImageList[curIndex]+'\n');

            if(!ui->checkBox_randomAccess->isChecked())datacenter.curImageIndex = (curIndex+1)%datacenter.curImageCount;
            else datacenter.curImageIndex = qrand()%datacenter.curImageCount;
            qDebug()<<datacenter.curImageIndex<<datacenter.curImageCount;
            return true;
        }
        else{
            InsertMessage("设置桌面失败");
            return false;
        }
    }
    else{
        suit[curIndex] = false;
        if(noSuchImageCnt  ==  datacenter.curImageCount){
            InsertMessage("当前目录无符合要求的图片");
            return false;
        }
        noSuchImageCnt++;
        datacenter.curImageIndex = (datacenter.curImageIndex+1)%datacenter.curImageCount;
        toNextImage();
    }
}
bool MainWindow::setDesktop(QString imagepath){
    const char* tmp = imagepath.toStdString().c_str();
    std::wstringstream wss;
    wss<<tmp;
    const wchar_t* filename = wss.str().c_str();
    if(!SystemParametersInfo(SPI_SETDESKWALLPAPER,0,(void*)filename,SPIF_UPDATEINIFILE)){
        qDebug("change failed.");
        return false;
    }
    return true;
}

void MainWindow::on_pushButton_setPath_clicked()
{
    datacenter.curPath =  QFileDialog::getExistingDirectory(this,"选择图片目录","D:/");
    if(datacenter.curPath.isEmpty()) return;
    InsertMessage(datacenter.curPath);
    setNewCurPath(datacenter.curPath);
}


void MainWindow::on_pushButton_applySettings_clicked()
{
    applySettings(datacenter.curPath);
}
void MainWindow::setRemainingTime(){
    ui->label_nextTimeReamaining->setText(QString::number(timer.remainingTime()/1000)+"s");
}
void MainWindow::setRemainingTimeCounter(){
    show_timer.start(1000);
}

void MainWindow::on_pushButton_nextImage_clicked()
{
    emit manualNextImage();
    timer.start(ui->lineEdit_intervalTime->text().toInt()*1000);
    show_timer.start(1000);
}

void MainWindow::on_pushButton_autoFilter_clicked()
{
    filter.exec();
}
void MainWindow::breakLoop(){
    if(timer.isActive()) timer.stop();
    if(show_timer.isActive()) show_timer.stop();
}
void MainWindow::setNewImageList(QStringList list){
    datacenter.curImageList = list;
    breakLoop();
}
void MainWindow::setNewCurPath(QString basePath){
    datacenter.curPath = basePath;
}
void MainWindow::getFullPath(QString basePath){

    qDebug()<<"here";

    for(int i = 0;i<20000;i++) suit[i] = true;
    datacenter.curPath = basePath;
    if(ui->checkBox_scarchSubfolders->isChecked())findFile(basePath);
    else{
        datacenter.curImageList = QDir(basePath,"*.jpg *.png",QDir::Name,QDir::Files).entryList();
        for(int i = 0;i<datacenter.curImageList.count();i++){
            datacenter.curImageList[i] = basePath+'/'+datacenter.curImageList[i];
        }
    }
    datacenter.curImageCount = datacenter.curImageList.count();

}
void MainWindow::findFile(QString basePath){
    QDir dir(basePath);
    QStringList imgList = QDir(basePath,"*jpg *png",QDir::Name,QDir::Files).entryList();
    for(int i = 0;i<imgList.count();i++){
        imgList[i] = basePath+'/'+imgList[i];
    }
    datacenter.curImageList.append(imgList);
    dir.setFilter(QDir::Dirs);
    QFileInfoList infoList = dir.entryInfoList();
    int i = 0;
    do{
        QFileInfo fileInfo = infoList.at(i);
        if(fileInfo.fileName() == "." || fileInfo.fileName() == ".."){
            i++;
            continue;
        }
        if(fileInfo.isDir()){
            findFile(fileInfo.absoluteFilePath());
            i++;
        }
    }while(i<infoList.size());
}

void MainWindow::_showInterface(){
    if(!isVisible()) show();
}
void MainWindow::_hideInterface(){
    if(isVisible()) hide();
}
void MainWindow::_nextImage(){
    if(timer.isActive()) toNextImage();
}
void MainWindow::_stopSwitching(){
    breakLoop();
}
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason activationReason){
    if(activationReason == QSystemTrayIcon::Trigger){
        show();
    }
}
void MainWindow::closeEvent(QCloseEvent *event){
    if(isVisible()){
        hide();
        trayIcon->showMessage("Desktop",tr("最小化至系统托盘"));
        event->ignore();
    }
}
void MainWindow::initStyle()
{
    //加载样式表
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}
//TextBrower内容自动滚动
void MainWindow::textBrowserAutoScroll(){
    QTextCursor cursor = ui->textBrowser_infoSection->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser_infoSection->setTextCursor(cursor);
}

