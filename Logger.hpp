#ifndef Logger_hpp
#define Logger_hpp

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iomanip>
using std::map;
using std::vector;
using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::mutex;
using std::lock_guard;
using id = std::thread::id;

class Logger {
        map<id, vector<string>> logParts;
        mutex logMutex;
    public:
        class LENDL  {
        };

        Logger& operator << (const string& text);
        Logger& operator << (const int& num);
#ifndef _WIN32
        Logger& operator << (const unsigned long& num);
#endif
        Logger& operator << (const Logger::LENDL);

        void dumpData();

    private:
        void printLogLine();
        void addPart(string part);
        string assembleLogLine(bool removeEntry = true);
};

#define lendl Logger::LENDL()

#endif /* Logger_hpp */
