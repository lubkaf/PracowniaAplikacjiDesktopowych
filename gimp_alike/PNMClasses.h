#ifndef PNMCLASSES_H
#define PNMCLASSES_H
#include<iostream>
#include<fstream>
#include<sstream>
//parent class
class PNMImage{
protected:
    int width, height;

public:
    PNMImage(int width = 0, int height = 0) : width(width), height(height){}
    virtual ~PNMImage() {}

    int getWidth() const {return width;}
    int getHeight() const {return height;}

    virtual bool load(const std::string& filename) = 0;
    virtual bool save(const std::string& filename) const = 0;
    virtual void print() const = 0;
};

struct Pixel{
    unsigned char r,g,b;
};

//only for header P3
class PPMImage : public PNMImage{
private:
    int maxColor;
    unsigned char* data;
public:


    PPMImage(int width = 0, int height = 0)
        : PNMImage(width, height), maxColor(255), data(nullptr) {}
    ~PPMImage() override { delete[] data; data = nullptr;};

    unsigned char* getData() const {return data; }
    bool load(const std::string& filename) override {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        file >> line;
        if (line != "P3") {
            std::cerr << "PPMImage::load: File is not a PPM image" << std::endl;
            return false;
        }

        //ignorowanie komentarzy
        do {
            std::getline(file, line);
        } while (line.empty() || line[0] == '#');

        std::istringstream iss(line);
        iss >> width >> height;

        do {
            std::getline(file, line);
        } while (line.empty() || line[0] == '#');

        maxColor = std::stoi(line);

        data = new unsigned char[width * height * 3];
        for (int i = 0; i < width * height * 3; i++) {
            int value;
            file >> value;
            data[i] = static_cast<unsigned char>(value);
        }
        return true;
    }
    bool save(const std::string& filename) const override {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        file << "P3\n";
        file << width << " " << height << "\n";
        file << maxColor << "\n";

        for (int i = 0; i < width * height * 3; i+= 3) {
            file << (int)data[i] << " "
                 << (int)data[i+1]<< " "
                 << (int)data[i+2] << "\n";
        }

        file.close();
        return true;
    }
    void print() const override {
        std::cout << "PPM image: " << width << "x" << height << " max=" << maxColor << "\n";
    }
    Pixel getPixel(int x, int y) const{
        if(x < 0 || y < 0 || x>= width || y>=height)
            throw std::out_of_range("Pixel coordinates out of range");
        int idx = (y*width + x) * 3;
        return {data[idx], data[idx+1], data[idx+2]};
    }
    void setPixel(int x, int y, Pixel p) {
        if (x < 0 || y < 0 || x >= width || y >= height)
            throw std::out_of_range("Pixel coordinates out of range");
        int idx = (y * width + x) * 3;
        data[idx] = p.r;
        data[idx + 1] = p.g;
        data[idx + 2] = p.b;
    }
};
//only for header P1
class PBMImage : public PNMImage {
private:
    bool* data;
public:

    PBMImage(int const width = 0, int const height = 0) : PNMImage(width, height),data(nullptr) {};
    ~PBMImage() { delete[] data; data = nullptr; };

    bool* getData() const {return data;}
    bool load(const std::string& filename) override{
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr<<"Unable to open file "<<filename<<std::endl;
            return false;
        }
        std::string line;
        file >> line;
        if (line != "P1") {
            std::cerr << "PBMImage::load: File is not a PBM image" << std::endl;
            return false;
        }

        while (true) {
            file >> line;
            if (line[0] == '#') {
                std::getline(file, line);
                continue;
            }
            width = std::stoi(line);
            break;
        }
        while (true) {
            file >> line;
            if (line[0] == '#') {
                std::getline(file, line);
                continue;
            }
            height = std::stoi(line);
            break;
        }

        data = new bool[width * height];
        char c;
        int count = 0;

        while (file.get(c) && count < width * height) {
            if (c == '0' || c == '1') {
                data[count++] = (c == '1');
            }
        }
        return true;
    }
    bool save(const std::string& filename) const override{
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr<<"Unable to open file "<<filename<<std::endl;
            return false;
        }

        file << "P1\n";
        file << width << " " << height << "\n";
        for (int i = 0; i < width * height; i++) {
            file << (int)data[i];
            if ((i + 1) % width == 0) {
                file << "\n";
            } else {
                file << " ";
            }
        }
        file.close();
        return true;


    }
    void print() const override {
        std::cout << "PBM image: " << width << "x" << height << "\n";
    }
    bool getPixel(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= height)
            throw std::out_of_range("Pixel coordinates out of range");
        return data[y * width + x];
    }
    void setPixel(int x, int y, bool value) {
        if (x < 0 || y < 0 || x >= width || y >= height)
            throw std::out_of_range("Pixel coordinates out of range");
        data[y * width + x] = value;
    }
};
//only for header P2
class PGMImage : public PNMImage {
private:
    unsigned char* data;

public:
    PGMImage(int const width = 0, int const height = 0) : PNMImage(width, height),data(nullptr) {};
    ~PGMImage() { delete[] data; data = nullptr;};

    bool load(const std::string& filename) override{
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr<<"Unable to open file "<<filename<<std::endl;
            return false;
        }

        std::string line;
        file >> line;

        if (line != "P2") {
            std::cerr << "PGMImage::load: File is not a PGM image" << std::endl;
        }
        while (true) {
            file >> line;
            if (line[0] == '#') {
                std::getline(file, line);
                continue;
            }
            width = std::stoi(line);
            break;
        }
        file >> line;
        height = std::stoi(line);

        data = new unsigned char[width * height];
        for (int i = 0; i < width * height; i++) {
            int value;
            file >> value;
            data[i] = static_cast<unsigned char>(value);
        }
        return true;
    }
    bool save(const std::string& filename) const override{
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr<<"Unable to open file "<<filename<<std::endl;
            return false;
        }
        file << "P2\n";
        file << width << " " << height << "\n";
        file << "255\n";
        for (int i = 0; i < width * height; i++) {
            file << (int)data[i] << '\n';

        }
        return true;
    }
    void print() const override {
        std::cout << "PGM image: " << width << "x" << height << "\n";
    }
    unsigned char getPixel(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= height)
            throw std::out_of_range("Pixel coordinates out of range");
        return data[y * width + x];
    }
    void setPixel(int x, int y, unsigned char value) {
        if (x < 0 || y < 0 || x >= width || y >= height)
            throw std::out_of_range("Pixel coordinates out of range");
        data[y * width + x] = value;
    }
};

#endif // PNMCLASSES_H
