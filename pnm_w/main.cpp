#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


class PPMImage {
private:
    int width, height, maxColor;
    unsigned char* data;
public:
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
    void print() const;
    PPMImage(int const width, int const height) : width(width), height(height), maxColor(255), data(nullptr) {};
    ~PPMImage() { delete[] data; };

};
//header P3
bool PPMImage::load(const std::string& filename) {
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
bool PPMImage::save(const std::string& filename) const {
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

class PBMImage {
private:
    int width, height;
    bool* data;
public:
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
    void print() const;
    PBMImage(int const width, int const height) : width(width), height(height),data(nullptr) {};
    ~PBMImage() { delete[] data; };
};
//header P1
bool PBMImage::load(const std::string& filename) {
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
bool PBMImage::save(const std::string& filename) const {
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

class PGMImage {
    private:
    int width, height;
    unsigned char* data;
    public:
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
    void print() const;
    PGMImage(int const width, int const height) : width(width), height(height),data(nullptr) {};
    ~PGMImage() { delete[] data; };
};
//header P2
bool PGMImage::load(const std::string& filename) {
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
bool PGMImage::save(const std::string& filename) const {
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
}

int main() {
    PGMImage image(30, 30);
    std::cout<<image.load("../pictures/okrag.pgm");
    std::cout<<image.save("../pictures/okrag2.pgm");


    return 0;
}