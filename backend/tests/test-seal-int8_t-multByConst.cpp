#include <memory>

#include <cstdint>
#include <cassert>
#include <algorithm>
#include "context-seal.hpp"
#include "circuit-repo.hpp"
#include "circuit-test-util.hpp"


int main(void) {

  using namespace SHEEP;
  
	ContextSeal<int8_t> ctx;

	/// test small postitive numbers
	ContextSeal<int8_t>::Plaintext pt_input = 55;
	ContextSeal<int8_t>::Ciphertext ct = ctx.encrypt(pt_input);
	long const_val = 2;
	ContextSeal<int8_t>::Ciphertext ct_out = ctx.MultByConstant(ct, const_val);
	ContextSeal<int8_t>::Plaintext pt_out = ctx.decrypt(ct_out);
	std::cout<<"D( E(55) * 2) = "<<std::to_string(pt_out)<<std::endl;
	assert(pt_out == 110);
	/// small negative numbers
	pt_input = 5;
	ct = ctx.encrypt(pt_input);
	const_val = -4;
	ct_out = ctx.MultByConstant(ct, const_val);
	pt_out = ctx.decrypt(ct_out);
	std::cout<<"D( E(5) * -4) = "<<std::to_string(pt_out)<<std::endl;
	assert(pt_out == -20);
	/// out-of-range negative
	pt_input = -125;
	ct = ctx.encrypt(pt_input);
	const_val = 21;
	ct_out = ctx.MultByConstant(ct, const_val);
	pt_out = ctx.decrypt(ct_out);
	std::cout<<"D( E(-125) * 21) = "<<std::to_string(pt_out)<<std::endl;
	assert(pt_out == -65);
	/// out-of-range positive
	pt_input = 120;
	ct = ctx.encrypt(pt_input);
	const_val = 121;
	ct_out = ctx.MultByConstant(ct, const_val);
	pt_out = ctx.decrypt(ct_out);
	std::cout<<"D( E(120) * 121) = "<<std::to_string(pt_out)<<std::endl;
	assert(pt_out == -72);

	
	

}