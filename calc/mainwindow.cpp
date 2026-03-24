#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mathoperations.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAdd_clicked()
{
    firstNum = ui->txtFirstNum->text().toDouble();
    secNum = ui->txtSecNum->text().toDouble();

    mathOperations engine;

    auto result = engine.Add(firstNum, secNum);
    ui->txtResult->setText(QString::number(result));

}


void MainWindow::on_btnSubstract_clicked()
{
    firstNum = ui->txtFirstNum->text().toDouble();
    secNum = ui->txtSecNum->text().toDouble();
    mathOperations engine;

    auto result = engine.Substract(firstNum, secNum);
    ui->txtResult->setText(QString::number(result));

}


void MainWindow::on_btnDivide_clicked()
{
    firstNum = ui->txtFirstNum->text().toDouble();
    secNum = ui->txtSecNum->text().toDouble();
    mathOperations engine;

    auto result = engine.Divide(firstNum, secNum);
    if(result.has_value()) ui->txtResult->setText(QString::number(result.value()));
    else ui->txtResult->setText("Nie dziel przez 0");
}


void MainWindow::on_btnMultiply_clicked()
{
    firstNum = ui->txtFirstNum->text().toDouble();
    secNum = ui->txtSecNum->text().toDouble();
    mathOperations engine;

    auto result = engine.Multiply(firstNum, secNum);
    ui->txtResult->setText(QString::number(result));
}

