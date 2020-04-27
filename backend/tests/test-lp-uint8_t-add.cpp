#include <algorithm>
#include <cassert>
#include <cstdint>
#include "sheep/circuit-repo.hpp"
#include "circuit-test-util.hpp"
#include "sheep/context-lp.hpp"
#include "sheep/simple-circuits.hpp"

typedef std::chrono::duration<double, std::micro> DurationT;

int main(void) {
  using namespace SHEEP;

  //// instantiate the Circuit Repository
  CircuitRepo cr;

  Circuit circ = cr.create_circuit(Gate::Add, 1);
  std::cout << circ;
  std::vector<DurationT> durations;
  ContextLP<uint8_t> ctx;
  ctx.set_parameter("NumSlots", 5);
  std::vector<std::vector<ContextLP<uint8_t>::Plaintext>> pt_input = {
      {15, 10, 10, 8, 80}, {22, 12, 0, 8, 80}};

  std::vector<std::vector<ContextLP<uint8_t>::Plaintext>> result =
      ctx.eval_with_plaintexts(circ, pt_input);

  std::vector<uint8_t> exp_values = {37, 22, 10, 16, 160};

  for (int i = 0; i < exp_values.size(); i++) {
    std::cout << std::to_string(pt_input[0][i]) << " + "
              << std::to_string(pt_input[1][i]) << " = "
              << std::to_string(result[0][i]) << std::endl;
  }

  assert(result.front() == exp_values);
}
