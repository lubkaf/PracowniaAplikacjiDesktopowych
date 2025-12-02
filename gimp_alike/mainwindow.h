#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QImage>
#include <QAction>
#include <QColor>
#include <QInputDialog>
#include <cmath>
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
    void adjustBrightnessDialog();
    void adjustSaturation();
    void contrastDialog();
    unsigned char** generateLUT(){

        unsigned char** LUT = new unsigned char*[256];
        for (int i = 0; i < 256; ++i) {
            LUT[i] = new unsigned char[201];
        }
        //S to współczynnik kontrolujący siłę zmiany
        const double S = 0.8;
        for (int i = - 100; i <= 100; i++){
            int lutIndex = i + 100;
            double gamma = 1.0 - (static_cast<double>(i) / 100) * S;
            for(int originalValue = 0; originalValue < 256; originalValue++){
                //normalizacja do zakresu od 0.0 do 1.0
                double Original = static_cast<double>(originalValue) / 255;
                double New = std::pow(Original, gamma);
                //powrot do wartosci od 0 do 255
                int newValue = static_cast<int>(std::round(New * 255));
                int clampedValue = qBound(0,newValue,255);
                LUT[originalValue][lutIndex] = static_cast<unsigned char>(clampedValue);

            }
        }
        return LUT;
    };
    void deleteLUT(unsigned char** LUT) {
        if (LUT == nullptr) return;
        for (int i = 0; i < 256; ++i) {
            if (LUT[i] != nullptr) {
                delete[] LUT[i];
            }
        }
        delete[] LUT;
    }
protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void displayImage() const;


    QLabel* imageLabel;
    QScrollArea* scrollArea;
    QImage currentImage;

    PNMImage* currentPNMImage = nullptr;
    QString currentFormat;
public:
    unsigned char** LUT = nullptr;



};

#endif // MAINWINDOW_H
