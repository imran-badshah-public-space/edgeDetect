#include "image.h"


bool isComment(string comment) {
    // Check if header contains comments (starts with `'#'`)
    for (uint i = 0; i < comment.length(); i++) {
        if (comment[i] == '#') return true;
        if (!isspace(comment[i])) return false;
    }
    return true;
}

void BinaryImage::readImage(ifstream &inFile) {
    if (!inFile) {
        cerr << "Could not find file." << endl;
        exit(500);
    }

    char* byteArray = new char[imageSize + 1];
    inFile.read(byteArray, imageSize);
    if (inFile.fail()) {
        cerr << "Error: Cannot read pixels." << endl;
        exit(500);
    }

    // Set last element in array to EOF char
    byteArray[imageSize] = '\0';

    for (uint i = 0; i < imageSize; i++) {
        pixels.push_back(
                static_cast<int> (byteArray[i])
            );
    }

    delete[] byteArray;
}

void BinaryImage::writeImage(ofstream &outFile) {
    if (!outFile) {
        cerr << "Could not find file to write to." << endl;
        exit(501);
    }

    // Write header
    outFile << "P5"         << " " <<
            width           << " " <<
            height          << " " <<
            maxPixelValue   << endl;

    char* byteArray = new char[imageSize + 1];

    // Set last element in array to EOF char
    byteArray[imageSize] = '\0';

    for (uint i = 0; i < imageSize; i++) {
        byteArray[i] = static_cast<char> (pixels[i]);
    }

    if (outFile.fail()) {
        cerr << "Error: Cannot write pixels." << endl;
        exit(501);
    }

    delete[] byteArray;
}


void AsciiImage::readImage(ifstream &inFile) {
    if (!inFile) {
        cerr << "Could not find file." << endl;
        exit(502);
    }

    int pixelValue;

    while(inFile >> pixelValue) {
        pixels.push_back(pixelValue);
    }
}

void AsciiImage::writeImage(ofstream &outFile) {
    if (!outFile) {
        cerr << "Could not find file to write to." << endl;
        exit(503);
    }

    // Write header
    outFile << "P2"         << ' ' <<
            width           << ' ' <<
            height          << ' ' <<
            maxPixelValue   << '\n';

    char* byteArray = new char[imageSize + 1];

    for (uint i = 0; i < imageSize; i++) {
        // Add a `'\n'` at the end of each row
        if (i % width == 0 && i != 0) outFile << '\n';
        outFile << pixels.at(i) << '\t';
    }

    if (outFile.fail()) {
        cerr << "Error: Cannot write pixels." << endl;
        exit(501);
    }

    delete[] byteArray;
}

void Image::readHeader(ifstream &inFile) {
    stringstream sStream;
    string line;

    if (!inFile) {
        cerr << "Error: Could not open file." << endl;
        exit(503);
    }

    char readChar;
    string errorMessage = "Error: incorrect picture format.";

    getline(inFile, line);
    uint lineSize = line.length();

    // Magic number and next line validity (no other char besides white space after Pn)
    for (uint i = 0; i < lineSize; i++) {
        if (!isspace(line[i])) {
            cerr << errorMessage << endl;
            cerr << "Extra info after magic number." << endl;
            exit(503);
        }
    }

    // Read through the rest of the header and skip through comments
    while (getline(inFile, line)) {
        if (!isComment(line)) break;
    }

    sStream << line;

    // Read width and height (`>>` ignores whitespace)
    if (!(sStream >> width) || (sStream >> height)) {
        cerr << errorMessage << endl;
        cerr << "Cannot read width or height ";
    }

    // Check if extra info after width and height
    while (sStream >> readChar) {
        if (!(isspace(readChar))) {
            cerr << errorMessage << endl;
            cerr << "Extra info when reading height and width" << endl;
            exit(503);
        }
    }


    // Make sure height and width are positive
    if (width <= 0 || height <= 0) {
        cerr << "Error: width and height cannot be negative." << endl;
        exit(503);
    }

    // Check if comments after height, width or maxPixelValue
    while (getline(inFile, line)) {
        if (!(isComment(line))) break;
    }

    // Clear out the string stream
    sStream.str("");
    sStream.clear();
    sStream << line;

    // Read in the maxPixelValue
    if (!(sStream >> maxPixelValue)) {
        cerr << errorMessage << endl;
        cerr << "Could not read maxPixelValue." << endl;
        exit(503);
    }

    // Check if extra info after maxPixelValue
    while (sStream >> readChar) {
        if (!(isspace(readChar))) {
            cerr << errorMessage << endl;
            cerr << "Extra info when reading maxPixelValue" << endl;
            exit(503);
        }
    }

    if (maxPixelValue < 0 || maxPixelValue > 255) {
            cerr << errorMessage << endl;
            cerr << "Invalid maxPixelValue" << endl;
            exit(503);
    }

    imageSize = width * height;
}

void Image::findMax() {
    int maxVal = 0;
    for (uint i = 0; i < imageSize; i++)  {
        if (pixels[i] > maxVal) {
            maxVal = pixels[i];
        }
    }
    maxpix = maxVal;
}

void Image::findMin() {
    int minVal = 255;
    for (uint i = 0; i < imageSize; i++)  {
        if (pixels[i] < minVal) {
            minVal = pixels[i];
        }
    }
    minpix = minVal;
}

void Image::scaleImage() {
    double calc = 0.0;
    int newPixelValue = 0;
    findMin();
    findMax();

    for (uint i = 0; i < imageSize; i++) {
        calc = (double)(pixels[i] - minpix) / (maxpix - minpix);
        newPixelValue = round(calc * 255);
        pixels.at(i) = newPixelValue;
    }

    maxPixelValue = 255;
}


// Sobel
void Image::edgeDetect() {
    int x, y = 0;
    int xG, yG = 0;
    int* tempImage = new int[imageSize];
    for (uint i = 0; i < imageSize; i++) {
        x = i % width;
        if (i != 0 && x == 0) {
            y++;
        }
        if (x < (width - 1) && y < (height - 1)
                && (y > 0) && (x > 0)) {

            // index = j + i*w
            // pixels[(x + 1) + ((y - 1) * width)]

            // Finds hori grad
            xG = (
                  pixels[(x + 1) + ((y - 1) * width)]
                + 2 * pixels[(x + 1) + ((y) * width)]
                + pixels[(x + 1) + ((y + 1) * width)]
                - pixels[(x - 1) + ((y - 1) * width)]
                - 2 * pixels[(x - 1) + ((y) * width)]
                - pixels[(x - 1) + ((y + 1) * width)]
            );
            // Finds verti grad
            yG = (
                - pixels[(x - 1) + ((y - 1) * width)]
                - 2 * pixels[(x) + ((y - 1) * width)]
                - pixels[(x + 1) + ((y - 1) * width)]
                + pixels[(x - 1) + ((y + 1) * width)]
                + 2 * pixels[(x) + ((y + 1) * width)]
                + pixels[(x + 1) + ((y + 1) * width)]
            );
            // newPixel = sqrt(xG^2 + yG^2)
            tempImage[i] = sqrt((xG * xG) + (yG * yG));
        } else {
            // Zero-padding
            tempImage[i] = 0;
        }
    }

    // Overwrite image
    for (uint i = 0; i < imageSize; i++) {
        pixels[i] = tempImage[i];
    }
}