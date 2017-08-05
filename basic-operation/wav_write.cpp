#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
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

int main( int argc, char *argv[] ) { 

    wavHeader header;

    ofstream of;
    of.open( "output.wav", ios::out | ios::binary | ios::trunc ); //
    assert( of );

    of.seekp( 0, ios::beg ); //point at file top
    
    of.write( "RIFF" , 4 );
    int riff_p = of.tellp();
    of.seekp( sizeof(char)*4, ios::cur );
    of.write( "WAVE" , 4 );
    of.write( "fmt " , 4 );
    int buf = header.formatSize;
    of.write( (char*)&buf, 4 );
    short buf2 = header.formatID;
    of.write( (char*)&buf2, 2 );
    buf2 = header.channels;
    of.write( (char*)&buf2, 2 );
    buf = header.sampleRate;
    of.write( (char*)&buf, 4 );
    header.bytePerSec = 88200;
    buf = header.bytePerSec;
    of.write( (char*)&buf, 4 );
    header.blockSize = 2;
    buf2 = header.blockSize;
    of.write( (char*)&buf2, 2 );
    buf2 = header.quantizationBitSize;
    of.write( (char*)&buf2, 2 );
    cout << "data" << endl;
    of.write( "data" , 4 );
    int data_p = of.tellp();
    of.seekp( sizeof(char)*4, ios::cur );

    cout << "initialized" << endl;

    static const double f1 = 440.0;
    static const double f2 = 880.0;
    static const int t = 5;

    double time = 0.0, fn = 0.0;
    short out = 0;

    while( (int)time < t ){
        for( int i=0; i < 16000; i++ ){
            fn = f1;
            out = (short)( ( sin( (double)2 * M_PI * fn * (double)i / (double)16000 ) ) * (double)0x7FFF );
            of.write( (char*)&out, sizeof(short) );
        }
        cout << "t  = " << time << endl;
        cout << "fn = " << fn << endl;
        cout << "----" << endl;
        time = time + 1.0;
    }

    int last_p = of.tellp();
    of.seekp( riff_p, ios::beg );
    buf = last_p - 8;
    of.write( (char*)&buf, 4 );
    of.seekp( data_p, ios::beg );
    buf = last_p - data_p - 4;
    of.write( (char*)&buf, 4 );

    of.close();

    return 0;
}
