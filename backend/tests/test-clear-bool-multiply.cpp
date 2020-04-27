#include <memory>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include "sheep/circuit-repo.hpp"
#include "circuit-test-util.hpp"
#include "sheep/context-clear.hpp"

typedef std::chrono::duration<double, std::micro> DurationT;

int main(void) {
  using namespace SHEEP;

  //// instantiate the Circuit Repository
  CircuitRepo cr;

  Circuit circ = cr.create_circuit(Gate::Multiply, 1);
  std::cout << circ;
  std::vector<DurationT> durations;
  ContextClear<bool> ctx;

  /// test two 1s
  /// test one of each
  /// test both zeros
  std::vector<std::vector<bool>> inputs = {{1, 0, 1, 0}, {1, 1, 0, 0}};
  std::vector<bool> exp_values = {1, 0, 0, 0};

  std::vector<std::vector<bool>> result =
      ctx.eval_with_plaintexts(circ, inputs);

  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(inputs[0][i]) << " * "
              << std::to_string(inputs[1][i]) << " = "
              << std::to_string(result[0][i]) << std::endl;
    assert(result.front()[i] == exp_values[i]);
  }
}
