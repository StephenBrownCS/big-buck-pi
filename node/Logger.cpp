#include <iostream>
#include "Logger.h"

using namespace std;

Logger::Logger(string fileName){
    fs.open (fileName.c_str(), ios::out | std::fstream::app);
    if (! fs.is_open()){
        cerr << "Error: could not open file" << endl;
    }
}

Logger::~Logger(){
    cout << "Close " << endl;
    fs.close();
}

void Logger::log(string str){
    cout << str << endl;
    fs << str << endl;
    fs.flush();
}

ofstream& Logger::operator<< (const string & str){
    (*this) << str.c_str();
}

ofstream& Logger::operator<< (const char* str){
    cout << str;
    fs << str;
    fs.flush();
}

