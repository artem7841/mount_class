#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

class Logger {
public:
    enum Level {
        INFO,
        WARNING,
        ERR,
    };

    Logger(const string& filename) {
        log_file.open(filename, ios::app); // Открытие файла для записи
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(Level level, const string& message) {
        if (log_file.is_open()) {
            log_file << getCurrentTime() << ",[" << levelToString(level) << "]," << message << endl;
        }
    }

private:
    ofstream log_file;

    string getCurrentTime() {
        auto now = chrono::system_clock::now(); // Получаем текущее время
        auto in_time_t = chrono::system_clock::to_time_t(now); // Преобразуем в time_t

        tm localTime;
        localtime_r(&in_time_t, &localTime); // Получаем локальное время для уровня потока

        ostringstream oss; // Поток строки для формирования времени
        oss << put_time(&localTime, "%Y-%m-%d %X"); // Форматируем время
        return oss.str(); // Возвращаем строку с текущим временем
    }

    string levelToString(Level level) {
        switch (level) {
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERR: return "ERROR";
        default: return "UNKNOWN";
        }
    }
};