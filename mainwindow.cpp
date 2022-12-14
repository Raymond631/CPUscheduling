#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);//计时器
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(refresh()));
    timer->start(1000);//1秒刷新一次

    ui->pcb_name->setFocus();
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if(ui->tabWidget->currentIndex()==0)
        {
            on_btnAddProcess_clicked();
            ui->pcb_name->setFocus();
        }
        else if(ui->tabWidget->currentIndex()==1)
        {
            on_btnAddProcess_2_clicked();
            ui->pcb_name_2->setFocus();
        }
        else
        {
            QMessageBox::warning(NULL,"","ERROR1!");
        }
    }
}
bool MainWindow::checkName(QString name)
{
    for(mypcb p : pcb_list)
    {
        if(p.name==name)
        {
            return false;
        }
    }
    return true;
}
void MainWindow::sortPCB(QList<mypcb> &list, int length)
{
    for(int i=0;i<length-1;i++)
    {
        for(int j=0;j<length-i-1;j++)
        {
            if(list.at(j).vip<list.at(j+1).vip)
            {
                continue;
            }
            else if(list.at(j).vip>list.at(j+1).vip)
            {
                mypcb temp=list.at(j);
                list.replace(j,list.at(j+1));
                list.replace(j+1,temp);
            }
            else
            {
                if(list.at(j).id<list.at(j+1).id)
                {
                    continue;
                }
                else if(list.at(j).id>list.at(j+1).id)
                {
                    mypcb temp=list.at(j);
                    list.replace(j,list.at(j+1));
                    list.replace(j+1,temp);
                }
                else
                {
                    QMessageBox::warning(NULL,"","ERROR2!");
                }
            }
        }
    }
}
void MainWindow::removePCB(QList<mypcb> &list, mypcb aPcb)
{
    int i=0;
    for(mypcb p:list)
    {
        if(p.id==aPcb.id)
        {
            list.removeAt(i);
        }
        i++;
    }
}

void MainWindow::on_btnAddProcess_clicked()
{
    if(pcb_list.count()>=6)
    {
        QMessageBox::warning(NULL,"","超出规定道数!");
    }
    else
    {
        QString name=ui->pcb_name->text();
        int time=ui->pcb_time->text().toInt();
        int vip=ui->pcb_vip->text().toInt();

        if(name==NULL)
        {
            QMessageBox::warning(NULL,"","进程信息不能为空!");
            ui->pcb_name->setFocus();
        }
        else
        {
            if(checkName(name))
            {
                mypcb aPCB(pcbID,name,time,vip,"ready");
                pcbID++;

                pcb_list.append(aPCB);
                pcb_ready.append(aPCB);

                sortPCB(pcb_ready,pcb_ready.length());//优先级排序

                addToList(pcb_list,ui->PCBlist);
                addToList(pcb_ready,ui->ReadyPCB);

                ui->pcb_name->clear();
                ui->pcb_time->clear();
                ui->pcb_vip->clear();
            }
            else
            {
                QMessageBox::warning(NULL,"","进程名已存在!");
                ui->pcb_name->clear();
            }
        }
    }
}
void MainWindow::on_btnAddProcess_2_clicked()
{
    if(pcb_list.count()>=6)
    {
        QMessageBox::warning(NULL,"","超出规定道数!");
    }
    else
    {
        timeSlice=ui->timeSlice->text().toInt();
        QString name=ui->pcb_name_2->text();
        int time=ui->pcb_time_2->text().toInt();

        if(name==NULL)
        {
            QMessageBox::warning(NULL,"","进程信息不能为空!");
            ui->pcb_name_2->setFocus();
        }
        else
        {
            if(checkName(name))
            {
                mypcb aPCB(pcbID,name,time,0,"ready");//优先级都为0
                pcbID++;

                pcb_list.append(aPCB);
                pcb_ready.append(aPCB);

                addToList_2(pcb_list,ui->PCBlist_2);
                addToList_2(pcb_ready,ui->ReadyPCB_2);

                ui->pcb_name_2->clear();
                ui->pcb_time_2->clear();
            }
            else
            {
                QMessageBox::warning(NULL,"","进程名已存在!");
                ui->pcb_name_2->clear();
            }
        }
    }
}

void MainWindow::refresh()
{
    if(ui->RunningPCB->count()==0&&ui->ReadyPCB->count()!=0)
    {
        //添加进running
        pcb_running=pcb_ready.at(0);
        refreshStatus(pcb_running.name,"running");
        addPCB(pcb_running,ui->RunningPCB);

        //从ready移除
        pcb_ready.removeFirst();
        addToList(pcb_ready,ui->ReadyPCB);
    }
    else if(ui->RunningPCB->count()==1)
    {
        if(pcb_running.time==0)
        {
            //从running移除
            ui->RunningPCB->clear();
            //从pcbList移除
            removePCB(pcb_list,pcb_running);
            addToList(pcb_list,ui->PCBlist);
        }
        else
        {
            pcb_running.time--;
            //刷新running序列
            ui->RunningPCB->clear();
            addPCB(pcb_running,ui->RunningPCB);
        }
    }
}
void MainWindow::refresh_2()
{
    if(ui->RunningPCB_2->count()==0&&ui->ReadyPCB_2->count()!=0)
    {
        counter=0;//计数器归零
        //添加进running
        pcb_running=pcb_ready.at(0);
        refreshStatus_2(pcb_running.name,"running");
        addPCB_2(pcb_running,ui->RunningPCB_2);

        //从ready移除
        pcb_ready.removeFirst();
        addToList_2(pcb_ready,ui->ReadyPCB_2);
    }
    else if(ui->RunningPCB_2->count()==1)
    {
        if(pcb_running.time==0)
        {
            //从running移除
            ui->RunningPCB_2->clear();
            //从pcbList移除
            removePCB(pcb_list,pcb_running);
            addToList_2(pcb_list,ui->PCBlist_2);
        }
        else
        {
            counter++;
            pcb_running.time--;
            //刷新running序列
            ui->RunningPCB_2->clear();
            addPCB_2(pcb_running,ui->RunningPCB_2);

            if(counter%timeSlice==0)//时间片轮转
            {
                on_btnInterrupt_2_clicked();
            }
        }
    }
}

void MainWindow::addToList(QList<mypcb> list, QListWidget *listWidget)
{
    listWidget->clear();
    for(mypcb aPcb:list)
    {
        addPCB(aPcb,listWidget);
    }
}
void MainWindow::addToList_2(QList<mypcb> list, QListWidget *listWidget)
{
    listWidget->clear();
    for(mypcb aPcb:list)
    {
        addPCB_2(aPcb,listWidget);
    }
}

void MainWindow::addPCB(mypcb aPcb, QListWidget *listWidget)
{
    QLabel* n=new QLabel("name："+aPcb.name);
    n->setObjectName(QStringLiteral("name"));//方便勾选时 查找选了哪个pcb
    QLabel* t=new QLabel("time："+QString::number(aPcb.time));
    QLabel* v=new QLabel("priority："+QString::number(aPcb.vip));
    QLabel* s=new QLabel("status："+aPcb.status);
    QCheckBox* c=new QCheckBox();
    c->setObjectName(QStringLiteral("checkbox"));


    QGridLayout *layout=new QGridLayout();
    layout->addWidget(n,0,0,1,1);
    layout->addWidget(t,0,1,1,1);
    layout->addWidget(v,1,0,1,1);
    layout->addWidget(s,1,1,1,1);
    layout->addWidget(c,0,2,2,1,Qt::AlignCenter);

    QWidget* w=new QWidget();
    w->setLayout(layout);
    w->setStyleSheet("QWidget{font:12pt;font-family:Times New Roman}");

    QListWidgetItem *item=new QListWidgetItem();
    listWidget->addItem(item);
    listWidget->setItemWidget(item,w);
}
void MainWindow::addPCB_2(mypcb aPcb, QListWidget *listWidget)
{
    QLabel* n=new QLabel("name："+aPcb.name);
    n->setObjectName(QStringLiteral("name"));//方便勾选时 查找选了哪个pcb
    QLabel* t=new QLabel("time："+QString::number(aPcb.time));
    QLabel* s=new QLabel("status："+aPcb.status);
    QCheckBox* c=new QCheckBox();
    c->setObjectName(QStringLiteral("checkbox"));


    QGridLayout *layout=new QGridLayout();
    layout->addWidget(n,0,0,1,1);
    layout->addWidget(t,0,1,1,1);
    layout->addWidget(s,1,0,1,1);
    layout->addWidget(c,0,2,2,1,Qt::AlignCenter);

    QWidget* w=new QWidget();
    w->setLayout(layout);
    w->setStyleSheet("QWidget{font:12pt;font-family:Times New Roman}");

    QListWidgetItem *item=new QListWidgetItem();
    listWidget->addItem(item);
    listWidget->setItemWidget(item,w);
}

void MainWindow::refreshStatus(QString name, QString status)
{
    for(mypcb &p : pcb_list)
    {
        if(p.name==name)
        {
            p.status=status;
        }
    }
    addToList(pcb_list,ui->PCBlist);

    if(status=="running")
    {
        pcb_running.status=status;
    }
    else if(status=="ready")
    {
        for(mypcb &p : pcb_ready)
        {
            if(p.name==name)
            {
                p.status=status;
            }
        }
    }
    else if(status=="waiting")
    {
        for(mypcb &p : pcb_waiting)
        {
            if(p.name==name)
            {
                p.status=status;
            }
        }
    }
    else if(status=="suspend")
    {
        for(mypcb &p : pcb_suspend)
        {
            if(p.name==name)
            {
                p.status=status;
            }
        }
    }
}
void MainWindow::refreshStatus_2(QString name, QString status)
{
    for(mypcb &p : pcb_list)
    {
        if(p.name==name)
        {
            p.status=status;
        }
    }
    addToList_2(pcb_list,ui->PCBlist_2);

    if(status=="running")
    {
        pcb_running.status=status;
    }
    else if(status=="ready")
    {
        for(mypcb &p : pcb_ready)
        {
            if(p.name==name)
            {
                p.status=status;
            }
        }
    }
    else if(status=="waiting")
    {
        for(mypcb &p : pcb_waiting)
        {
            if(p.name==name)
            {
                p.status=status;
            }
        }
    }
    else if(status=="suspend")
    {
        for(mypcb &p : pcb_suspend)
        {
            if(p.name==name)
            {
                p.status=status;
            }
        }
    }

}

void MainWindow::on_btnInterrupt_clicked()
{
    timer->stop();

    ui->RunningPCB->clear();

    pcb_ready.append(pcb_running);
    refreshStatus(pcb_running.name,"ready");
    sortPCB(pcb_ready,pcb_ready.length());
    addToList(pcb_ready,ui->ReadyPCB);

    timer->start(1000);
}
void MainWindow::on_btnInterrupt_2_clicked()
{
    timer->stop();

    ui->RunningPCB_2->clear();

    pcb_ready.append(pcb_running);
    refreshStatus_2(pcb_running.name,"ready");
    addToList_2(pcb_ready,ui->ReadyPCB_2);

    timer->start(1000);
}

void MainWindow::on_btnWaiting_clicked()
{
    timer->stop();

    ui->RunningPCB->clear();
    pcb_waiting.append(pcb_running);
    refreshStatus(pcb_running.name,"waiting");
    addToList(pcb_waiting,ui->WaitingPCB);

    timer->start(1000);
}
void MainWindow::on_btnWaiting_2_clicked()
{
    timer->stop();

    ui->RunningPCB_2->clear();
    pcb_waiting.append(pcb_running);
    refreshStatus_2(pcb_running.name,"waiting");
    addToList_2(pcb_waiting,ui->WaitingPCB_2);

    timer->start(1000);
}

void MainWindow::on_btnDeWaiting_clicked()
{
    timer->stop();

    for(int i=0;i<ui->WaitingPCB->count();i++)
    {
        QListWidgetItem* item=ui->WaitingPCB->item(i);
        QCheckBox* box=ui->WaitingPCB->itemWidget(item)->findChild<QCheckBox*>("checkbox");
        if(box->isChecked())
        {
            QString name=ui->WaitingPCB->itemWidget(item)->findChild<QLabel*>("name")->text().mid(5);
            for(mypcb m:pcb_waiting)
            {
                if(m.name==name)
                {
                    removePCB(pcb_waiting,m);
                    pcb_ready.append(m);
                    refreshStatus(name,"ready");
                }
            }
        }
    }
    sortPCB(pcb_ready,pcb_ready.length());

    addToList(pcb_waiting,ui->WaitingPCB);
    addToList(pcb_ready,ui->ReadyPCB);

    timer->start(1000);
}
void MainWindow::on_btnDeWaiting_2_clicked()
{
    timer->stop();

    for(int i=0;i<ui->WaitingPCB_2->count();i++)
    {
        QListWidgetItem* item=ui->WaitingPCB_2->item(i);
        QCheckBox* box=ui->WaitingPCB_2->itemWidget(item)->findChild<QCheckBox*>("checkbox");
        if(box->isChecked())
        {
            QString name=ui->WaitingPCB_2->itemWidget(item)->findChild<QLabel*>("name")->text().mid(5);
            for(mypcb m:pcb_waiting)
            {
                if(m.name==name)
                {
                    removePCB(pcb_waiting,m);
                    pcb_ready.append(m);
                    refreshStatus_2(name,"ready");
                }
            }
        }
    }
    addToList_2(pcb_waiting,ui->WaitingPCB_2);
    addToList_2(pcb_ready,ui->ReadyPCB_2);

    timer->start(1000);
}

void MainWindow::on_btnSuspend_clicked()
{
    timer->stop();

    for(int i=0;i<ui->PCBlist->count();i++)
    {
        QListWidgetItem* item=ui->PCBlist->item(i);
        QCheckBox* box=ui->PCBlist->itemWidget(item)->findChild<QCheckBox*>("checkbox");
        QString name=ui->PCBlist->itemWidget(item)->findChild<QLabel*>("name")->text().mid(5);
        if(box->isChecked())
        {
            for(mypcb m:pcb_list)
            {
                if(m.name==name)
                {
                    QString status=m.status;
                    if(status=="suspend")
                    {
                        continue;
                    }
                    else if(status=="running")
                    {
                        ui->RunningPCB->clear();
                    }
                    else if(status=="ready")
                    {
                        removePCB(pcb_ready,m);
                        addToList(pcb_ready,ui->ReadyPCB);
                    }
                    else if(status=="waiting")
                    {
                        removePCB(pcb_waiting,m);
                        addToList(pcb_waiting,ui->WaitingPCB);
                    }
                    pcb_suspend.append(m);
                }
            }
        }
    }
    for(mypcb p:pcb_suspend)
    {
        refreshStatus(p.name,"suspend");
    }
    addToList(pcb_suspend,ui->SuspendPCB);

    timer->start(1000);
}
void MainWindow::on_btnSuspend_2_clicked()
{
    timer->stop();

    for(int i=0;i<ui->PCBlist_2->count();i++)
    {
        QListWidgetItem* item=ui->PCBlist_2->item(i);
        QCheckBox* box=ui->PCBlist_2->itemWidget(item)->findChild<QCheckBox*>("checkbox");
        QString name=ui->PCBlist_2->itemWidget(item)->findChild<QLabel*>("name")->text().mid(5);
        if(box->isChecked())
        {
            for(mypcb m:pcb_list)
            {
                if(m.name==name)
                {
                    QString status=m.status;
                    if(status=="suspend")
                    {
                        continue;
                    }
                    else if(status=="running")
                    {
                        ui->RunningPCB_2->clear();
                    }
                    else if(status=="ready")
                    {
                        removePCB(pcb_ready,m);
                        addToList_2(pcb_ready,ui->ReadyPCB_2);
                    }
                    else if(status=="waiting")
                    {
                        removePCB(pcb_waiting,m);
                        addToList_2(pcb_waiting,ui->WaitingPCB_2);
                    }
                    pcb_suspend.append(m);
                }
            }
        }
    }
    for(mypcb p:pcb_suspend)
    {
        refreshStatus_2(p.name,"suspend");
    }
    addToList_2(pcb_suspend,ui->SuspendPCB_2);

    timer->start(1000);
}

void MainWindow::on_btnDeSuspend_clicked()
{
    timer->stop();

    for(int i=0;i<ui->SuspendPCB->count();i++)
    {
        QListWidgetItem* item=ui->SuspendPCB->item(i);
        QCheckBox* box=ui->SuspendPCB->itemWidget(item)->findChild<QCheckBox*>("checkbox");
        if(box->isChecked())
        {
            QString name=ui->SuspendPCB->itemWidget(item)->findChild<QLabel*>("name")->text().mid(5);
            for(mypcb m:pcb_list)
            {
                if(m.name==name)
                {
                    removePCB(pcb_suspend,m);
                    pcb_ready.append(m);
                    refreshStatus(name,"ready");
                }
            }
        }
    }
    sortPCB(pcb_ready,pcb_ready.length());

    addToList(pcb_suspend,ui->SuspendPCB);
    addToList(pcb_ready,ui->ReadyPCB);

    timer->start(1000);
}
void MainWindow::on_btnDeSuspend_2_clicked()
{
    timer->stop();

    for(int i=0;i<ui->SuspendPCB_2->count();i++)
    {
        QListWidgetItem* item=ui->SuspendPCB_2->item(i);
        QCheckBox* box=ui->SuspendPCB_2->itemWidget(item)->findChild<QCheckBox*>("checkbox");
        if(box->isChecked())
        {
            QString name=ui->SuspendPCB_2->itemWidget(item)->findChild<QLabel*>("name")->text().mid(5);
            for(mypcb m:pcb_list)
            {
                if(m.name==name)
                {
                    removePCB(pcb_suspend,m);
                    pcb_ready.append(m);
                    refreshStatus_2(name,"ready");
                }
            }
        }
    }
    addToList_2(pcb_suspend,ui->SuspendPCB_2);
    addToList_2(pcb_ready,ui->ReadyPCB_2);

    timer->start(1000);
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    timer->stop();

    ui->PCBlist->clear();
    ui->RunningPCB->clear();
    ui->ReadyPCB->clear();
    ui->WaitingPCB->clear();
    ui->SuspendPCB->clear();

    ui->PCBlist_2->clear();
    ui->RunningPCB_2->clear();
    ui->ReadyPCB_2->clear();
    ui->WaitingPCB_2->clear();
    ui->SuspendPCB_2->clear();

    pcbID=0;
    pcb_list.clear();
    pcb_ready.clear();
    pcb_waiting.clear();
    pcb_suspend.clear();

    counter=0;//时间片计数器归零

    if(index==0)
    {
        timer = new QTimer(this);//计时器
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(refresh()));
        timer->start(1000);//1秒刷新一次
    }
    else if(index==1)
    {
        ui->timeSlice->setText("5");
        timeSlice=5;
        ui->pcb_name_2->setFocus();

        timer = new QTimer(this);//计时器
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(refresh_2()));
        timer->start(1000);//1秒刷新一次
    }
    else
    {
        QMessageBox::warning(NULL,"","ERROR3!");
    }
}

