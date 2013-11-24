#include<iostream>
#include "Logger.h"
#include "HostAndPort.h"

using namespace std;

int main(){
    Logger log("output.txt");
    string str = "hello";
    //log << str << "wut?" << endl;
    HostAndPort h(2,3);
    
    log << "ddd" << "ddd" << 1 << " " << h << "\n";
    
    int a;
    cin >> a;
    return 0;
}