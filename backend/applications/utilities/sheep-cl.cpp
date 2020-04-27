////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/// This is the main executable that is used by the frontend to:
/// 1) run a benchmark test on a specified circuit file
///      ./benchmark <circuit_file> <context_name> <input_type> <inputs_file>
///      [<params_file>]
/// 2) print out the paramaters for a given context
///      ./benchmark PARAMS <context_name> <input_type> [<params_file>]
/// 3) print out the key and ciphertext sizes for a given context.
///      ./benchmark SIZES <context_name> <input_type> [<params_file>]
///

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <map>
#include <memory>

#include "sheep/context-clear.hpp"
#ifdef HAVE_HElib
#include "context-helib.hpp"
#endif
#ifdef HAVE_TFHE
#include "context-tfhe.hpp"
#endif
#ifdef HAVE_SEAL
#include "context-seal.hpp"
#endif
#ifdef HAVE_LP
#include "context-lp.hpp"
#endif

using namespace SHEEP;

typedef std::chrono::duration<double, std::micro> DurationT;

template <typename PlaintextT>
std::unique_ptr<BaseContext<PlaintextT>> make_context(
    std::string context_type, std::string context_params = "") {
  if (context_type == "Clear") {
    return std::make_unique<ContextClear<PlaintextT>>();
#ifdef HAVE_HElib
  } else if (context_type == "HElib_F2") {
    auto ctx = std::make_unique<ContextHElib_F2<PlaintextT>>();
    if (context_params.length() > 0) ctx->read_params_from_file(context_params);
    return ctx;
  } else if (context_type == "HElib_Fp") {
    auto ctx = std::make_unique<ContextHElib_Fp<PlaintextT>>();
    if (context_params.length() > 0) ctx->read_params_from_file(context_params);
    return ctx;
#endif
#ifdef HAVE_TFHE
  } else if (context_type == "TFHE") {
    auto ctx = std::make_unique<ContextTFHE<PlaintextT>>();
    if (context_params.length() > 0) ctx->read_params_from_file(context_params);
    return ctx;
#endif
#ifdef HAVE_SEAL
  } else if (context_type == "SEAL") {
    auto ctx = std::make_unique<ContextSeal<PlaintextT>>();
    if (context_params.length() > 0) ctx->read_params_from_file(context_params);
    return ctx;
#endif
#ifdef HAVE_LP
  } else if (context_type == "LP") {
    auto ctx = std::make_unique<ContextLP<PlaintextT>>();
    if (context_params.length() > 0) ctx->read_params_from_file(context_params);
    return ctx;
#endif
  } else {
    throw std::runtime_error("Unknown context requested");
  }
}

template <typename T>
std::map<std::string, T> read_inputs_file(std::string filename) {
  std::vector<T> inputs_list;
  std::map<std::string, T> inputs_map;
  std::ifstream inputstream(filename);
  if (inputstream.bad()) {
    std::cout << "Empty or non-existent input file" << std::endl;
  }

  /// loop over all lines in the input file
  std::string line;
  while (std::getline(inputstream, line)) {
    /// remove comments (lines starting with #) and empty lines
    int found = line.find_first_not_of(" \t");
    if (found != std::string::npos) {
      if (line[found] == '#') continue;

      /// split up by whitespace
      std::string buffer;
      std::vector<std::string> tokens;
      std::stringstream ss(line);
      while (ss >> buffer) tokens.push_back(buffer);

      if (tokens.size() == 2) {  /// assume we have param_name param_value
        std::string input_name = (std::string)(tokens[0]);
        long input_val_long = stol(tokens[1]);
        T input_val = (T)(input_val_long);
        inputs_map.insert({input_name, input_val});
        inputs_list.push_back(input_val);
      }
    }
  }  // end of loop over lines
  return inputs_map;
}

template <typename PlaintextT>
bool check_correct(std::vector<std::vector<PlaintextT>> test_results,
                   std::vector<std::vector<PlaintextT>> plaintext_results) {
  if (test_results.size() != plaintext_results.size())
    throw std::runtime_error("outputs have different sizes");
  if (test_results.size() == 0) throw std::runtime_error("zero length output");

  bool all_correct = true;

  auto test_iter = test_results.begin();
  auto plaintext_iter = plaintext_results.begin();

  while (test_iter != test_results.end()) {
    all_correct &= (*test_iter == *plaintext_iter);
    test_iter++;
    plaintext_iter++;
  }

  return all_correct;
}

template <typename PlaintextT>
void print_outputs(Circuit C, std::vector<std::vector<PlaintextT>> test_results,
                   std::vector<std::vector<PlaintextT>> cleartext_results,
                   DurationContainer& durations) {
  std::cout << std::endl << "===============" << std::endl;
  std::cout << "=== RESULTS ===" << std::endl << std::endl;
  std::cout << "== Processing times: ==" << std::endl;
  std::cout << "setup: " << durations.first[0].count() << std::endl;
  std::cout << "encryption: " << durations.first[1].count() << std::endl;
  std::cout << "circuit_evaluation: " << durations.first[2].count() << std::endl;
  std::cout << "decryption: " << durations.first[3].count() << std::endl;
  std::cout << std::endl;

  std::vector<Wire> circuit_outputs = C.get_outputs();
  if (circuit_outputs.size() != test_results.size())
    throw std::runtime_error("outputs have different sizes");
  std::cout << "== Output values ==" << std::endl;

  auto test_iter = test_results.begin();
  auto wire_iter = circuit_outputs.begin();

  while (test_iter != test_results.end()) {
    std::cout << wire_iter->get_name() << ": " << std::endl;

    // TODO: fix the to_string to print the list!
    // std::to_string(*test_iter)<<std::endl;
    wire_iter++;
    test_iter++;
  }
  //// comparison with the same circuit+inputs evaluated in clear context
  std::cout << std::endl
            << "== Check against cleartext context ==" << std::endl;

  bool matches = check_correct<PlaintextT>(test_results, cleartext_results);

  if (matches)
    std::cout << "Cleartext check passed OK" << std::endl;
  else
    std::cout << "Cleartext check failed" << std::endl;
  std::cout << std::endl << "==== END RESULTS ===" << std::endl;
}

template <typename T>
std::vector<std::vector<T>> match_inputs_to_circuit(
    Circuit C, std::map<std::string, T> inputs_map) {
  std::vector<std::vector<T>> ordered_inputs;

  for (auto input : C.get_inputs()) {
    if (inputs_map.find(input.get_name()) != inputs_map.end()) {
      ordered_inputs.push_back({inputs_map.find(input.get_name())->second});
    }
  }
  return ordered_inputs;
}

template <typename PlaintextT>
void param_print(std::string context_name, std::string parameter_file = "") {
  std::unique_ptr<BaseContext<PlaintextT>> test_ctx =
      make_context<PlaintextT>(context_name, parameter_file);
  std::cout << " made context for " << context_name << std::endl;
  test_ctx->print_parameters();
}

template <typename PlaintextT>
bool benchmark_run(std::string context_name, std::string parameter_file,
                   Circuit C, std::string input_filename,
                   EvaluationStrategy eval_strategy) {
  std::vector<DurationT> totalDurations;
  typedef std::chrono::duration<double, std::micro> microsecond;
  typedef std::chrono::high_resolution_clock high_res_clock;
  auto setup_start_time = high_res_clock::now();

  std::map<std::string, DurationT> perGateDurations;
  DurationContainer durations = std::make_pair(totalDurations, perGateDurations);

  std::unique_ptr<BaseContext<PlaintextT>> test_ctx =
      make_context<PlaintextT>(context_name, parameter_file);
  std::cout << " === Made context " << context_name << std::endl;
  auto setup_end_time = high_res_clock::now();
  durations.first.push_back(microsecond(setup_end_time - setup_start_time));

  std::unique_ptr<BaseContext<PlaintextT>> clear_ctx =
      make_context<PlaintextT>("Clear");

  // read in inputs from input_filename
  std::map<std::string, PlaintextT> inputs =
      read_inputs_file<PlaintextT>(input_filename);

  std::cout << " === Read inputs file - found " << inputs.size() << " values."
            << std::endl;
  std::vector<std::vector<PlaintextT>> ordered_inputs =
      match_inputs_to_circuit(C, inputs);
  std::cout << " === Matched inputs from file with circuit inputs" << std::endl;

  std::vector<std::vector<PlaintextT>> result_bench =
      test_ctx->eval_with_plaintexts(C, ordered_inputs, durations,
                                     eval_strategy);

  std::cout << " === Ran benchmark test. " << std::endl;
  test_ctx->print_parameters();
  test_ctx->print_sizes();
  std::vector<std::vector<PlaintextT>> result_clear =
    clear_ctx->eval_with_plaintexts(C, ordered_inputs);

  print_outputs(C, result_bench, result_clear, durations);

  return true;
}

int main(int argc, const char** argv) {
  if (argc < 3) {
    std::cout << "Usage: \n ./benchmark  <circuit_file> <context_name> "
                 "<input_type> <inputs_file> <eval_strategy> [<params_file>]"
              << std::endl;
    std::cout << "OR: \n ./benchmark PARAMS  <context_name> <input_type> "
                 "[<params_file>]"
              << std::endl;
    return 0;
  }

  if (strncmp(argv[1], "PARAMS", 5) == 0) {
    std::string context_name = argv[2];
    std::string input_type = argv[3];
    std::string param_file = "";
    if (argc == 5) param_file = argv[4];
    if (input_type == "bool")
      param_print<bool>(context_name, param_file);
    else if (input_type == "int8_t")
      param_print<int8_t>(context_name, param_file);
    else if (input_type == "uint8_t")
      param_print<uint8_t>(context_name, param_file);
    else if (input_type == "int16_t")
      param_print<int16_t>(context_name, param_file);
    else if (input_type == "uint16_t")
      param_print<uint16_t>(context_name, param_file);
    else if (input_type == "int32_t")
      param_print<int32_t>(context_name, param_file);
    else if (input_type == "uint32_t")
      param_print<uint32_t>(context_name, param_file);
    return 0;
  }

  /// read the circuit
  std::ifstream input_circuit(argv[1]);
  Circuit C;
  input_circuit >> C;
  std::cout << C;

  /// read the other input args
  std::string context_name = argv[2];
  std::string input_type = argv[3];
  std::string inputs_file = argv[4];
  std::string eval_strategy_string = argv[5];
  std::string parameter_file = "";
  if (argc == 7) parameter_file = argv[6];

  EvaluationStrategy eval_strategy;
  std::string eval_strategy_used;
  if (eval_strategy_string == "parallel") {
    eval_strategy = EvaluationStrategy::parallel;
    eval_strategy_used = "parallel";
  } else {
    eval_strategy = EvaluationStrategy::serial;
    eval_strategy_used = "serial";
  }

  std::cout << "======  Running benchmark test with:  =======" << std::endl
            << "Circuit file: " << argv[1] << std::endl
            << "Context: " << context_name << std::endl
            << "Input type: " << input_type << std::endl
            << "Inputs_file " << inputs_file << std::endl
            << "Evaluation strategy: " << eval_strategy_used << std::endl
            << "Parameter file: " << parameter_file << std::endl;

  /// run the benchmark
  bool isOK = false;
  if (input_type == "bool") {
    isOK = benchmark_run<bool>(context_name, parameter_file, C, inputs_file,
                               eval_strategy);
  } else if (input_type == "uint8_t") {
    isOK = benchmark_run<uint8_t>(context_name, parameter_file, C, inputs_file,
                                  eval_strategy);
  } else if (input_type == "int8_t") {
    isOK = benchmark_run<int8_t>(context_name, parameter_file, C, inputs_file,
                                 eval_strategy);
  } else if (input_type == "uint16_t") {
    isOK = benchmark_run<uint16_t>(context_name, parameter_file, C, inputs_file,
                                   eval_strategy);
  } else if (input_type == "int16_t") {
    isOK = benchmark_run<int16_t>(context_name, parameter_file, C, inputs_file,
                                  eval_strategy);
  } else if (input_type == "uint32_t") {
    isOK = benchmark_run<int32_t>(context_name, parameter_file, C, inputs_file,
                                  eval_strategy);
  } else if (input_type == "int32_t") {
    isOK = benchmark_run<int32_t>(context_name, parameter_file, C, inputs_file,
                                  eval_strategy);
  }

  return isOK;
}
