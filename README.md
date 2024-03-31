# PGM reader and Edge detection

## Description
***entirely from scratch***: Reads and writes a [PGM image](https://en.wikipedia.org/wiki/Netpbm#File_formats) upon performing edge detection (Sobel) **without** relying on any external helper libraries.

**Bonus:** An auto-compile against the Makefile on file save ;)

## Tech Stack

### Languages
- C++
- Makefile
- Bash

### Frameworks and Libraries
- math.h
- fstream
- vector
- sstream
- time.h

### Algorithms and Techniques
- Sobel Operator for Edge Detection
- Automatically run Makefile on file save
- Image File streaming (reading and writing)
- Image scaling

### Categories
- Imaging
- Scripting
- Console

### Platforms
- Linux

## Install
```
make
```

## Usage

```shell
cd bin
./edgeDetect mountain.pgm ouput.pgm
```

> NOTE: A sample image and the generated output is placed in the `bin` dir for reference.

 
