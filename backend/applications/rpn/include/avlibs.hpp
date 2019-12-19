//
// Created by mario on 19.12.19.
//
#include <iostream>

#ifndef SHEEP_AVLIBS_HPP
#define SHEEP_AVLIBS_HPP


void printlibs() {

    bool uninstalled = false;
    std::cout << "Available libraries:" << std::endl;
    std::cout << "0: Plaintext" << std::endl;

#ifdef HAVE_HElib
    std::cout << "1: HElib_F2" << std::endl;
#else
    uninstalled = true;
#endif

#ifdef HAVE_LP
    std::cout << "2: LP" << std::endl;
#else
    uninstalled = true;
#endif

#ifdef HAVE_PALISADE
    std::cout << "3: Palisade" << std::endl;
#else
    uninstalled = true;
#endif

#ifdef HAVE_SEAL_BFV
    std::cout << "4: SealBFV" << std::endl;
#else
    uninstalled = true;
#endif

#ifdef HAVE_SEAL_CKKS
    std::cout << "5: SealCKKS" << std::endl;
#else
    uninstalled = true;
#endif

#ifdef HAVE_TFHE
    std::cout << "6: TFHE" << std::endl;
#else
    uninstalled = true;
#endif

    if (uninstalled) {
        std::cout << "The following libraries are NOT available and need to be installed correctly: " << std::endl;
#ifndef HAVE_HElib
        std::cout << "1: HElib_F2" << std::endl;
#endif

#ifndef HAVE_LP
        std::cout << "2: LP" << std::endl;
#endif

#ifndef HAVE_PALISADE
        std::cout << "3: Palisade" << std::endl;
#endif

#ifndef HAVE_SEAL_BFV
        std::cout << "4: SealBFV" << std::endl;
#endif

#ifndef HAVE_SEAL_CKKS
        std::cout << "5: SealCKKS" << std::endl;
#endif

#ifndef HAVE_TFHE
        std::cout << "6: TFHE" << std::endl;
#endif
    }

}


#endif //SHEEP_AVLIBS_HPP
