#include <string>

#include "format.h"
using std::string;

#define SECONDS_PER_HOUR    3600U
#define MINUTES_PER_HOUR    60U
#define SECONDS_PER_MINUTE  60U
// Todo: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long unsigned seconds) { 
    long unsigned hours, minutes, rem_seconds;
    hours = seconds/SECONDS_PER_HOUR;
    seconds = seconds%SECONDS_PER_HOUR;
    minutes = seconds/MINUTES_PER_HOUR;
    rem_seconds = seconds%SECONDS_PER_MINUTE;

    string elapsed_time = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(rem_seconds);
    int elapsed_time_len = elapsed_time.size();
    if (elapsed_time_len < 9) {
        for(int i=elapsed_time_len; i<=9; i++) {
            elapsed_time = elapsed_time + " ";
        }
    }
    return elapsed_time; 
}

std::ifstream Format::GetStream(std::string path) {
    std::ifstream stream(path);
    if (!stream) {
        throw std::runtime_error("Non-existing Path/PID");
    }
    return stream;
}