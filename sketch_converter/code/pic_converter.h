#ifndef PIC_CONVERTER_H
#define PIC_CONVERTER_H
#include<QImage>
#include<cmath>

class pic_converter
{
    //Q_OBJECT
public:
    pic_converter(QImage _img);
    QImage start(QWidget *parent, int dif,bool isBlackandwhite);
private:
    int getdif(QRgb tar,int r,int g,int b);
    QImage img;
    //int dif;
};

#endif // PIC_CONVERTER_H
