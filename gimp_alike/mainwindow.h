#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QImage>
#include <QAction>
#include <QColor>
#include <QInputDialog>
#include "pnmclasses.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void openFile();
    void saveFile();

    // Image operations
    void desaturateImage();
    void negativeImage();
    void linearContrast();
    void logContrast();
    void gammaContrast();
    void adjustBrightnessDialog();
    void adjustSaturation();
    void contrastDialog();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void displayImage() const;

    QLabel* imageLabel;
    QScrollArea* scrollArea;
    QImage currentImage;

    PNMImage* currentPNMImage = nullptr;
    QString currentFormat;
};

#endif // MAINWINDOW_H
