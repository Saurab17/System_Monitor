#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   // Todo: See src/system.cpp
  std::vector<Process>& Processes();  // Todo: See src/system.cpp
  float MemoryUtilization();          // Todo: See src/system.cpp
  long UpTime();                      // Todo: See src/system.cpp
  int TotalProcesses();               // Todo: See src/system.cpp
  int RunningProcesses();             // Todo: See src/system.cpp
  std::string Kernel();               // Todo: See src/system.cpp
  std::string OperatingSystem();      // Todo: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string kernel_;
  std::string os_;
  float memory_util_;
  int total_processes_;
  int running_processes_;
  long int sys_uptime_;
};

#endif