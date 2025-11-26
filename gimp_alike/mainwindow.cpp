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
}

MainWindow::~MainWindow() {
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

void MainWindow::linearContrast(){
    if(currentImage.isNull()) return;
    int minVal=255,maxVal=0;
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            minVal = std::min({minVal,c.red(),c.green(),c.blue()});
            maxVal = std::max({maxVal,c.red(),c.green(),c.blue()});
        }
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            int r = (c.red()-minVal)*255/(maxVal-minVal);
            int g = (c.green()-minVal)*255/(maxVal-minVal);
            int b = (c.blue()-minVal)*255/(maxVal-minVal);
            currentImage.setPixel(x,y,qRgb(r,g,b));
        }
    displayImage();
}

void MainWindow::logContrast() {
    if(currentImage.isNull()) return;
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            int r = qBound(0,int(255.0*log(1+c.red())/log(256.0)),255);
            int g = qBound(0,int(255.0*log(1+c.green())/log(256.0)),255);
            int b = qBound(0,int(255.0*log(1+c.blue())/log(256.0)),255);
            currentImage.setPixel(x,y,qRgb(r,g,b));
        }
    displayImage();
}

void MainWindow::gammaContrast() {
    if(currentImage.isNull()) return;
    double gamma = 2.2;
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            int r = qBound(0,int(pow(c.red()/255.0,gamma)*255),255);
            int g = qBound(0,int(pow(c.green()/255.0,gamma)*255),255);
            int b = qBound(0,int(pow(c.blue()/255.0,gamma)*255),255);
            currentImage.setPixel(x,y,qRgb(r,g,b));
        }
    displayImage();
}

void MainWindow::contrastDialog() {
    if(currentImage.isNull()) return;
    QStringList options = {"Linear","Log","Gamma"};
    bool ok;
    QString choice = QInputDialog::getItem(this,"Contrast","Select contrast type:",options,0,false,&ok);
    if(!ok) return;

    if(choice=="Linear") linearContrast();
    else if(choice=="Log") logContrast();
    else if(choice=="Gamma") gammaContrast();
}

void MainWindow::adjustBrightnessDialog() {
    if(currentImage.isNull()) return;

    bool ok;
    int value = QInputDialog::getInt(this,"Brightness","Set brightness (-100 to 100):",0,-100,100,1,&ok);
    if(!ok) return;

    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            int r = qBound(0,c.red()+value,255);
            int g = qBound(0,c.green()+value,255);
            int b = qBound(0,c.blue()+value,255);
            currentImage.setPixel(x,y,qRgb(r,g,b));
        }
    displayImage();
}

void MainWindow::adjustSaturation() {
    if(currentImage.isNull()) return;
    for(int y=0;y<currentImage.height();++y)
        for(int x=0;x<currentImage.width();++x){
            QColor c(currentImage.pixel(x,y));
            float h,s,l;
            c.getHslF(&h,&s,&l);
            s = qBound(0.0f, s*1.2f, 1.0f);
            c.setHslF(h,s,l);
            currentImage.setPixel(x,y,c.rgb());
        }
    displayImage();
}
