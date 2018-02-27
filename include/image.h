#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <time.h>

using namespace std;

class Image {
    public:
        Image():
            height(0),
            width(0),
            maxPixelValue(0),
            minpix(0),
            maxpix(0) {};
        virtual ~Image() {};

        virtual void readImage(ifstream &inFile) = 0;
        virtual void writeImage(ofstream &outFile) = 0;

        void readHeader(ifstream &inFile);
        void scaleImage();
        void edgeDetect();

        int getHeight() { return height; };
        int getWidth() { return width; };
        int getMaxPixelValue() { return maxPixelValue; };

        void setHeight(int h) { height = h; };
        void setWidth(int w) { width = w; };
        void setMaxPixelValue(int mpv) { maxPixelValue = mpv; };

    protected:
        int height;
        int width;
        int maxPixelValue;
        int minpix;
        int maxpix;
        uint imageSize;
        vector<int> pixels;

        inline void findMin();
        inline void findMax();
};


class BinaryImage: public Image {
    public:
        BinaryImage() {};
        ~BinaryImage() {};

        void readImage(ifstream &inFile);
        void writeImage(ofstream &outFile);
};

class AsciiImage: public Image {
    public:
        AsciiImage() {};
        ~AsciiImage() {};

        void readImage(ifstream &inFile);
        void writeImage(ofstream &outFile);
};


#endif