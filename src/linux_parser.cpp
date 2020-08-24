#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using Format::GetStream;
using Format::ElapsedTime;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Todo: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
    string mem_file = kProcDirectory + kMeminfoFilename;
    std::ifstream file = GetStream(mem_file);

    unsigned long total_mem_KB;
    unsigned long avail_mem_KB;
    
    bool read_memTotal = false;
    bool read_memAvailable = false;
    
    string token;
    while(file >> token) {
        if(token == "MemTotal:") {
            if(file >> total_mem_KB) {
                read_memTotal = true;
            } else {
                file.close();
                throw std::runtime_error("Couldn't read total memory!!");
                return 0.0;       
            }
        }

        if(token == "MemAvailable:") {
            if(file >> avail_mem_KB) {
                read_memAvailable = true;
            } else {
                file.close();
                throw std::runtime_error("Couldn't read available memory!!");
                return 0.0;       
            }
        }

        if (read_memAvailable && read_memTotal) {
            break;
        }
        // ignore rest of the line
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    file.close();
    float mem_util_percent = ((total_mem_KB - avail_mem_KB)*100.0)/total_mem_KB;
    return mem_util_percent; 
}

// Todo: Read and return the system uptime
long LinuxParser::UpTime() { 
    string uptime_file = kProcDirectory + kUptimeFilename;
    long uptime_seconds;

    std::ifstream file = GetStream(uptime_file);
    file >> uptime_seconds;
    return uptime_seconds; 
}

// Todo: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
    string filepath = kProcDirectory + kStatFilename;
    std::ifstream file = GetStream(filepath);
    
    string key = "cpu";
    
    long sys_jiffies;
    string line;

    while(std::getline(file, line)) {
        if(line.compare(0, key.size(), key) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);

            sys_jiffies = stol(values[3]);
            break;
        }
    }
    file.close();
    return sys_jiffies;
}

// Todo: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
    string filepath = kProcDirectory + "/" + std::to_string(pid) + kStatFilename;
    std::ifstream file = GetStream(filepath);
    
    long process_jiffies;
    string line;
    std::getline(file, line);

    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end);

    process_jiffies = stol(values[13]) + stol(values[14]) + stol(values[15]) 
                        + stol(values[16]);

    file.close();
    return process_jiffies;
}

// Todo: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    string filepath = kProcDirectory + kStatFilename;
    std::ifstream file = GetStream(filepath);
    
    string key = "cpu";
    
    long sys_active_jiffies;
    string line;

    while(std::getline(file, line)) {
        if(line.compare(0, key.size(), key) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);

            sys_active_jiffies = stol(values[2]) - stol(values[3]);
            break;
        }
    }
    file.close();
    return sys_active_jiffies;
    return 0; 
}

// Todo: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
    string filepath = kProcDirectory + kStatFilename;
    std::ifstream file = GetStream(filepath);
    
    string key = "cpu";
    
    long sys_idle_jiffies;
    string line;

    while(std::getline(file, line)) {
        if(line.compare(0, key.size(), key) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);

            sys_idle_jiffies = stol(values[3]);
            break;
        }
    }
    file.close();
    return sys_idle_jiffies;
}

// Todo: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
    string filepath = kProcDirectory + kStatFilename;
    std::ifstream file = GetStream(filepath);
    
    string key = "cpu";
    string line;
    vector<string> values;
    while(std::getline(file, line)) {
        if(line.compare(0, key.size(), key) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            break;
        }
    }
    file.close();
    return values; 
}

// Todo: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    string filepath = kProcDirectory + kStatFilename;
    std::ifstream file = GetStream(filepath);
    string name = "processes";
    string token;
    int total_processes = 0;
    while(file >> token) {
        if(token == name) {
            if(file >> total_processes) {
            } else {
                file.close();
                throw std::runtime_error("Couldn't read total processes!!");
                return total_processes;       
            }
        }
        // ignore rest of the line
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    file.close();
    return total_processes; 
}

// Todo: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string filepath = kProcDirectory + kStatFilename;
    std::ifstream file = GetStream(filepath);
    string name = "procs_running";
    string token;
    int running_procs = 0;
    while(file >> token) {
        if(token == name) {
            if(file >> running_procs) {
            } else {
                file.close();
                throw std::runtime_error("Couldn't read total processes!!");
                return running_procs;       
            }
        }
        // ignore rest of the line
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    file.close();
    return running_procs; 
}

// Todo: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
    string filepath = kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename;
    std::ifstream file = GetStream(filepath);
    string line;
    std::getline(file, line);
    return line; 
}

// Todo: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
    string name = "VmData";
    string filepath = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
    std::ifstream file = GetStream(filepath);
    string line;
    string ram = "";
    
    while(std::getline(file, line)) {
        if(line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);

            ram = values[1];
            break;
        }
    }
    return ram;
}

// Todo: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    string uid_name = "Uid";
    string filepath = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
    std::ifstream file = GetStream(filepath);
    string line;
    string uid = "";
    
    while(std::getline(file, line)) {
        if(line.compare(0, uid_name.size(), uid_name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);

            uid = values[1];
            break;
        }
    }
    return uid;
}

// Todo: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
    
    string uid = Uid(pid);
    string line;
    string username = "";

    std::ifstream file = GetStream(kPasswordPath);

    string name = "x:"+uid;
    while(getline(file,line)) {
        if( line.find(name) != string::npos ) {
            username = line.substr(0, line.find(":"));
            break;
        }
    }
    file.close();
    return username; 
}

// Todo: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
    string filepath = kProcDirectory + "/" + std::to_string(pid) + kStatFilename;
    std::ifstream file = GetStream(filepath);
    
    long process_uptime_jiffy;
    string line;
    std::getline(file, line);

    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end);

    process_uptime_jiffy = stol(values[13]);

    file.close();
    return process_uptime_jiffy;
}

long LinuxParser::StartTimeJiffy(int pid) {
    string filepath = kProcDirectory + "/" + std::to_string(pid) + kStatFilename;
    std::ifstream file = GetStream(filepath);
    
    long process_starttime_jiffy;
    string line;
    std::getline(file, line);

    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end);

    process_starttime_jiffy = stol(values[21]);

    file.close();
    return process_starttime_jiffy;
}