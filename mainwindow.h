#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTimer>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct mypcb
{
    int id;
    QString name;
    int time;
    int vip;
    QString status;
    mypcb(int i,QString n,int t,int v,QString s)
    {
        id=i;
        name=n;
        time=t;
        vip=v;
        status=s;
    }
    mypcb()
    {

    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);    //设置回车响应函数

private slots:
    void refresh();
    void refresh_2();

    void on_btnAddProcess_clicked();
    void on_btnAddProcess_2_clicked();

    void on_btnInterrupt_clicked();
    void on_btnInterrupt_2_clicked();

    void on_btnWaiting_clicked();
    void on_btnWaiting_2_clicked();
    void on_btnDeWaiting_clicked();
    void on_btnDeWaiting_2_clicked();

    void on_btnSuspend_clicked();
    void on_btnSuspend_2_clicked();
    void on_btnDeSuspend_clicked();
    void on_btnDeSuspend_2_clicked();

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::MainWindow *ui;

    int pcbID=0;
    QList<mypcb> pcb_list;
    QList<mypcb> pcb_ready;
    QList<mypcb> pcb_waiting;
    QList<mypcb> pcb_suspend;
    mypcb pcb_running;

    int timeSlice;//时间片大小
    int counter=0;

    QTimer *timer;//计时器

    bool checkName(QString name);
    void sortPCB(QList<mypcb> &list,int length);
    void removePCB(QList<mypcb> &list,mypcb aPcb);

    void addToList(QList<mypcb> list,QListWidget* listWidget);
    void addToList_2(QList<mypcb> list,QListWidget* listWidget);

    void addPCB(mypcb aPcb,QListWidget* listWidget);
    void addPCB_2(mypcb aPcb,QListWidget* listWidget);

    void refreshStatus(QString name,QString status);
    void refreshStatus_2(QString name,QString status);
};
#endif // MAINWINDOW_H
