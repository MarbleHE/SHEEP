#include <cassert>
#include <memory>
#include "sheep/context-seal-bfv.hpp"

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
  ContextSealBFV<bool> ctx;
  ctx.set_parameter("BaseParamSet",0);
  ctx.configure();
  // inputs is vector of vectors
  std::vector<std::vector<bool>> inputs = {{1, 0, 0 , 0, 0}};
  // const_inputs is vector (same across slots)
  std::vector<long> const_inputs = {-2};
  std::vector<bool> exp_values = {0, 0, 0, 1, 0};

  std::vector<std::vector<bool>> result =
      ctx.eval_with_plaintexts(C, inputs, const_inputs);

  std::cout << "Original vector: ";
  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(inputs[0][i]) << " ";
  }
  std::cout << std::endl << "Rotated vector:  ";
  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(result[0][i]) << " ";
  }
  for (int i=0; i < exp_values.size(); i++) {
    assert(exp_values[i] == result[0][i]);
  }
  /// now rotate right
  inputs = {{0,0, 0, 1, 0}};
  // const_inputs is vector (same across slots)
  const_inputs = {2};
  exp_values = {1,0,0,0,0};

  result =
      ctx.eval_with_plaintexts(C, inputs, const_inputs);

  std::cout << "Original vector: ";
  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(inputs[0][i]) << " ";
  }
  std::cout << std::endl << "Rotated vector:  ";
  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(result[0][i]) << " ";
  }
  for (int i=0; i < exp_values.size(); i++) {
    assert(exp_values[i] == result[0][i]);
  }

  std::cout << std::endl;
}
