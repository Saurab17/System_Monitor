#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  Processor();
  float Utilization();  // Todo: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    std::vector<std::string> last_stats_;
    float utilization_;
};

#endif