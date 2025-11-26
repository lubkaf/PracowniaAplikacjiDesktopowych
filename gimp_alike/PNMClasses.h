#ifndef PNMCLASSES_H
#define PNMCLASSES_H
#include<iostream>
#include<fstream>
#include<sstream>

inline void skipComments(std::istream& file){
    char c;
    file >> std::ws;
    while (file.peek() == '#') {
        std::string tmp;
        std::getline(file,tmp);
        file >> std::ws;
    }
}
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
//for headers P1 and P4
class PBMImage : public PNMImage {
private:
    bool* data;
    bool isBinary;

public:
    PBMImage(int width = 0, int height = 0)
        : PNMImage(width, height), data(nullptr), isBinary(false) {}

    ~PBMImage() override {
        delete[] data;
    }

    bool load(const std::string& filename) override {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "PBMImage::load: cannot open " << filename << "\n";
            return false;
        }

        std::string header;
        file >> header;

        if (header == "P1") isBinary = false;
        else if (header == "P4") isBinary = true;
        else {
            std::cerr << "PBMImage::load: file is not PBM\n";
            return false;
        }

        skipComments(file);
        file >> width >> height;
        file.get(); // zjada znak końca linii po nagłówku lub wymiarach

        delete[] data;
        data = new bool[width * height];

        if (!isBinary) {
            // ASCII P1: czytamy znaki '0' lub '1'
            char ch;
            int idx = 0;
            while (idx < width * height && file.get(ch)) {
                if (ch == '0') data[idx++] = false;
                else if (ch == '1') data[idx++] = true;
                // ignorujemy spacje, \n, \r
            }
        } else {
            // Binary P4
            int rowBytes = (width + 7) / 8;
            for (int y = 0; y < height; y++) {
                std::vector<unsigned char> row(rowBytes);
                file.read(reinterpret_cast<char*>(row.data()), rowBytes);
                for (int x = 0; x < width; x++) {
                    int byteIndex = x / 8;
                    int bitIndex = 7 - (x % 8);
                    data[y * width + x] = (row[byteIndex] >> bitIndex) & 1;
                }
            }
        }

        return true;
    }

    bool save(const std::string& filename) const override {
        std::ofstream file(filename, isBinary ? std::ios::binary : std::ios::out);
        if (!file.is_open()) return false;

        if (isBinary) file << "P4\n";
        else file << "P1\n";

        file << width << " " << height << "\n";

        if (!isBinary) {
            for (int i = 0; i < width * height; i++) {
                file << (data[i] ? 1 : 0);
                if ((i + 1) % width == 0) file << "\n";
                else file << " ";
            }
            return file.good();
        } else {
            int rowBytes = (width + 7) / 8;
            std::vector<unsigned char> row(rowBytes);

            for (int y = 0; y < height; y++) {
                std::fill(row.begin(), row.end(), 0);
                for (int x = 0; x < width; x++) {
                    int byteIndex = x / 8;
                    int bitIndex = 7 - (x % 8);
                    if (data[y * width + x])
                        row[byteIndex] |= (1 << bitIndex);
                }
                file.write(reinterpret_cast<char*>(row.data()), rowBytes);
            }
            return file.good();
        }
    }

    void print() const override {
        std::cout << "PBM image: " << width << "x" << height << " "
                  << (isBinary ? "(binary)" : "(ASCII)") << "\n";
    }

    bool getPixel(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= height)
            throw std::out_of_range("Pixel coordinates out of range");
        return data[y * width + x];
    }

    void setPixel(int x, int y, bool v) {
        if (x < 0 || y < 0 || x >= width || y >= height)
            throw std::out_of_range("Pixel coordinates out of range");
        data[y * width + x] = v;
    }

    bool* getData() const { return data; }
};//for headers P2 and P5
class PGMImage : public PNMImage {
private:
    unsigned char* data;
    bool isBinary;
    int maxColor;
public:
    PGMImage(int width = 0, int height = 0)
        : PNMImage(width, height), isBinary(false), maxColor(255), data(nullptr) {};
    ~PGMImage() { delete[] data; data = nullptr;};

    bool load(const std::string& filename) override{
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "PGMImage::load: cannot open " << filename << "\n";
            return false;
        }

        std::string line;
        file >> line;


        if (line == "P2") {
            isBinary = false;
        } else if (line == "P5") {
            isBinary = true;
        } else {
            std::cerr << "PGMImage::load: file is not P2/P5\n";
            return false;
        }

        skipComments(file);
        file >> width >> height;
        skipComments(file);
        file >> maxColor;
        file.get();

        delete[] data;
        data = new unsigned char[width * height];

        if (!isBinary) {
            for (int i = 0; i < width * height; i++) {
                int v;
                file >> v;
                data[i] = (unsigned char)v;
            }
            return true;
        }

        file.read((char*)data, width * height);
        return true;
    }
    bool save(const std::string& filename) const override{
        std::ofstream file(filename, isBinary ? std::ios::binary : std::ios::out);
        if (!file.is_open()) return false;

        if (isBinary)
            file << "P5\n";
        else
            file << "P2\n";

        file << width << " " << height << "\n";
        file << maxColor << "\n";

        if (!isBinary) {
            for (int i = 0; i < width * height; i++)
                file << (int)data[i] << "\n";
            return true;
        }

        file.write((char*)data, width * height);
        return file.good();
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
//for headers P3 and P6
class PPMImage : public PNMImage{
private:

    bool isBinary;
    int maxColor;
    unsigned char* data;

    bool loadBinary(std::ifstream& file, int size){
        file.read((char*)data, size);
        return true;
    }
    bool loadASCII(std::ifstream& file, int size){

        for (int i = 0; i < width * height * 3; i++) {
            int value;
            file >> value;
            data[i] = static_cast<unsigned char>(value);
        }
        return true;
    }

public:


    PPMImage(int width = 0, int height = 0)
        : PNMImage(width, height), isBinary(false), maxColor(255), data(nullptr) {}
    ~PPMImage() override { delete[] data; data = nullptr;};

    unsigned char* getData() const {return data; }
    bool load(const std::string& filename) override {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file " << filename << std::endl;
            return false;
        }

        std::string line;
        file >> line;

        if (line == "P3") isBinary = false;
        else if (line == "P6") isBinary = true;
        else {
            std::cerr << "PPMImage::load: File is not a PPM image\n";
            return false;
        }

        skipComments(file);
        file >> width >> height;

        skipComments(file);
        file >> maxColor;
        file.get();

        int size = width * height * 3;

        delete[] data;
        data = new unsigned char[size];

        if (isBinary)
            return loadBinary(file, size);
        else
            return loadASCII(file, size);
    }
    bool save(const std::string& filename) const override {
        std::ofstream file(filename, isBinary ? std::ios::binary : std::ios::out);
        if (!file.is_open()) return false;

        if (isBinary)
            file << "P6\n";
        else
            file << "P3\n";
        file << width << " " << height << "\n";
        file << maxColor << "\n";

        int size = width * height * 3;
        if(isBinary){
            file.write(reinterpret_cast<const char*>(data), size);
            return file.good();
        }
        else{
            for (int i = 0; i < size; i+= 3) {
                file << (int)data[i] << " "
                     << (int)data[i+1]<< " "
                     << (int)data[i+2] << "\n";
            }
            return file.good();
        }
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



#endif // PNMCLASSES_H
