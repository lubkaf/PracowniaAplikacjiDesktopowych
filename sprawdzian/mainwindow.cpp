#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->txtKurs->setText("Kurs: " + QString::number(kurs));
    ustawIkone("../../usd.png");
    ui->txtWynik->setText("Wynik: 0 USD");
    this->setWindowTitle("Kalkulator Walut, Numer: 11111111111");
    ui->radioUsd->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ustawIkone(QString sciezka){
    QPixmap pix(sciezka);
    if(!pix.isNull()) {
        int w = ui->icnWaluta->width();
        int h = ui->icnWaluta->height();
        ui->icnWaluta->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
void MainWindow::on_radioUsd_clicked()
{
    walutaWybor = 1;
    ikonaWybor = "../../usd.png";
    kurs = 4.10;
}


void MainWindow::on_radioEur_clicked()
{
    walutaWybor = 2;
    ikonaWybor = "../../euro.png";
    kurs = 4.50;
}




void MainWindow::on_radioGbp_clicked()
{
    walutaWybor = 3;
    ikonaWybor = "../../gbp.png";
    kurs = 5.20;
}




void MainWindow::on_btnPrzelicz_clicked()
{
    bool ok;
    QString text = ui->txtWaluta->text();
    wpisanaWaluta = text.toDouble(&ok);

    if(text.isEmpty() || !ok){
        ui->txtWynik->setText("Błąd: wpisz wartość liczbową");
        return;
    }
    switch(walutaWybor){
    case 1:

        wynik = wpisanaWaluta / kurs;
        ui->txtWynik->setText("Wynik:" + QString::number(wynik) + " USD");
        ui->txtKurs->setText("Kurs:" + QString::number(kurs));
        ustawIkone(ikonaWybor);
        return;
    case 2:
        wynik = wpisanaWaluta / kurs;
        ui->txtWynik->setText("Wynik:" + QString::number(wynik) + " EUR");
        ui->txtKurs->setText("Kurs:" + QString::number(kurs));
        ustawIkone(ikonaWybor);

        return;
    case 3:
        wynik = wpisanaWaluta / kurs;
        ui->txtWynik->setText("Wynik:" + QString::number(wynik) + " GBP");
        ui->txtKurs->setText("Kurs:" + QString::number(kurs));
        ustawIkone(ikonaWybor);

        return;
    }
}


void MainWindow::on_btnExit_clicked()
{
    this->close();
}

