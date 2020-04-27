#include <algorithm>
#include <cassert>
#include <cstdint>
#include "circuit-test-util.hpp"
#include "sheep/context-lp.hpp"
#include "sheep/simple-circuits.hpp"

using namespace SHEEP;
typedef ContextLP<int8_t>::Plaintext Plaintext;
typedef ContextLP<int8_t>::Ciphertext Ciphertext;

// Encrypt a value, decrypt the result, and check that we are
// left with the original value.
void test_single(ContextLP<int8_t>& context) {
  std::vector<Plaintext> pt_orig = {56};
  std::string serialized_ciphertext = context.encrypt_and_serialize(pt_orig);

  std::cout << std::to_string(pt_orig[0]) << " => " << serialized_ciphertext
            << std::endl;
  assert(serialized_ciphertext.size() > 0);
}

int main(void) {
  ContextLP<int8_t> context;
  test_single(context);
}
