#include "pic_converter.h"
#include<QImage>
#include<cmath>
#include<QProgressDialog>
#include<QString>
#include<QCoreApplication>
pic_converter::pic_converter(QImage _img)
{
    img = _img;
}
QImage pic_converter::start(QWidget* parent, int dif, bool isBlackandwhite)
{
    QImage* newimg = new QImage(img.width(),img.height(),QImage::Format_ARGB32);
    newimg->fill(Qt::white);
    QProgressDialog process(QObject::tr("图像处理进度"),QObject::tr("取消"),0,img.height()-2,parent);
    process.setWindowTitle(QObject::tr("进度"));
    process.setWindowModality(Qt::WindowModal);
    process.show();
    QRgb* line1;
    QRgb* line2 = (QRgb*)img.scanLine(0);
    QRgb* line3 = (QRgb*)img.scanLine(1);
    for(int y = 1;y<img.height()-1;y++)
    {
        process.setValue(y);
        if(process.wasCanceled()) break;//break应该不返回图像的
        QCoreApplication::processEvents();
        line1 = line2;
        line2 = line3;
        line3 = (QRgb*)img.scanLine(y+1);
        for(int x = 1;x<img.width()-1;x++)
        {
            int res = 0;
            int r = qRed(line2[x]);
            int g = qGreen(line2[x]);
            int b = qBlue(line2[x]);
            for(int i = -1;i<=1;i++) if(getdif(line1[x+i],r,g,b)>=dif) res++;
            for(int i = -1;i<=1;i++) if(getdif(line3[x+i],r,g,b)>=dif) res++;
            if(getdif(line2[x+1],r,g,b)>=dif) res++;
            if(getdif(line2[x-1],r,g,b)>=dif) res++;
            if(res>=3&&!isBlackandwhite) newimg->setPixel(x,y,QRgb(line2[x]));
            if(res>=3&&isBlackandwhite) newimg->setPixel(x,y,Qt::black);
        }
    }
    return *newimg;
}
int pic_converter::getdif(QRgb tar,int r, int g,int b)
{
    double ans = 0;
    ans+=pow(qRed(tar)-r,2);
    ans+=pow(qGreen(tar)-g,2);
    ans+=pow(qBlue(tar)-b,2);
    ans = sqrt(ans);
    return int(ans);
}
