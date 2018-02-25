#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pic_converter.h"
#include<QProcess>
#include<QImage>
#include<QDebug>
#include<QDir>
#include<QString>
#include<QQueue>
#include<QFileDialog>
#include<QTextEdit>
#include<QDragEnterEvent>
#include<QUrl>
#include<QFile>
#include<QMimeData>
#include<QColorDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    Status_info = new Status_info_queue(5);
    this->setWindowTitle("sketch-converter v1.0");
    connect(ui->checkBox_sizefixed,SIGNAL(stateChanged(int)),this,SLOT(change_pic_to_fixed()));
    init();
    initStyle();
}
void MainWindow::init()
{
    status_update("welcome to vicety's sketch converting software!");
    QValidator* validator_input_dif = new QIntValidator(0,500,this);//411.673
    ui->input_dif->setValidator(validator_input_dif);
    ui->scrollArea->setWidget(ui->pic_area);
}

MainWindow::~MainWindow()
{
    delete ui;
}




bool MainWindow::on_button_savepic_clicked()
{
    QString savefilename = QFileDialog::getSaveFileName(this,tr("保存图片"),"D:");
    if(!new_image.save(savefilename))
    {
        status_update(tr("保存失败 请检查是否进行过图片转换"));
        return false;
    }
    status_update(tr("图片成功保存于 ")+savefilename);
    return true;
}

void MainWindow::status_update(QString _str)
{
    /*Status_info->add(_str);
    ui->status_area->setText(Status_info->getstr());
    qDebug()<<Status_info->getstr();*/
    ui->status_area->insertPlainText(_str+'\n');
}

bool MainWindow::on_button_openpic_clicked()
{
    QString pic_filename = QFileDialog::getOpenFileName(this,tr("选择图片"),"D:",tr("图片文件（* png * jpg * bmp"));
    if(!ori_image.load(pic_filename))
    {
        status_update(tr("打开图片失败"));
        return false;
    }
    cur_image = ori_image;
    show_image(cur_image);
    return true;
}

void MainWindow::on_button_start_clicked()
{
    pic_converter converter(ori_image);
    if(ui->input_dif->text().isEmpty())
    {
        status_update("请先输入参数");
        return;
    }
    new_image = converter.start(this,ui->input_dif->text().toInt(),ui->checkBox_blackwhite->isChecked());
    cur_image = new_image;

    show_image(new_image);
}

bool MainWindow::on_button_show_oripic_clicked()//函数名有问题 以后再改
{
    cur_image = cur_image == ori_image?new_image:ori_image;
    if(cur_image.isNull())
    {
        status_update(tr("暂不能切换图片"));
        return false;
    }

    show_image(cur_image);
    return true;
}

void MainWindow::change_pic_to_fixed()
{
    QImage image_to_show;
    if(ui->checkBox_sizefixed->isChecked())image_to_show = cur_image.scaled(ui->scrollArea->size(),Qt::KeepAspectRatio);//ui->scrollArea->size()
    else image_to_show = cur_image;
    ui->pic_area->setPixmap(QPixmap::fromImage(image_to_show));
    ui->pic_area->show();
}

void MainWindow::show_image(QImage img)
{
    if(ui->checkBox_sizefixed->isChecked())ui->pic_area->setPixmap(QPixmap::fromImage(img.scaled(ui->pic_area->size(),Qt::KeepAspectRatio)));
    else ui->pic_area->setPixmap(QPixmap::fromImage(img));//new_image.scaled(ui->pic_area->size(),Qt::KeepAspectRatio
    ui->pic_area->show();
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    //if(event->mimeData()->hasImage())qDebug()<<"has image";
    //qDebug()<<event->mimeData()->urls()[0];
    if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
            ||!event->mimeData()->urls()[0].fileName().right(3).compare("png"))
    {
        event->acceptProposedAction();;
    }
    else event->ignore();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        QList<QUrl>urlList = mimeData->urls();
        QString fileName = urlList.at(0).toLocalFile();
        if(!fileName.isEmpty())
        {
            qDebug()<<fileName;
            if(fileName.endsWith("jpg")||fileName.endsWith("png"))
            {
                ori_image.load(fileName);
                cur_image = ori_image;
                new_image = cur_image;
                show_image(cur_image);
            }
        }
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
        ui->action_changeto_psblack->setChecked(true);
        //ui->action_changeto_psblack->setCheckable(false);
    }
}



void MainWindow::on_action_changeto_psblack_toggled(bool checked)
{
    if(ui->action_changeto_psblack->isChecked())  //应该是check之后
    {
        QFile file(":/qss/psblack.css");
        if (file.open(QFile::ReadOnly)) {
            QString qss = QLatin1String(file.readAll());
            QString paletteColor = qss.mid(20, 7);
            qApp->setPalette(QPalette(QColor(paletteColor)));
            qApp->setStyleSheet(qss);
            file.close();
            ui->action_changeto_psblack->setChecked(true);
            ui->action_changeto_lightgrey->setChecked(false);
        }
        //ui->action_changeto_lightgrey->setChecked(false);
    }

}

void MainWindow::on_action_changeto_lightgrey_toggled(bool checked)
{
    if(ui->action_changeto_lightgrey->isChecked()){
        QFile file(":/qss/lightgrey.css");
        if (file.open(QFile::ReadOnly)) {
            QString qss = QLatin1String(file.readAll());
            QString paletteColor = qss.mid(20, 7);
            qApp->setPalette(QPalette(QColor(paletteColor)));
            qApp->setStyleSheet(qss);
            file.close();
            ui->action_changeto_lightgrey->setChecked(true);
            ui->action_changeto_psblack->setChecked(false);
        }
        //ui->action_changeto_psblack->setChecked(false);
    }
}
