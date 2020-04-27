#include <memory>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include "sheep/circuit-repo.hpp"
#include "circuit-test-util.hpp"
#include "sheep/context-helib.hpp"

int main(void) {
  using namespace SHEEP;

  ContextHElib_F2<bool> ctx;

  /// test small postitive numbers
  ContextHElib_F2<bool>::Plaintext pt_input = 1;
  ContextHElib_F2<bool>::Ciphertext ct = ctx.encrypt(pt_input);
  long const_val = 1;
  ContextHElib_F2<bool>::Ciphertext ct_out = ctx.MultByConstant(ct, const_val);
  ContextHElib_F2<bool>::Plaintext pt_out = ctx.decrypt(ct_out);
  std::cout << "D( E(1) * 1) = " << std::to_string(pt_out) << std::endl;
  assert(pt_out == 1);
  /// 0+1
  pt_input = 0;
  ct = ctx.encrypt(pt_input);
  const_val = -1;
  ct_out = ctx.MultByConstant(ct, const_val);
  pt_out = ctx.decrypt(ct_out);
  std::cout << "D( E(0) * 1) = " << std::to_string(pt_out) << std::endl;
  assert(pt_out == 0);
}
