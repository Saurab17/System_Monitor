#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <fstream>

namespace Format {

std::string ElapsedTime(long times);  // Todo: See src/format.cpp
std::ifstream GetStream(std::string path);

};                                    // namespace Format

#endif