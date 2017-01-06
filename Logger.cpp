#include "Logger.hpp"

Logger& Logger::operator << (const string& line) {
    lock_guard<mutex> logLock(this->logMutex);

    addPart(line);

    return *this;
}

Logger& Logger::operator << (const int& num) {
    lock_guard<mutex> logLock(this->logMutex);

    addPart(to_string(num));

    return *this;
}

#ifndef _WIN32
Logger& Logger::operator << (const unsigned long& num) {
    lock_guard<mutex> logLock(this->logMutex);

    addPart(to_string(num));

    return *this;
}
#endif
Logger& Logger::operator << (const Logger::LENDL) {
    lock_guard<mutex> logLock(this->logMutex);

    printLogLine();

    return *this;
}

void Logger::printLogLine() {
    string logLine = assembleLogLine();

    // And add the date and time
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    cout << std::this_thread::get_id() << " : " << std::put_time(&tm, "%m/%d,%H:%M:%S") << " ---> " << logLine << endl;
}

string Logger::assembleLogLine(bool removeEntry /* = true */) {
    auto threadID = std::this_thread::get_id();
    auto findIt = logParts.find(threadID);
    string logLine;

    if (findIt != logParts.end()) {
        for (string part : findIt->second) {
            logLine.append(part);
        }
    }

    if (removeEntry) {
        logParts.erase(threadID);
    }

    return logLine;
}

void Logger::addPart(string part) {
    auto threadID = std::this_thread::get_id();
    auto findIt = logParts.find(threadID);

    if (findIt == logParts.end()) {
        vector<string> partsVec;
        partsVec.push_back(part);
        logParts.insert(std::make_pair(threadID, partsVec));
    } else {
        findIt->second.push_back(part);
    }
}

void Logger::dumpData() {
    for (auto& kv : logParts) {
        string logLine;

        for (string part : kv.second) {
            logLine.append(part);
        }

        // And add the date and time
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        cout << std::put_time(&tm, "%m/%d,%l:%M:%S %p") << " ****> " << kv.first << " - " << logLine << endl;
    }
}
