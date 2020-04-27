#include <algorithm>
#include <cassert>
#include <cstdint>
#include "circuit-test-util.hpp"
#include "sheep/context-tfhe.hpp"
#include "sheep/simple-circuits.hpp"

typedef std::chrono::duration<double, std::micro> DurationT;

int main(void) {
  using namespace SHEEP;

  Circuit circ;
  Wire in = circ.add_input("in");
  Wire out = circ.add_assignment("out", Gate::Negate, in);
  circ.set_output(out);

  std::vector<DurationT> durations;
  ContextTFHE<uint8_t> ctx;
  ctx.set_parameter("NumSlots", 2);
  std::vector<std::vector<ContextTFHE<uint8_t>::Plaintext>> pt_input = {
      {15, 128}};

  std::vector<std::vector<ContextTFHE<uint8_t>::Plaintext>> result =
      ctx.eval_with_plaintexts(circ, pt_input);

  std::vector<uint8_t> exp_values = {241, 128};

  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << "- (" << std::to_string(pt_input[0][i])
              << ") = " << std::to_string(result[0][i]) << std::endl;
  }

  assert(result.front() == exp_values);
}
