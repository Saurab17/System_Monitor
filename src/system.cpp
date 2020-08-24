#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
    cpu_ = Processor();
    processes_ = Processes();
    kernel_ = LinuxParser::Kernel();
    os_ = LinuxParser::OperatingSystem();
    total_processes_ = TotalProcesses();
    running_processes_ = RunningProcesses();
    memory_util_ = MemoryUtilization();
    sys_uptime_ = UpTime();
}

// Todo: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// Todo: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();

    for(int pid : pids) {
        processes_.push_back(Process(pid));
    }
    return processes_; 
}

// Todo: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return kernel_; 
}

// Todo: Return the system's memory utilization
float System::MemoryUtilization() { 
    memory_util_ = LinuxParser::MemoryUtilization();
    return memory_util_; 
}

// Todo: Return the operating system name
std::string System::OperatingSystem() { 
    return os_; 
}

// Todo: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    running_processes_ = LinuxParser::RunningProcesses();
    return running_processes_; 
}

// Todo: Return the total number of processes on the system
int System::TotalProcesses() { 
    total_processes_ = LinuxParser::TotalProcesses();
    return total_processes_; 
}

// Todo: Return the number of seconds since the system started running
long int System::UpTime() { 
    sys_uptime_ = LinuxParser::UpTime();
    return 0; 
}