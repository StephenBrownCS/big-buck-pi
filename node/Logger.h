
#ifndef LOGGER_H_
#define LOGGER_H_

#include<fstream>


class Logger : public std::ofstream {
    public:
        Logger(std::string fileName);
        ~Logger();
    
        void log(std::string str);
        
    template< typename T >
    Logger& operator<< (T thing){
        std::cout << thing;
        fs << thing;
        fs.flush();
        return *this;
    }
        
    private:
        std::fstream fs;
};

#endif
