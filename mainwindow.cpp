 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowIcon(QIcon(":/images/bitbug_favicon.ico"));
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("user");
    model->select();
    // 设置编辑策略
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(46,46,46),stop:1 rgb(66,66,66));color: rgb(210,210,210);;padding-left: 4px;border: 1px solid #383838;}"); //设置表头背景色
    ui->tableView->horizontalHeader()->resizeSection(3,180);
    ui->tableView->horizontalHeader()->resizeSection(1,100);
    ui->tableView->horizontalHeader()->resizeSection(2,100);
    ui->tableView->horizontalHeader()->resizeSection(5,130);
    ui->tableView->horizontalHeader()->resizeSection(7,130);
    model2 = new QSqlTableModel(this);
    model2->setTable("info_flight");
    model2->select();
    model2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView_2->setModel(model2);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(46,46,46),stop:1 rgb(66,66,66));color: rgb(210,210,210);;padding-left: 4px;border: 1px solid #383838;}"); //设置表头背景色
    ui->tableView_2->setAlternatingRowColors(true); //使用交替行颜色
    ui->tableView_2->horizontalHeader()->resizeSection(0,30);
    ui->tableView_2->horizontalHeader()->resizeSection(1,70);
    ui->tableView_2->horizontalHeader()->resizeSection(2,50);
    ui->tableView_2->horizontalHeader()->resizeSection(3,50);
    ui->tableView_2->horizontalHeader()->resizeSection(4,90);
    ui->tableView_2->horizontalHeader()->resizeSection(5,90);
    ui->tableView_2->horizontalHeader()->resizeSection(6,90);
    ui->tableView_2->horizontalHeader()->resizeSection(7,50);
    ui->tableView_2->horizontalHeader()->resizeSection(8,90);
    ui->tableView_2->horizontalHeader()->resizeSection(9,70);
    model3 = new QSqlTableModel(this);
    model3->setTable("info_seat");
    model3->select();
    model3->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView_3->setModel(model3);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //设置表格列宽度自适应
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView_3->resizeColumnsToContents();
    ui->tableView_3->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(46,46,46),stop:1 rgb(66,66,66));color: rgb(210,210,210);;padding-left: 4px;border: 1px solid #383838;}"); //设置表头背景色
    ui->tableView_3->setAlternatingRowColors(true); //使用交替行颜色
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 提交修改按钮
void MainWindow::on_pushButton_clicked()
{
    // 开始事务操作
    model->database().transaction();
    if (model->submitAll()) {
        if(model->database().commit()) // 提交
            QMessageBox::information(this, tr("tableModel"),
                                     tr("数据修改成功！"));
    } else {
        model->database().rollback(); // 回滚
        QMessageBox::warning(this, tr("tableModel"),
                             tr("数据库错误: %1").arg(model->lastError().text()),
                             QMessageBox::Ok);
    }
}

// 撤销修改按钮
void MainWindow::on_pushButton_2_clicked()
{
    model->revertAll();
}

// 查询按钮，进行筛选
void MainWindow::on_pushButton_5_clicked()
{
    QString name = ui->lineEdit->text();

    // 根据姓名进行筛选，一定要使用单引号
    model->setFilter(QString("username = '%1'").arg(name));
    model->select();
}

// 显示全表按钮
void MainWindow::on_pushButton_6_clicked()
{
    model->setTable("user");
    model->select();
}

// 按id升序排列按钮
void MainWindow::on_pushButton_7_clicked()
{
    //id字段，即第0列，升序排列
    model->setSort(0, Qt::AscendingOrder);
    model->select();

}

// 按id降序排列按钮
void MainWindow::on_pushButton_8_clicked()
{
    model->setSort(0, Qt::DescendingOrder);
    model->select();

}
// 删除选中行按钮
void MainWindow::on_pushButton_4_clicked()
{
    // 获取选中的行
    int curRow = ui->tableView->currentIndex().row();

    // 删除该行
    model->removeRow(curRow);
    int ok = QMessageBox::warning(this,tr("删除当前行!"),
                                  tr("你确定删除当前行吗？"), QMessageBox::Yes, QMessageBox::No);
    if(ok == QMessageBox::No)
    { // 如果不删除，则撤销
        model->revertAll();
    } else { // 否则提交，在数据库中删除该行
        model->submitAll();
    }

}
// 添加记录按钮
void MainWindow::on_pushButton_3_clicked()
{
    // 获得表的行数
    int rowNum = model->rowCount();
    int id = 10;

    // 添加一行
    model->insertRow(rowNum);
    model->setData(model->index(0, 0), id);

    // 可以直接提交
    //model->submitAll();
}

void MainWindow::on_pushButton_9_clicked()
{
    // 开始事务操作
    model2->database().transaction();
    if (model2->submitAll()) {
        if(model2->database().commit()) // 提交
            QMessageBox::information(this, tr("tableModel"),
                                     tr("数据修改成功！"));
    } else {
        model2->database().rollback(); // 回滚
        QMessageBox::warning(this, tr("tableModel"),
                             tr("数据库错误: %1").arg(model2->lastError().text()),
                             QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    model2->revertAll();
}

void MainWindow::on_pushButton_11_clicked()
{
    // 获得表的行数
    int rowNum = model2->rowCount();
    int id = 10;

    // 添加一行
    model2->insertRow(rowNum);
    model2->setData(model2->index(0, 0), id);

    // 可以直接提交
    //model->submitAll();
}

void MainWindow::on_pushButton_12_clicked()
{
    // 获取选中的行
    int curRow = ui->tableView_2->currentIndex().row();

    // 删除该行
    model2->removeRow(curRow);
    int ok = QMessageBox::warning(this,tr("删除当前行!"),
                                  tr("你确定删除当前行吗？"), QMessageBox::Yes, QMessageBox::No);
    if(ok == QMessageBox::No)
    { // 如果不删除，则撤销
        model2->revertAll();
    } else { // 否则提交，在数据库中删除该行
        model2->submitAll();
    }

}

void MainWindow::on_pushButton_13_clicked()
{
    model2->setSort(0, Qt::AscendingOrder);
    model2->select();
}

void MainWindow::on_pushButton_14_clicked()
{
    model2->setSort(0, Qt::DescendingOrder);
    model2->select();
}

void MainWindow::on_pushButton_15_clicked()
{
    QString Fids = ui->lineEdit_2->text();

    // 根据姓名进行筛选，一定要使用单引号
    model2->setFilter(QString("Fid = '%1'").arg(Fids));
    model2->select();
}

void MainWindow::on_pushButton_16_clicked()
{
    model2->setTable("info_flight");
    model2->select();
}

void MainWindow::on_pushButton_17_clicked()
{
    // 开始事务操作
    model3->database().transaction();
    if (model3->submitAll()) {
        if(model3->database().commit()) // 提交
            QMessageBox::information(this, tr("tableModel"),
                                     tr("数据修改成功！"));
    } else {
        model3->database().rollback(); // 回滚
        QMessageBox::warning(this, tr("tableModel"),
                             tr("数据库错误: %1").arg(model3->lastError().text()),
                             QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_18_clicked()
{
    model3->revertAll();
}

void MainWindow::on_pushButton_19_clicked()
{
    // 获得表的行数
    int rowNum = model3->rowCount();
    int id = 10;

    // 添加一行
    model3->insertRow(rowNum);
    model3->setData(model3->index(0, 0), id);

    // 可以直接提交
    //model->submitAll();
}

void MainWindow::on_pushButton_20_clicked()
{
    // 获取选中的行
    int curRow = ui->tableView_3->currentIndex().row();

    // 删除该行
    model3->removeRow(curRow);
    int ok = QMessageBox::warning(this,tr("删除当前行!"),
                                  tr("你确定删除当前行吗？"), QMessageBox::Yes, QMessageBox::No);
    if(ok == QMessageBox::No)
    { // 如果不删除，则撤销
        model3->revertAll();
    } else { // 否则提交，在数据库中删除该行
        model3->submitAll();
    }
}

void MainWindow::on_pushButton_21_clicked()
{
    model3->setSort(1, Qt::AscendingOrder);
    model3->select();
}

void MainWindow::on_pushButton_22_clicked()
{
    model3->setSort(1, Qt::DescendingOrder);
    model3->select();
}

void MainWindow::on_pushButton_23_clicked()
{
    QString Fids3 = ui->lineEdit_3->text();

    // 根据姓名进行筛选，一定要使用单引号
    model3->setFilter(QString("Fid = '%1'").arg(Fids3));
    model3->select();
}

void MainWindow::on_pushButton_24_clicked()
{
    model3->setTable("info_seat");
    model3->select();
}
