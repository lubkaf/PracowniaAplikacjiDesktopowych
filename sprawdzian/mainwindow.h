#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_radioUsd_clicked();

    void on_radioEur_clicked();

    void on_radioGbp_clicked();

    void on_btnPrzelicz_clicked();

    void ustawIkone(QString sciezka);

    void on_btnExit_clicked();

private:
    Ui::MainWindow *ui;
    int walutaWybor = 1;
    QString ikonaWybor;
    double kurs = 4.10;
    double wpisanaWaluta, wynik;

};
#endif // MAINWINDOW_H
