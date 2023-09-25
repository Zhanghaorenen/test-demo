#include "widget.h"
#include "ui_widget.h"
#include<QTimer>
#include<QElapsedTimer>
#include<QTime>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_timer=new QTimer(this);
    m_counter=new QElapsedTimer();
    m_timer->stop();
    m_timer->setTimerType(Qt::CoarseTimer);
    ui->coarsetime->setChecked(true);
    connect(m_timer,&QTimer::timeout,this,&Widget::do_timer_timeout);
}

Widget::~Widget()
{
    delete m_counter;
    delete ui;
}

void Widget::do_timer_timeout()
{
    QApplication::beep();
    QTime curTime=QTime::currentTime();
    ui->lcdhour->display(curTime.hour());
    ui->lcdmin->display(curTime.minute());
    ui->lcdsec->display(curTime.second());

    if(m_timer->isSingleShot()){
        int tmMsecc=m_counter->elapsed();
        QString str=QString("流逝的时间: %1 毫秒").arg(tmMsecc);
        ui->label_2->setText(str);

        ui->btnstart->setEnabled(true);
        ui->btnoneshot->setEnabled(true);
        ui->btnstop->setEnabled(false);

    }

}


void Widget::on_btnstart_clicked()
{
    m_timer->setInterval(ui->spinBox->value());
    if(ui->rbtncontinue->isChecked()){
        m_timer->setSingleShot(false);
    }
    else{
        m_timer->setSingleShot(true);
    }
    if(ui->precioustime){
        m_timer->setTimerType(Qt::PreciseTimer);
    }
    else if(ui->coarsetime){
        m_timer->setTimerType(Qt::CoarseTimer);
    }
    else
        m_timer->setTimerType(Qt::VeryCoarseTimer);

    m_timer->start();
    m_counter->start();
    ui->btnstart->setEnabled(false);
    ui->btnoneshot->setEnabled(false);
    ui->btnstop->setEnabled(true);


}


void Widget::on_btnstop_clicked()
{
    m_timer->stop();
    int tmMsecc=m_counter->elapsed();
    int ms=tmMsecc%1000;
    int s=tmMsecc/1000;
    QString str=QString("流逝的时间: %1秒。 %2 毫秒").arg(s).arg(ms);
    ui->label_2->setText(str);
    ui->btnstart->setEnabled(true);
    ui->btnoneshot->setEnabled(true);
    ui->btnstop->setEnabled(true);

}


void Widget::on_btnoneshot_clicked()
{
    int intv=ui->spinBox->value();

    QTimer ::singleShot(intv,Qt::PreciseTimer,this,&Widget::do_timer_timeout);

    m_counter->start();
    ui->btnstart->setEnabled(false);
    ui->btnoneshot->setEnabled(false);
    ui->btnstop->setEnabled(true);

}

