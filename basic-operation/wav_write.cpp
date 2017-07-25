#include <iostream>
#include <fstream>
#include <vector>
#include <string>

typedef struct {
    char riffID[4];
    unsigned int fileSize;
    char waveID[4];
    char fmtID[4];
    unsigned int formatSize;
    unsigned short formatID;
    unsigned short channels;
    unsigned int sampleRate;
    unsigned int bytePerSed;
    unsigned short blockSize; // Byte / sample * channels
    unsigned short quantizationBitSize;
    char dataID[4];
    unsigned int dataSize; // bytes of wave data
} wavHeader;

using namespace std;

int main( int argc, char *argv[]) {

    wavHeader header;
    
    return 0;
}
