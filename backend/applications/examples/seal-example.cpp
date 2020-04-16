#include <memory>

#include "context-seal-bfv.hpp"

#include "circuit-repo.hpp"

using namespace SHEEP;

int main(void) {
  //// instantiate the Circuit Repository
  CircuitRepo cr;

  /// can either retrieve pre-build test circuits by name:

  Circuit C = cr.get_circuit_by_name("TestCircuit1");
  std::cout << C;

  //// or build a circuit with a specified depth of a specified gate

  // Circuit C2 = cr.create_circuit(Gate::Add, 3);
  // std::cout << C2;

  ContextSealBFV<int8_t> ctx;

  // ContextSealBFV<int8_t>::CircuitEvaluator run_circuit;
  // run_circuit = ctx.compile(C);

  std::vector<std::vector<int8_t>> plaintext_inputs = {{1}, {1}, {3}, {0}};
  std::list<ContextSealBFV<int8_t>::Ciphertext> ciphertext_inputs;

  for (std::vector<int8_t> pt : plaintext_inputs)
    ciphertext_inputs.push_back(ctx.encrypt(pt));

  std::list<ContextSealBFV<int8_t>::Ciphertext> ciphertext_outputs;
  using microsecond = std::chrono::duration<double, std::micro>;
  microsecond time = ctx.eval(C, ciphertext_inputs, ciphertext_outputs);

  std::vector<int8_t> plaintext_outputs;
  for (ContextSealBFV<int8_t>::Ciphertext ct : ciphertext_outputs) {
    int8_t pt = ctx.decrypt(ct).at(0);
    plaintext_outputs.push_back(pt);
    std::cout << "output: " << std::to_string(pt) << std::endl;
  }
  std::cout << "time was " << time.count() << " microseconds\n";
}
