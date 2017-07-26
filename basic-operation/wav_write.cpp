#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>

typedef struct {
    char riffID[4];
    unsigned int fileSize = 0;
    char waveID[4];
    char fmtID[4];
    unsigned int formatSize = 16;
    unsigned short formatID = 1;
    unsigned short channels = 1;
    unsigned int sampleRate = 16000;
    unsigned int bytePerSec = 0;
    unsigned short blockSize = 0; // Byte / sample * channels
    unsigned short quantizationBitSize = 16;
    char dataID[4];
    unsigned int dataSize = 0; // bytes of wave data
} wavHeader;

using namespace std;

int main( int argc, char *argv[]) {

    wavHeader header;

    if( argc != 2 ){
        cout << "Usage: ./a.out output_file.wav"<< endl;
        exit(-1);
    }
    ofstream outputFiles( argv[1] );

    memcpy(header.riffID, "RIFF", 4);
    memcpy(header.waveID, "WAVE", 4);
    memcpy(header.fmtID, "fmt ", 4);
    memcpy(header.dataID, "data", 4);

    double f = 1000.0; // sin wave frequency
    header.dataSize = header.channels * ( 16000 + 0 ) * 2; // 1 sample is 2 bytes
    header.fileSize = 36 + header.dataSize;
    header.bytePerSec = header.sampleRate * header.channels * 2;
    header.blockSize = header.channels * 2; // byte / sample * channels 

    outputFiles << header.riffID;
    outputFiles << header.fileSize;
    outputFiles << header.waveID;
    outputFiles << header.fmtID;
    outputFiles << header.formatSize;
    outputFiles << header.formatID; 
    outputFiles << header.channels; 
    outputFiles << header.sampleRate;
    outputFiles << header.bytePerSec;
    outputFiles << header.blockSize;
    outputFiles << header.quantizationBitSize;
    outputFiles << header.dataID;
    outputFiles << header.dataSize;
    
    cout << "output data wav is " << argv[1] << endl;
    cout << "Channel = " << header.channels;
    cout << "\nSample rate = " << header.sampleRate;
    cout << "\nSample number = " << header.dataSize / header.channels / 2 << endl;

    int t = 0;
    long int t_out = 0;
    double s[16000 + 1] = { 0 };
    double y[16000 + 1] = { 0 };
    short output = 0;

    while( t_out < 16000 ) {
        s[t] = 0.5 * sin( 2.0 * M_PI * f / header.sampleRate * t_out );
        y[t] = s[t];
        output = y[t] * 23767; // output force to decimal
        outputFiles << output;
        if (header.channels == 2) {
            outputFiles << output; // output for Right channel 
        }
        t = (t + 1) % 16000;
        t_out++;
    }
    
    outputFiles.close();
    return 0;
}
