#ifndef DATACENTER_H
#define DATACENTER_H
#include<QString>
#include<QStringList>

class DataCenter
{

public:
    /*static DataCenter& getInstance(){
        static DataCenter datacenter;
        return datacenter;
    }*/
    DataCenter();
    void filterOutput();
    QStringList curImageList;
    QString curPath;
    QString autoFilterInputPath;
    QString autoFilterOutputPath;
    int curImageIndex;
    int curImageCount;
    double minWHrate;
    double maxWHrate;
    double minWrate;
    double minHrate;
private:

    /*DataCenter(const DataCenter&){
        // 禁止复制
    }
    DataCenter& operator = (const DataCenter& c){
        return c;
    }*/


};

#endif // DATACENTER_H
