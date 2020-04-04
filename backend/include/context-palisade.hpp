#ifndef CONTEXT_PALISADE_HPP
#define CONTEXT_PALISADE_HPP

#include <cstdint>
#include <cmath>
#include <sstream>
#include <type_traits>

#define RLWE RLWE_Palisade
#include <palisade.h>
#include <cryptocontexthelper.h>
#undef RLWE

#include "bits.hpp"
#include "circuit.hpp"
#include "context.hpp"

namespace SHEEP {
  template <typename PlaintextT>
  class ContextPalisade :
    public Context<PlaintextT, lbcrypto::Ciphertext<lbcrypto::DCRTPoly> >
  {
  protected:
    std::string m_paramSetName;
    lbcrypto::CryptoContext<lbcrypto::DCRTPoly> m_PalisadeContext;
    lbcrypto::LPKeyPair<lbcrypto::DCRTPoly> m_keyPair;
    long m;
  public:
    typedef PlaintextT Plaintext;
    typedef lbcrypto::Ciphertext<lbcrypto::DCRTPoly> Ciphertext;
    typedef std::int64_t Plaintext64;

    ContextPalisade(int plaintextSize, int slotSize, int nMults, int multDepth)
    {
        int plaintext_mod = pow(2,plaintextSize); //TODO: Automatically select so that primeModulus/plaintext_mod is an int.
      m_PalisadeContext = lbcrypto::CryptoContextFactory<lbcrypto::DCRTPoly>::genCryptoContextBFVrns(
              plaintext_mod, lbcrypto::HEStd_128_classic, 3.6, 0, (int) ceil(log2(nMults)), 0, OPTIMIZED, multDepth
              );
      m_PalisadeContext->Enable(ENCRYPTION);
      m_PalisadeContext->Enable(SHE);
      m_keyPair = m_PalisadeContext->KeyGen();
      m_PalisadeContext->EvalSumKeyGen(m_keyPair.secretKey);
      m_PalisadeContext->EvalMultKeyGen(m_keyPair.secretKey);

      this->m_nslots = slotSize; //TODO: Put correct value. slotSize is virtual slot size required by computation. Instead, actual slot size of the scheme needs to be here for padding.
    }

    void configure() { /* nothing to do */ }

    Ciphertext encrypt(std::vector<Plaintext> p) {
      std::vector<Plaintext64> p64(this->get_num_slots(), (Plaintext64)0);
      for (size_t i = 0; i < this->get_num_slots(); i++) {
        p64[i] = p[i % p.size()];
      }

      lbcrypto::Plaintext pt = m_PalisadeContext->MakePackedPlaintext(p64);
      return m_PalisadeContext->Encrypt(m_keyPair.publicKey, pt);
    }

    std::vector<Plaintext> decrypt(Ciphertext ct) {
      lbcrypto::Plaintext pt;
      m_PalisadeContext->Decrypt(m_keyPair.secretKey, ct, &pt);
      std::vector<Plaintext64> p64 = pt->GetPackedValue();
      std::vector<Plaintext> p(this->get_num_slots(), (Plaintext)0);
      if (std::is_same<Plaintext, bool>::value) {
        for (size_t i = 0; i < this->get_num_slots(); i++) {
          p[i] = (p64[i] % 2);
        }
      } else {
        for (size_t i = 0; i < this->get_num_slots(); i++) {
          p[i] = p64[i];
        }
      }
      return p;
    }

    Ciphertext Add(Ciphertext a, Ciphertext b) {
      return m_PalisadeContext->EvalAdd(a, b);
    }

    Ciphertext Multiply(Ciphertext a, Ciphertext b) {
      return m_PalisadeContext->EvalMult(a, b);
    }

    Ciphertext Subtract(Ciphertext a, Ciphertext b) {
      return m_PalisadeContext->EvalSub(a, b);
    }

    Ciphertext Negate(Ciphertext a) {
      return m_PalisadeContext->EvalNegate(a);
    }
  };


  // Dummy specializations to deal with types that Palisade doesn't support
  template <>
  class ContextPalisade<double> : public Context<double, lbcrypto::Ciphertext<lbcrypto::DCRTPoly> > {
  public:
    typedef double Plaintext;
    typedef lbcrypto::Ciphertext<lbcrypto::DCRTPoly>  Ciphertext;

    ContextPalisade() {
      throw InputTypeNotSupported();
    }
    Ciphertext encrypt(std::vector<Plaintext> pt) {
      throw InputTypeNotSupported();
    }
    std::vector<Plaintext> decrypt(Ciphertext ct) {
      throw InputTypeNotSupported();
    }
  };

  template <>
  class ContextPalisade<std::complex<double> >: public Context<std::complex<double>, lbcrypto::Ciphertext<lbcrypto::DCRTPoly>> {
  public:
    typedef std::complex<double> Plaintext;
    typedef lbcrypto::Ciphertext<lbcrypto::DCRTPoly>  Ciphertext;

    ContextPalisade() {
      throw InputTypeNotSupported();
    }
    Ciphertext encrypt(std::vector<Plaintext> pt) {
      throw InputTypeNotSupported();
    }
    std::vector<Plaintext> decrypt(Ciphertext ct) {
      throw InputTypeNotSupported();
    }
  };


}

#endif
