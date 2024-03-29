# include "image.h"


bool isBinary(ifstream &inFile);
void run (char **argv);

int main (int argc, char **argv) {

    if (argc != 3) {
        cerr << "Usage: bin/edgeDetect imageName.pgm output.pgm"; // Creates output file automatically
        return 1;
    }

    long start, end;
    double total;

    start = clock();
    run(argv);
    end = clock();
    total = (double) (end - start) / 1000;
    cout << "Total time: " << total << endl;
    return 0;
}

bool isBinary(ifstream &inFile) {
    char readChar = ' ';
    string errorMessage = "Error: incorrect picture format.";

    // If there is no char or char != 'P'  => error
    if (!(inFile >> readChar) || (readChar != 'P')) {
        cerr << errorMessage << endl;
        cerr << "P" << endl;
        exit(502);
    }
    // If there is no char or 2nd char != '2' or '5'  => error
    if (!(inFile >> readChar) || (readChar != '2' && readChar != '5')) {
        cerr << errorMessage << endl;
        cerr << readChar<< endl;
        exit(502);
    }

    if (readChar == '5') return true;
    return false;
}

// cmd line args
void run(char **argv) {
    ifstream inFile;
    inFile.open(argv[1], ios::binary | ios::in); // ios:: for Windows
    ofstream outFile;
    outFile.open(argv[2], ios::binary | ios::out | ios::trunc);

    if (isBinary(inFile)) {
        BinaryImage binaryImage;
        binaryImage.readHeader(inFile);
        binaryImage.readImage(inFile);
        binaryImage.edgeDetect();
        binaryImage.scaleImage();
        binaryImage.writeImage(outFile);
    } else {
        AsciiImage asciiImage;
        asciiImage.readHeader(inFile);
        asciiImage.readImage(inFile);
        asciiImage.edgeDetect();
        asciiImage.scaleImage();
        asciiImage.writeImage(outFile);
    }

    inFile.close();
    outFile.close();
}