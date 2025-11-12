#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include<QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QPushButton *openBtn = new QPushButton("Open", this);
    QPushButton *saveBtn = new QPushButton("Save", this);
    imageLabel = new QLabel("No image", this);
    imageLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(openBtn);
    layout->addWidget(saveBtn);
    layout->addWidget(imageLabel);


    setCentralWidget(central);

    connect(openBtn, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveFile);

}

void MainWindow::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, "Choose PNM file", "",  "PNM Files (*.pbm *.pgm *.ppm)");
    if (fileName.isEmpty()) return;

    std::ifstream file(fileName.toStdString());
    if(!file.is_open()){
        QMessageBox::warning(this, "Error", "Cannot open the file");
        return;
    }

    std::string header;
    file >> header;
    file.close();

    if(header == "P1"){
        PBMImage img;
        if(img.load(fileName.toStdString())){
            QImage qimg(img.getWidth(), img.getHeight(), QImage::Format_Grayscale8);
            for(int y=0; y < img.getHeight(); ++y){
                for(int x = 0; x < img.getWidth(); ++x){
                    bool pixel = img.getPixel(x,y);
                    int value = pixel ? 0 : 255;
                    qimg.setPixel(x, y, qRgb(value, value, value));
                }
            }
            imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(400,400, Qt::KeepAspectRatio));
        }
    }
    else if(header == "P3"){
        PPMImage img;
        if(img.load(fileName.toStdString())){
            QImage qimg(img.getWidth(), img.getHeight(), QImage::Format_RGB888);
            for(int y = 0; y < img.getHeight(); ++y){
                for(int x = 0 ; x < img.getWidth(); ++x){
                    Pixel p = img.getPixel(x,y);
                    qimg.setPixel(x, y, qRgb(p.r, p.g, p.b));
                }
            }
            imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(400,400,Qt::KeepAspectRatio));
        }
    }
    else if(header == "P2"){
        PGMImage img;
        if (img.load(fileName.toStdString())) {
            QImage qimg(img.getWidth(), img.getHeight(), QImage::Format_Grayscale8);
            for (int y = 0; y < img.getHeight(); ++y) {
                for (int x = 0; x < img.getWidth(); ++x) {
                    unsigned char value = img.getPixel(x, y);
                    qimg.setPixel(x, y, qRgb(value, value, value));
                }
            }
            imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(400, 400, Qt::KeepAspectRatio));
        }
    }
    else {
         QMessageBox::warning(this, "Error", "Unsupported PNM format.");
    }

}
void MainWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save PNM file", "", "PNM Files (*.pbm *.pgm *.ppm)");
    if (fileName.isEmpty()) return;

    // tutaj możesz np. dodać zapis aktualnie załadowanego obrazu
    // np. zapamiętać w MainWindow typ i wskaźnik do obiektu obrazu (PBM/PGM/PPM)
    QMessageBox::information(this, "Info", "Saving not yet implemented.");
}
MainWindow::~MainWindow()
{
    delete ui;
}
