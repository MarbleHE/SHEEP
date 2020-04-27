#include <cassert>

#include "sheep/circuit-util.hpp"
#include "sheep/circuit.hpp"

#include "sheep/all_equal.hpp"
#include "sheep/context-clear.hpp"

int main(void) {
  Circuit C;
  Wire in1 = C.add_input("in1");
  Wire in2 = C.add_input("in2");
  Wire sum = C.add_assignment("sum", Gate::Add, in1, in2);
  Wire product = C.add_assignment("product", Gate::Multiply, in1, in2);
  C.set_output(sum);
  C.set_output(product);

  std::cout << seq(C, C) << std::endl;

  //	assert(false);
}
