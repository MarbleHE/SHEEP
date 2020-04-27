#include <cassert>
#include <memory>
#include "sheep/context-clear.hpp"

#include "sheep/circuit-repo.hpp"

using namespace SHEEP;

typedef std::chrono::duration<double, std::micro> DurationT;

int main(void) {
  using namespace SHEEP;

  Circuit C;
  Wire in0 = C.add_input("in0");
  Wire cin0 = C.add_const_input("cin0");
  Wire out0 = C.add_assignment("out0", Gate::Rotate, in0, cin0);

  C.set_output(out0);

  std::cout << C;
  std::vector<DurationT> durations;

  // The type of the wires in circ are unsigned bools
  ContextClear<int8_t> ctx;
  ctx.set_parameter("NumSlots", 20);
  // inputs is vector of vectors
  std::vector<std::vector<int8_t>> inputs = {{1, 2, 3, 4, 5, 6, 7, 8}};
  // const_inputs is vector (same across slots)
  std::vector<long> const_inputs = {-3};
  std::vector<int8_t> exp_values = {4, 5, 6, 7, 8, 1, 2, 3};

  std::vector<std::vector<int8_t>> result =
      ctx.eval_with_plaintexts(C, inputs, const_inputs);

  std::cout << "Original vector: ";
  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(inputs[0][i]) << " ";
  }
  std::cout << std::endl << "Rotated vector:  ";
  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(result[0][i]) << " ";
    assert(result.front()[i] == exp_values[i]);
  }
  std::cout << std::endl;

  /// now rotate the other way
  const_inputs = {3};
  exp_values = {6, 7, 8, 1, 2, 3, 4, 5};

  result = ctx.eval_with_plaintexts(C, inputs, const_inputs);

  std::cout << "Original vector: ";
  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(inputs[0][i]) << " ";
  }
  std::cout << std::endl << "Rotated vector:  ";
  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(result[0][i]) << " ";
    assert(result.front()[i] == exp_values[i]);
  }
  std::cout << std::endl;


}
