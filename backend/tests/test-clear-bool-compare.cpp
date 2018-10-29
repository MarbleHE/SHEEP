#include <cstdint>
#include <cassert>
#include "context-clear.hpp"
#include "circuit.hpp"
#include "circuit-test-util.hpp"
#include "simple-circuits.hpp"

using namespace SHEEP;

int main(void) {
  
	typedef std::vector<std::vector<ContextClear<bool>::Plaintext>> PtVec;
	
	Circuit circ = single_binary_gate_circuit(Gate::Compare);
  
	ContextClear<bool> ctx;
	
	assert(eval_encrypted_check_equal(ctx, circ, PtVec{{1, 0, 0, 1}, {0, 1, 0, 1}}, PtVec{{1, 0, 0, 0}}));
};