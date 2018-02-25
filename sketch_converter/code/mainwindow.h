#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QProcess>
#include <QMainWindow>
#include<QPixmap>
#include<QImage>
#include<QQueue>
#include<QString>
namespace Ui {
class MainWindow;
}

struct Status_info_node;
class Status_info_queue;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    void status_update(QString _str);
    void show_image(QImage img);
    //QImage* find_border(QImage* origin);
private slots:
    void on_pushButton_clicked();

    bool on_button_savepic_clicked();

    bool on_button_openpic_clicked();

    void on_button_start_clicked();

    bool on_button_show_oripic_clicked();

    void change_pic_to_fixed();
    void on_action_changeStyle_triggered();

    void on_action_changeto_psblack_toggled(bool checked);

    void on_action_changeto_lightgrey_toggled(bool checked);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    void initStyle();
    Ui::MainWindow *ui;
    QProcess process;
    QImage ori_image;
    QImage new_image;
    QImage cur_image;
    Status_info_queue* Status_info;
};

struct Status_info_node
{
    QString str;
    Status_info_node* next;
};

class Status_info_queue//只保存max_size行命令
{
public:
    Status_info_queue(int _max_size)
    {
        size = 0;
        max_size = _max_size;
        head = new Status_info_node[_max_size];
        for(int i = 0;i<max_size-1;i++)
        {
            head[i].next = &head[i+1];
        }
        head[max_size-1].next = head;
    }
    void add(QString _str)
    {
        if(size<max_size) size++;
        head->str = _str;
        head = head->next;
    }
    QString getstr()
    {
        QString res;
        if(size == 0) return "";
        Status_info_node* temp_head = head;
        if(size<max_size) while(temp_head->str.isEmpty()) temp_head = temp_head->next;
        else
        {
            res+=(temp_head->str+'\n');
            temp_head = temp_head->next;
        }
        while(temp_head!=head)
        {
            res+=(temp_head->str+"\n");
            temp_head = temp_head->next;
        }
        return res;
    }

private:
    int size;
    int max_size;
    Status_info_node* head;
};

/*class status_info_vector
{
public:

private:

};*/

#endif // MAINWINDOW_H
