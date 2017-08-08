#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <cassert>

typedef struct {
    char riffID[4];
    unsigned int fileSize;
    char waveID[4];
    char fmtID[4];
    unsigned int formatSize;
    unsigned short formatID;
    unsigned short channels;
    unsigned int sampleRate;
    unsigned int bytePerSec;
    unsigned short blockSize; // Byte / sample * channels
    unsigned short quantizationBitSize;
    char dataID[4];
    unsigned int dataSize; // bytes of wave data
} wavHeader;

using namespace std;

int main(int argc, char *argv[]){

    wavHeader header;
    
    ifstream ifs;
    ifs.open( argv[1] );
    assert( ifs );
    
    if ( !ifs ){
        cout << "入力ファイルが開けません" << endl;
        exit(-1);
    }
    
    cout << argv[1] << "を開きました。" << endl;

    ifs.read( (char*)&header , sizeof( header ) );

    vector<short> leftData;

    short buf;
    do {
        ifs.read( (char*)&buf, sizeof( buf ) );
        leftData.push_back( buf );
    } while ( !ifs.eof() );
    cout << "end" << endl;
    ifs.close();

    // 加工するならここで

    //
    
    vector<short> leftNewData = leftData;

    ofstream ofs;
    ofs.open( argv[2], ios::out | ios::binary | ios::trunc );
    assert( ofs );
    if ( !ofs ) {
        cout << "出力ファイルが開けません。" << endl;
        exit(-2);
    }

    ofs.write( (char*)&header , sizeof( header ) );
    vector<short>::iterator lit = leftNewData.begin();

    while ( lit != leftNewData.end() ){
        if ( lit != leftNewData.end() ){
            buf = *lit;
            lit++;
        }else{
            buf = 0;
        }

        ofs.write( (char*)&buf, sizeof( buf ) );
    }
    ofs.close();
    return 0;

}
