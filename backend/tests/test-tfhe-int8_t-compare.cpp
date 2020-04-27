#include <cassert>
#include <cstdint>
#include "circuit-test-util.hpp"
#include "sheep/circuit.hpp"
#include "sheep/context-tfhe.hpp"
#include "sheep/simple-circuits.hpp"

typedef std::chrono::duration<double, std::micro> DurationT;

int main(void) {
  using namespace SHEEP;

  Circuit circ;
  Wire a = circ.add_input("a");
  Wire b = circ.add_input("b");
  Wire out = circ.add_assignment("out", Gate::Compare, a, b);
  circ.set_output(out);

  std::cout << circ;
  std::vector<DurationT> durations;
  ContextTFHE<int8_t> ctx;
  ctx.set_parameter("NumSlots", 3);
  std::vector<std::vector<ContextTFHE<int8_t>::Plaintext>> pt_input = {
      {10, -12, -81}, {-12, -12, -80}};
  std::vector<std::vector<ContextTFHE<int8_t>::Plaintext>> result =
      ctx.eval_with_plaintexts(circ, pt_input);
  std::vector<int8_t> exp_values = {1, 0, 0};

  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(pt_input[0][i]) << " > "
              << std::to_string(pt_input[1][i]) << " ? "
              << std::to_string(result[0][i]) << std::endl;
  }

  assert(result.front() == exp_values);
}
