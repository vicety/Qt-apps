#include "filter.h"
#include "ui_filter.h"
#include<datacenter.h>
#include<mainwindow.h>
#include<QStringList>
#include<QDir>
#include<QImage>
#include<QFileDialog>
#include<QDebug>
#include<QPixmap>
#include<QProgressDialog>

//extern DataCenter datacenter;
int Filter::settingModified;


Filter::Filter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::filter)
{
    ui->setupUi(this);
    if(!datacenter.curPath.isEmpty()) ui->lineEdit_sourceDir->setText(datacenter.curPath);
    setWindowTitle("筛选");
    //qDebug()<<"constructing";
}

Filter::~Filter()
{
    delete ui;
}
bool Filter::filtrate(QString imagePath){  //不需要第二个static
    //QStringList list = QDir(basePath,"*.jpg",QDir::Name).entryList();
//    QProgressDialog process("处理进度","取消",0,list.count()-1,this);
//    process.setWindowTitle(QObject::tr("进度"));
//    process.setWindowModality(Qt::WindowModal);
//    process.show();
//    for(int i = 0;i<list.count();i++) list[i] = basePath+'/'+list[i];
//    QStringList newlist;
//    QPixmap img;
//    for(int i = 0;i<list.count();i++){
//        if(!img.load(list[i])) continue;
//        if(judge_resolution(img.width(),img.height())) newlist.append(list[i]);
//        process.setValue(i);
//        if(process.wasCanceled()) break;
//    }
//    return newlist;
    if(!Filter::settingModified) return true;
    QImage img;
    img.load(imagePath);
    if(Filter::judge_resolution(img.width(),img.height())) return true;
    else return false;
}

void Filter::on_pushButton_apply_clicked()
{
//    if(ui->lineEdit_resolutionHeight->text().isEmpty() || ui->lineEdit_resolutionWidth->text().isEmpty() || ui->lineEdit_sourceDir->text().isEmpty()) return;
//    QStringList newlist = filtrate(ui->lineEdit_sourceDir->text());
//    emit newImageList(newlist);
    Filter::settingModified = 1;
    datacenter.maxWHrate = ui->lineEdit_maxWHrate->text().toDouble();
    datacenter.minWHrate = ui->lineEdit_minWHrate->text().toDouble();
    datacenter.minHrate = ui->lineEdit_minHrate->text().toDouble();
    datacenter.minWrate = ui->lineEdit_minWrate->text().toDouble();
    //if(ui->checkBox_subFolderIncluded) emit includeSubfolder;
    emit newCurPath(ui->lineEdit_sourceDir->text());
    accept();
    //验证是否填满[[
}

bool Filter::judge_resolution(double width, double height){
    double screen_width = ui->lineEdit_resolutionWidth->text().toDouble();
    double screen_height = ui->lineEdit_resolutionHeight->text().toDouble();
    double ori_rate = screen_width/screen_height;
    double rate = width/height;
    if(width<datacenter.minWrate*screen_width || height<datacenter.minHrate*screen_height) return false;
    if(rate<datacenter.minWHrate*ori_rate || rate>datacenter.maxWHrate*ori_rate) return false;
    return true;
}

void Filter::on_pushButton_browseSource_clicked()
{
    QString tmp = QFileDialog::getExistingDirectory(this,"选择图片目录","D:/");

    if(!tmp.isEmpty()) ui->lineEdit_sourceDir->setText(tmp);
}

void Filter::on_pushButton_cancel_clicked()
{
    reject();
}
