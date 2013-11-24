
#ifndef LOGGER_H_
#define LOGGER_H_

#include<fstream>


class Logger : public std::ostream {
    public:
        Logger(std::string fileName);
        ~Logger();
    
        void log(std::string str);
        
        std::ostream& operator<< (const std::string & str);
        
        std::ostream& operator<< (const char* str);
        
    private:
        std::fstream fs;
};

#endif
