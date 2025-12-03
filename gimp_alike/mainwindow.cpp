#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtMath>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);

    // --- Menu File ---
    QMenu* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("&Open", this, &MainWindow::openFile);
    fileMenu->addAction("&Save", this, &MainWindow::saveFile);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, &QWidget::close);

    // --- Menu Operations ---
    QMenu* opsMenu = menuBar()->addMenu("&Operations");
    opsMenu->addAction("&Desaturate", this, &MainWindow::desaturateImage);
    opsMenu->addAction("&Negative", this, &MainWindow::negativeImage);
    opsMenu->addSeparator();
    opsMenu->addAction("&Contrast", this, &MainWindow::contrastDialog);
    opsMenu->addAction("&Brightness", this, &MainWindow::adjustBrightnessDialog);
    opsMenu->addAction("&Saturation", this, &MainWindow::adjustSaturation);

    setWindowTitle("PNM Viewer");
    resize(800,600);
    brightnessLUT = generateBrightnessLUT();
    contrastLUT = generateContrastLUT();
}

MainWindow::~MainWindow() {
    deleteLUT(brightnessLUT);
    delete currentPNMImage;
}
// --- File ---
void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choose PNM file", "", "PNM Files (*.pbm *.pgm *.ppm)");
    if(fileName.isEmpty()) return;

    std::ifstream file(fileName.toStdString(), std::ios::binary);
    if(!file.is_open()){
        QMessageBox::warning(this,"Error","Cannot open file");
        return;
    }

    std::string header;
    file >> header;
    file.close();

    delete currentPNMImage;
    currentPNMImage = nullptr;
    currentFormat = QString::fromStdString(header);

    if(header=="P1" || header=="P4") {
        PBMImage* img = new PBMImage();
        if(img->load(fileName.toStdString())) currentPNMImage = img;
        else { delete img; return; }
    }
    else if(header=="P2" || header=="P5") {
        PGMImage* img = new PGMImage();
        if(img->load(fileName.toStdString())) currentPNMImage = img;
        else { delete img; return; }
    }
    else if(header=="P3" || header=="P6") {
        PPMImage* img = new PPMImage();
        if(img->load(fileName.toStdString())) currentPNMImage = img;
        else { delete img; return; }
    } else {
        QMessageBox::warning(this,"Error","Unsupported PNM format");
        return;
    }

    // --- Konwersja do QImage ---
    if(header=="P1" || header=="P4"){
        PBMImage* img = dynamic_cast<PBMImage*>(currentPNMImage);
        currentImage = QImage(img->getWidth(), img->getHeight(), QImage::Format_Grayscale8);
        for(int y=0;y<img->getHeight();++y)
            for(int x=0;x<img->getWidth();++x){
                int val = img->getPixel(x,y)?0:255;
                currentImage.setPixel(x,y,qRgb(val,val,val));
            }
    }
    else if(header=="P2" || header=="P5"){
        PGMImage* img = dynamic_cast<PGMImage*>(currentPNMImage);
        currentImage = QImage(img->getWidth(), img->getHeight(), QImage::Format_Grayscale8);
        for(int y=0;y<img->getHeight();++y)
            for(int x=0;x<img->getWidth();++x){
                int val = img->getPixel(x,y);
                currentImage.setPixel(x,y,qRgb(val,val,val));
            }
    }
    else if(header=="P3" || header=="P6"){
        PPMImage* img = dynamic_cast<PPMImage*>(currentPNMImage);
        currentImage = QImage(img->getWidth(), img->getHeight(), QImage::Format_RGB888);
        for(int y=0;y<img->getHeight();++y)
            for(int x=0;x<img->getWidth();++x){
                Pixel p = img->getPixel(x,y);
                currentImage.setPixel(x,y,qRgb(p.r,p.g,p.b));
            }
    }

    displayImage();
}
void MainWindow::saveFile(){
    if(!currentPNMImage){
        QMessageBox::information(this,"Info","No image to save");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this,"Save PNM file","","PNM Files (*.pbm *.pgm *.ppm)");
    if(fileName.isEmpty()) return;
    if(!currentPNMImage->save(fileName.toStdString())){
        QMessageBox::warning(this,"Error","Failed to save file");
        return;
    }
    QMessageBox::information(this,"Info","File saved successfully");
}
// --- Display ---
void MainWindow::displayImage() const {
    if(!currentImage.isNull()){
        imageLabel->setPixmap(QPixmap::fromImage(currentImage).scaled
                (
                    scrollArea->viewport()->size(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                ));
    }
}

void MainWindow::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    displayImage();
}

// --- Operations ---
void MainWindow::desaturateImage(){
    if(currentImage.isNull()) return;
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            int gray = qGray(c.rgb());
            currentImage.setPixel(x,y,qRgb(gray,gray,gray));
        }
    displayImage();
}

void MainWindow::negativeImage(){
    if(currentImage.isNull()) return;
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            currentImage.setPixel(x,y,qRgb(255-c.red(),255-c.green(),255-c.blue()));
        }
    displayImage();
}
void MainWindow::contrastDialog() {
    if(currentImage.isNull()) return;

    bool ok;
    int brightnessFactor = QInputDialog::getInt(this,"Contrast","Set contrast (-100 to 100):",0,-100,100,1,&ok);
    if(!ok) return;
    int lutIndex = brightnessFactor + 100;
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            unsigned char r_o = c.red();
            unsigned char g_o = c.green();
            unsigned char b_o = c.blue();

            int r = contrastLUT[r_o][lutIndex];
            int g = contrastLUT[g_o][lutIndex];
            int b = contrastLUT[b_o][lutIndex];
            currentImage.setPixel(x,y,qRgb(r,g,b));
        }
    displayImage();
}
void MainWindow::adjustBrightnessDialog() {
    if(currentImage.isNull()) return;

    bool ok;
    int brightnessFactor = QInputDialog::getInt(this,"Brightness","Set brightness (-100 to 100):",0,-100,100,1,&ok);
    if(!ok) return;
    int lutIndex = brightnessFactor + 100;
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            unsigned char r_o = c.red();
            unsigned char g_o = c.green();
            unsigned char b_o = c.blue();

            int r = brightnessLUT[r_o][lutIndex];
            int g = brightnessLUT[g_o][lutIndex];
            int b = brightnessLUT[b_o][lutIndex];
            currentImage.setPixel(x,y,qRgb(r,g,b));
        }
    displayImage();
}

void MainWindow::adjustSaturation() {
    if(currentImage.isNull()) return;

    bool ok;
    // Pobierz współczynnik jako procentową zmianę
    int saturationFactorPercent = QInputDialog::getInt(this,"Saturation", "Set saturation (-100 to 1000):",0,-100,1000,1,&ok);
    if(!ok) return;

    // Współczynnik skalowania, gdzie 0% = 1.0 (bez zmian), 100% = 2.0 (podwójne nasycenie), -100% = 0.0 (odszaturowanie)
    // Mapowanie -100..0..100 na 0.0..1.0..2.0
    float saturationScaleFactor = 1.0f + (static_cast<float>(saturationFactorPercent) / 100.0f);

    for(int y=0;y<currentImage.height();++y) {
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            float h,s,l;

            c.getHslF(&h,&s,&l);

            // Zastosowanie skalowania do nasycenia
            float newSaturation = s * saturationScaleFactor;

            // Ograniczenie do zakresu 0.0 do 1.0
            newSaturation = qBound(0.0f, newSaturation, 1.0f);

            // Ustawienie nowego koloru za pomocą nowej wartości nasycenia
            c.setHslF(h, newSaturation, l);

            currentImage.setPixel(x,y,c.rgb());
        }
    }
    displayImage();
}
