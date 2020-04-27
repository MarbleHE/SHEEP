#include <algorithm>
#include <cassert>
#include <cstdint>
#include "circuit-test-util.hpp"
#include "sheep/context-tfhe.hpp"
#include "sheep/simple-circuits.hpp"

using namespace SHEEP;
typedef ContextTFHE<bool>::Plaintext Plaintext;
typedef ContextTFHE<bool>::Ciphertext Ciphertext;

// Encrypt a value, decrypt the result, and check that we are
// left with the original value.
void test_single(ContextTFHE<bool>& context) {
  std::vector<Plaintext> pt_orig = {true, false};
  std::vector<Plaintext> pt_new = context.decrypt(context.encrypt(pt_orig));

  for (int i = 0; i < pt_orig.size(); i++) {
    std::cout << pt_orig[i] << " = " << pt_new[i] << std::endl;
    assert(pt_orig[i] == pt_new[i]);
  }


};

// // Similar to test_single, but using the container interface
// bool test_containers(ContextTFHE<bool>& context) {
// 	std::vector<Plaintext> ptxt_orig{1,1,0,0,1,0,0,1,0,0,0,0,1,1,1,1};
// 	std::vector<Plaintext> ptxt_expected(ptxt_orig);

// 	std::vector<Ciphertext> ctxt;
// 	encrypt(context, ptxt_orig.begin(), ptxt_orig.end(),
// std::back_inserter(ctxt));

// 	std::vector<Plaintext> ptxt_actual;
// 	decrypt(context, ctxt.begin(), ctxt.end(),
// std::back_inserter(ptxt_actual));

// 	return all_equal(ptxt_actual, ptxt_expected);
// }

int main(void) {
  ContextTFHE<bool> context;

  test_single(context);
}
