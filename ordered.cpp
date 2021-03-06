#include "ordered.h"
#include "ui_ordered.h"
#include"global.h"
#include<QDebug>
#include <QSqlQuery>
#include<QMessageBox>
ordered::ordered(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ordered)
{
    ui->setupUi(this);
    model6=new QSqlQueryModel(this);
    model6->setQuery(QString("select info_seat.Fid,info_seat.Seatid,info_seat.Aircraftid,info_seat.sdate,info_flight.Fstart,info_flight.Fend,info_flight.AirportS, info_flight.AirportE,info_flight.Fstarttime,info_flight.Fendtime from info_seat,info_flight where id = '%1' AND info_seat.Fid=info_flight.Fid").arg(userinfo));
    ui->tableView->setModel(model6);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior ( QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(46,46,46),stop:1 rgb(66,66,66));color: rgb(210,210,210);;padding-left: 4px;border: 1px solid #383838;}"); //设置表头背景色
    ui->tableView->setAlternatingRowColors(true);//使用交替行颜色
    ui->tableView->setFocusPolicy(Qt::NoFocus); //去除选中虚线框
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalHeader()->resizeSection(4,50);
    ui->tableView->horizontalHeader()->resizeSection(5,50);
    ui->tableView->horizontalHeader()->resizeSection(6,100);
    ui->tableView->horizontalHeader()->resizeSection(7,100);

}

ordered::~ordered()
{
    delete ui;
}

void ordered::on_pushButton_clicked()
{

    int show_curRow = ui->tableView->currentIndex().row();
    if(model6->index(show_curRow,0).data().toString()=="")
    {
        QMessageBox::information(NULL, QString("错啦"), QString("您还没买过票"));

    }
    else
    {
     QMessageBox::StandardButton reply;

     reply = QMessageBox::question(this, tr("确认退票？"),
                                  QString(tr("尊敬的客户")+userinfo+tr("\n您确认退掉您")+model6->index(show_curRow,3).data().toString()+tr("日\n从")+model6->index(show_curRow,4).data().toString()+tr("到")+model6->index(show_curRow,5).data().toString()+tr("的航班")+model6->index(show_curRow,2).data().toString()+tr("\n您的座位号为")+model6->index(show_curRow,0).data().toString()+tr(".\n如有疑问请致电24小时人工客服热线：95530,vip客户专属服务热线：96300，中国东方航空祝您旅途愉快！！")),
                                   QMessageBox::Yes | QMessageBox::Cancel);
     if (reply == QMessageBox::Yes)
         {
         QSqlQuery query17;
         query17.exec(QString("update info_seat set id = '' where Fid='%1' and Seatid='%2' and Aircraftid='%3' ").arg(model6->index(show_curRow,0).data().toString()).arg(model6->index(show_curRow,1).data().toString()).arg(model6->index(show_curRow,2).data().toString()));
         model6->setQuery(QString("select info_seat.Fid,info_seat.Seatid,info_seat.Aircraftid,info_seat.sdate,info_flight.Fstart,info_flight.Fend,info_flight.AirportS, info_flight.AirportE,info_flight.Fstarttime,info_flight.Fendtime from info_seat,info_flight where id = '%1' AND info_seat.Fid=info_flight.Fid").arg(userinfo));

     }
    }
}
void ordered::on_pushButton_2_clicked()
{

    close();
}
