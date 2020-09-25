#include <string>
#include <iostream>
#include "../include/WzSerialPort.h"
using namespace std;
WzSerialPort w;

char check(unsigned char *data, int len) {
    uchar result = 0x00;
    for(int i = 0; i < len - 1; i++){
        result += data[i];
    }
    return result;

}
int zoo_enable(char id = 0x0C) {
    int len = 7;
    unsigned char data[len];
    data[0] = 0x5A;
    data[1] = id;
    data[2] = 0x03;
    data[3] = 0x01;
    data[4] = 0x01;
    data[5] = 0x01;
    data[7] = check(data, len);
    if(w.send(data, len))
        return 1;
    else
        return -1;

}
int zoo_disable(char id = 0x0C) {
    int len = 7;
    unsigned char data[len];
    data[0] = 0x5A;
    data[1] = id;
    data[2] = 0x03;
    data[3] = 0x00;
    data[4] = 0x00;
    data[5] = 0x00;
    data[7] = check(data, len);
    if(w.send(data, len))
        return 1;
    else
        return -1;

}
int main(int argc, char **argv){
    w.open("/dev/zoo", 115200, 'N', 8, 1);
    if(argv[1] == "enable")
        if(zoo_enable())
            cout << "enable success!!" << endl;
        else
            cout << "enable fail!!" << endl;
    else{
        if(zoo_disable())
            cout << "disable success!!" << endl;
        else
            cout << "disable fail!!" << endl;
    }
    return 0;
}
