#include "processor.h"
#include <unistd.h>
#include "linux_parser.h"

using std::vector;
using std::string;

// Todo: Return the aggregate CPU utilization
float Processor::Utilization() { 

    vector<string> t1 = LinuxParser::CpuUtilization();
    sleep(1);
    vector<string> t2 = LinuxParser::CpuUtilization();

    float delta_total = 0.0;
    float delta_idle = 0.0;
    float delta_t = 0.0;
    float percent_cpu_util = 0.0;

    for(unsigned int index=0; index<t1.size(); index++) {
        delta_t = stof(t2[index]) - stof(t1[index]);
        delta_total += delta_t;
        if(index == 3) {
            delta_idle = delta_t;
        }
    }

    percent_cpu_util = 100.0 * (delta_total-delta_idle)/delta_total;
    return percent_cpu_util; 
}