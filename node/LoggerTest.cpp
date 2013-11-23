#include<iostream>
#include "Logger.h"

using namespace std;

int main(){
    Logger log("output.txt");
    string str = "hello";
    log << str << endl;
    
    int a;
    cin >> a;
    return 0;
}