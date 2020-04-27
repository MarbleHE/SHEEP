#include <cassert>

#include "sheep/circuit-repo.hpp"
#include "sheep/circuit-util.hpp"
#include "sheep/circuit.hpp"

#include "sheep/all_equal.hpp"
#include "sheep/context-clear.hpp"

int main(void) {
  CircuitRepo cr;
  NameGenerator names;

  Circuit C = cr.get_circuit_by_name("TestCircuit3");
  std::cout << C << std::endl;

  std::cout << par(C, C) << std::endl;

  //	assert(false);
}
