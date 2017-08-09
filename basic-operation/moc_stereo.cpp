#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <functional>

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
    header.channels = 2;
    header.dataSize = header.dataSize * 2;    
    vector<short> leftNewData = leftData;
    vector<short> rightNewData = leftData;
    vector<short> delayNewData = leftData;
    
    int delay = stoi( argv[3] );
    // 5ms遅延櫛形フィルタ(相補的フィルタ)
    for ( int i=0 ; i < delay; i++){ // 0.005ms * 16000
        delayNewData.insert(delayNewData.begin(), 0);
        delayNewData.pop_back();
    }
    cout << delayNewData.size() << endl;
    cout << leftNewData.size() << endl;
    cout << rightNewData.size() << endl;
    transform( leftNewData.begin(), leftNewData.end(), delayNewData.begin(), leftNewData.end(), plus<short>() );
    transform( rightNewData.begin(), rightNewData.end(), delayNewData.begin(), rightNewData.begin(), minus<short>() );
    //
    cout << leftNewData.size() << endl;
    cout << rightNewData.size() << endl;
    ofstream ofs;
    ofs.open( argv[2], ios::out | ios::binary | ios::trunc );
    assert( ofs );
    if ( !ofs ) {
        cout << "出力ファイルが開けません。" << endl;
        exit(-2);
    }
    
    ofs.write( (char*)&header , sizeof( header ) );
    vector<short>::iterator lit = leftNewData.begin();
    vector<short>::iterator rit = rightNewData.begin();
    short buf2[2];
    while ( lit != leftNewData.end() && rit != rightNewData.end() ){
        if ( lit != leftNewData.end() ){
            buf2[0] = *lit;
            lit++;
        }else{
            buf2[0] = 0;
        }

        if ( rit != rightNewData.end() ){
            buf2[1] = *rit;
            rit++;
        }else{
            buf2[1] = 0;
        }

        ofs.write( (char*)buf2, sizeof( buf2[0] )*2 );
    }
    ofs.close();
    return 0;

}
