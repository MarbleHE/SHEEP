#include <memory>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include "sheep/circuit-repo.hpp"
#include "circuit-test-util.hpp"
#include "sheep/context-helib.hpp"

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
  ContextHElib_Fp<int8_t> ctx;

  // typedef std::vector<std::vector<ContextHElib_Fp<int8_t>::Plaintext>> PtVec;

  // assert(eval_encrypted_check_equal(ctx, circ, PtVec{{1}, {0}}, PtVec{{1}}));

  // std::vector<std::vector<std::vector<ContextHElib_Fp<int8_t>::Plaintext>>>
  // pt_input = {{{15}, {22}}};//, {10, -12}, {-12, -12}, {-81, -80}};

  // std::vector<std::vector<ContextHElib_Fp<int8_t>::Plaintext>> result =
  // ctx.eval_with_plaintexts(circ, pt_input);

  // std::vector<int8_t> exp_values = {1};//, 1, 0, 0};

  // for (int i = 0; i < exp_values.size(); i++) {
  //   std::cout << std::to_string(pt_input[0][i]) << " + " <<
  //   std::to_string(pt_input[1][i]) << " = " << std::to_string(result[0][i])
  //   << std::endl; assert(result.front()[i] == exp_values[i]);
  // }
}
