#include <memory>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include "sheep/circuit-repo.hpp"
#include "circuit-test-util.hpp"
#include "sheep/context-seal-bfv.hpp"

typedef std::chrono::duration<double, std::micro> DurationT;

int main(void) {
  using namespace SHEEP;

  //// instantiate the Circuit Repository
  CircuitRepo cr;

  Circuit circ = cr.create_circuit(Gate::Multiply, 1);
  std::cout << circ;
  std::vector<DurationT> durations;
  ContextSealBFV<bool> ctx;

  /// test two 1s
  std::vector<std::vector<bool> > inputs = {{1}, {1}};
  std::vector<std::vector<bool> > result =
      ctx.eval_with_plaintexts(circ, inputs);
  assert(result.front()[0] == 1);
  std::cout << "  1 * 1 = " << std::to_string(result.front()[0]) << std::endl;
  /// test one of each
  inputs = {{0}, {1}};
  result = ctx.eval_with_plaintexts(circ, inputs);
  assert(result.front()[0] == 0);
  std::cout << " 0 * 1 = " << std::to_string(result.front()[0]) << std::endl;
  /// test both zeros
  inputs = {{0}, {0}};
  result = ctx.eval_with_plaintexts(circ, inputs);
  std::cout << " 0 * 0 = " << std::to_string(result.front()[0]) << std::endl;
  assert(result.front()[0] == 0);
  /// test for slots
  inputs = {{0, 1, 1}, {1, 0, 1}};
  result = ctx.eval_with_plaintexts(circ, inputs);
  assert(result[0][0] == 0);
  assert(result[0][1] == 0);
  assert(result[0][2] == 1);
}
