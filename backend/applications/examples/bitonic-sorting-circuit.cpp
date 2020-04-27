#include <cstdint>
#include <fstream>
#include <iostream>
#include "sheep/all_equal.hpp"
#include "sheep/context.hpp"
#include "sheep/simple-circuits.hpp"

int main(int argc, const char** argv) {
  if (argc < 3) {
    std::cout << "Usage:  bitonic-sorting-circuit <num_inputs> <filename>"
              << std::endl;
    return 0;
  }
  int num_inputs = std::stoi(argv[1]);
  std::string filename = argv[2];

  std::ofstream outfile(filename);

  Circuit bitonic = bitonic_sort(num_inputs, false);

  outfile << bitonic;
  outfile.close();
}
