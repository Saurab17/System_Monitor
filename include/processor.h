#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // Todo: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    float utilization_;
};

#endif