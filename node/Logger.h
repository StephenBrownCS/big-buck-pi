
#ifndef LOGGER_H_
#define LOGGER_H_

#include<fstream>
#include<ctime>


class Logger : public std::ofstream {
    public:
        Logger(std::string fileName);
        ~Logger();
                
    template< typename T >
    Logger& operator<< (T thing){
        time_t now;
        time(&now);
    
        std::cout << "\n" << ctime(&now) << "\t" << thing << " ";
        fs << "\n" << ctime(&now)  << "\t" << thing << " ";
        fs.flush();
        return *this;
    }
        
    private:
        std::fstream fs;
};

#endif
