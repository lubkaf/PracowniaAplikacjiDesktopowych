#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include "PNMClasses.h"


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
    void openFile();
    void saveFile();
private:
    QLabel *imageLabel;
    QString currentFormat;
    QImage currentImage;

    void showImage();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
