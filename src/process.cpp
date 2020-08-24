#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::vector;


// Todo: Return this process's ID
int Process::Pid() { 
    return _pid;
}

// Todo: Return this process's CPU utilization
float Process::CpuUtilization() { 

    float process_active_jiffies = LinuxParser::ActiveJiffies(_pid);
    float sys_uptime = LinuxParser::UpTime();
    float start_time_jiffies = LinuxParser::StartTimeJiffy(_pid);
    float freq = sysconf(_SC_CLK_TCK);

    float seconds = sys_uptime - start_time_jiffies/freq;
    float result = 100.0 * (start_time_jiffies/freq)/seconds;
    return result; 
}

// Todo: Return the command that generated this process
string Process::Command() { 
    string command = LinuxParser::Command(_pid);
    return command; 
}

// Todo: Return this process's memory utilization
string Process::Ram() { 
    string ram_usage = LinuxParser::Ram(_pid);
    return ram_usage; 
}

// Todo: Return the user (name) that generated this process
string Process::User() { 
    string user = LinuxParser::User(_pid);
    return user; 
}

// Todo: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long uptime = LinuxParser::UpTime(_pid);
    return 0; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }