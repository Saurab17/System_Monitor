#include <string>

#include "format.h"

using std::string;
// Todo: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hours, minutes, rem_seconds;
    hours = seconds/3600;
    seconds = seconds%3600;
    minutes = seconds/60;
    rem_seconds = seconds%60;

    string elapsed_time = std::to_string(hours) + ":" + std::to_string(minutes)\
                           + ":" + std::to_string(rem_seconds);
    return elapsed_time; 
}

std::ifstream Format::GetStream(std::string path) {
    std::ifstream stream(path);
    if (!stream) {
        throw std::runtime_error("Non-existing Path/PID");
    }
    return stream;
}