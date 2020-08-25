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
Process::Process(int pid):
    _pid(pid)
    {
        _user = LinuxParser::User(_pid);
        _ram = LinuxParser::Ram(_pid);
        CalculateCpuUtil();
        _cmd = LinuxParser::Command(_pid);
        _uptime = LinuxParser::UpTime(_pid);;
    }

    
int Process::Pid() const { 
    return _pid;
}

// Todo: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    return _cpu_util; 
}

// Todo: Return the command that generated this process
string Process::Command() const { 
    return _cmd; 
}

// Todo: Return this process's memory utilization
string Process::Ram() const { 
    return _ram; 
}

// Todo: Return the user (name) that generated this process
string Process::User() const { 
    return _user; 
}

// Todo: Return the age of this process (in seconds)
long int Process::UpTime() const { 
    return _uptime; 
}

void Process::CalculateCpuUtil() {
    float process_active_jiffies = LinuxParser::ActiveJiffies(_pid);
    float sys_uptime = LinuxParser::UpTime();
    float start_time_jiffies = LinuxParser::StartTimeJiffy(_pid);
    float freq = sysconf(_SC_CLK_TCK);

    float seconds = sys_uptime - start_time_jiffies/freq;
    _cpu_util = 100.0 * (process_active_jiffies/freq)/seconds;
}

// Todo: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->_cpu_util < a.CpuUtilization(); 
}

bool Process::operator>(Process const& a) const { 
    return this->_cpu_util > a.CpuUtilization(); 
}