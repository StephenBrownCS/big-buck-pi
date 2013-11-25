#include <iostream>
#include <ctime>
#include "Logger.h"


using namespace std;

Logger::Logger(string fileName){
    fs.open (fileName.c_str(), ios::out | std::fstream::app);
    if (! fs.is_open()){
        cerr << "Error: could not open file" << endl;
    }
}

Logger::~Logger(){
    cout << "Close the Logger" << endl;
    fs.close();
}