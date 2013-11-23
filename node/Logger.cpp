#include <iostream>
#include "Logger.h"

using namespace std;

Logger::Logger(string fileName){
    fs.open (fileName.c_str(), std::fstream::app);
}

Logger::~Logger(){
    fs.close();
}

void Logger::log(string str){
    cout << str << endl;
    fs << str << endl;
    fs.flush();
}

ofstream& Logger::operator>> (string & str){
    cout << str << endl;
    fs << str << endl;
    fs.flush();
}

