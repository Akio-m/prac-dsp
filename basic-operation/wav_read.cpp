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

int main( int argc, char *argv[] ) {
    
    wavHeader header;

    ifstream ifs;
    ifs.open( argv[1] );
    if ( !ifs ) {
        cout << "Can not open " << argv[1] << "." << endl;
	exit(1);
    }
    cout << "Open " << argv[1] << "." << endl;
    ifs.read( (char*)&header, sizeof(header) );

    int sampleNumber = header.dataSize / 2 / header.channels; // 2 byte = 1 sample
    cout << "wav data is \n";
    cout << "FileSize      = " << header.fileSize << "\n";
    cout << "Channel       = " << header.channels << "\n";
    cout << "Sample Rate   = " << header.sampleRate << "\n";
    cout << "Sample Number = " << sampleNumber << endl;

    vector<short> leftChannelData;
    vector<short> rightChannelData;
    do {
        short buffer[2];
        ifs.read( (char*)buffer, sizeof( buffer[0] ) *2 );
        leftChannelData.push_back( buffer[0] );
        rightChannelData.push_back( buffer[1] );
    } while ( !ifs.eof() );
    
    for ( int i ; i < (int)leftChannelData.size(); ++i ) {
        cout << header.quantizationBitSize << "bit = left : " << leftChannelData[i] << " right : " << rightChannelData[i] << endl;
    }
    ifs.close();


    return 0;
}
