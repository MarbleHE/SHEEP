#include <cassert>
#include <cstdint>
#include "circuit-test-util.hpp"
#include "sheep/circuit.hpp"
#include "sheep/context-clear.hpp"
#include "sheep/simple-circuits.hpp"

using namespace SHEEP;

int main(void) {
  typedef std::vector<std::vector<ContextClear<uint8_t>::Plaintext>> PtVec;

  Circuit circ = single_binary_gate_circuit(Gate::Compare);

  ContextClear<uint8_t> ctx;
  ctx.set_parameter("NumSlots", 1);
  assert(eval_encrypted_check_equal(ctx, circ, PtVec{{1}, {0}}, PtVec{{1}}));
  ctx.set_parameter("NumSlots", 2);
  assert(eval_encrypted_check_equal(ctx, circ, PtVec{{0, 1}, {0, 1}},
                                    PtVec{{0, 0}}));
};
