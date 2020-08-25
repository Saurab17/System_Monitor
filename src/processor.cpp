#include "processor.h"
#include <unistd.h>
#include "linux_parser.h"

using std::vector;
using std::string;

Processor::Processor() {
    last_stats_ = LinuxParser::CpuUtilization();
    utilization_ = 0;
}
// Todo: Return the aggregate CPU utilization
float Processor::Utilization() { 

    vector<string> current_stats = LinuxParser::CpuUtilization();

    float delta_total = 0.0;
    float delta_idle = 0.0;
    float delta_t = 0.0;
    for(unsigned int index=0; index<current_stats.size(); index++) {
        delta_t = stof(current_stats[index]) - stof(last_stats_[index]);
        delta_total += delta_t;
        if(index == 3 || index == 4) {
            delta_idle += delta_t;
        }
    }

    utilization_ = 100.0 * (delta_total-delta_idle)/delta_total;
    last_stats_ = current_stats;
    return utilization_; 
}