####################################################################################
## CLEAR
####################################################################################

add_executable(circuit-parser
        examples/circuit-parser.cpp ${CMAKE_SOURCE_DIR}/src/circuit.cpp)

add_executable(circuit-example
        examples/circuit-example.cpp ${CMAKE_SOURCE_DIR}/src/circuit.cpp)

add_test(NAME circuit-example COMMAND $<TARGET_FILE:circuit-example>)

add_executable(simple-circuit-maker
        utilities/simple-circuit-maker.cpp ${CMAKE_SOURCE_DIR}/src/circuit.cpp)

add_executable(bitonic-sorting-circuit
        examples/bitonic-sorting-circuit.cpp
        ${CMAKE_SOURCE_DIR}/src/circuit.cpp
        ${CMAKE_SOURCE_DIR}/src/circuit-util.cpp
        ${CMAKE_SOURCE_DIR}/src/simple-circuits.cpp)

add_executable(clear-context-example
        examples/clear-context-example.cpp ${CMAKE_SOURCE_DIR}/src/circuit.cpp)

add_test(NAME clear-context-example COMMAND $<TARGET_FILE:clear-context-example>)

####################################################################################
## HElib
####################################################################################

if (HElib_FOUND)
    add_executable(helib-context-example
            examples/helib-context-example.cpp
            ${CMAKE_SOURCE_DIR}/src/circuit.cpp)

    add_test(NAME helib-context-example COMMAND $<TARGET_FILE:helib-context-example>)

    target_link_libraries(helib-context-example
            ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})

    add_executable(bitonic-sort-helib
            examples/bitonic-sort-helib.cpp
            ${CMAKE_SOURCE_DIR}/src/circuit.cpp
            ${CMAKE_SOURCE_DIR}/src/circuit-util.cpp
            ${CMAKE_SOURCE_DIR}/src/simple-circuits.cpp)

    target_link_libraries(bitonic-sort-helib
            ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})

    add_test(NAME bitonic-sort-helib COMMAND bitonic-sort-helib)

endif(HElib_FOUND)

if (TFHE_FOUND)
    add_executable(tfhe-example
            examples/tfhe-example.cpp
            ${CMAKE_SOURCE_DIR}/src/circuit.cpp)

    add_test(NAME tfhe-example COMMAND $<TARGET_FILE:tfhe-example>)

    target_link_libraries(tfhe-example
            ${TFHE_LIBRARIES})


    add_executable(bitonic-sort-tfhe
            examples/bitonic-sort-tfhe.cpp
            ${CMAKE_SOURCE_DIR}/src/circuit.cpp
            ${CMAKE_SOURCE_DIR}/src/circuit-util.cpp
            ${CMAKE_SOURCE_DIR}/src/simple-circuits.cpp)
    target_link_libraries(bitonic-sort-tfhe ${TFHE_LIBRARIES})

    add_test(NAME bitonic-sort-tfhe COMMAND bitonic-sort-tfhe)

endif(TFHE_FOUND)

if (SEAL_FOUND)
    #     add_executable(seal-example
    #                      examples/seal-example.cpp
    # 		     ${CMAKE_SOURCE_DIR}/src/circuit.cpp)
    #     add_test(NAME seal-example COMMAND $<TARGET_FILE:seal-example>)
    #     target_link_libraries(seal-example
    #                             ${SEAL_LIBRARIES})

    #     add_executable(seal-param-selector
    #                      utilities/seal-param-selector.cpp)
    #     target_link_libraries(seal-param-selector
    #                             ${SEAL_LIBRARIES}
    # 			    pthread)
endif(SEAL_FOUND)


if (LP_FOUND)
    add_executable(lp-context-example
            examples/lp-context-example.cpp
            ${CMAKE_SOURCE_DIR}/src/circuit.cpp)
    add_test(NAME lp-context-example COMMAND $<TARGET_FILE:lp-context-example>)
    target_link_libraries(lp-context-example
            ${LP_LIBRARIES} ${GMP_LIBRARIES})
    add_executable(lp-example
            examples/lpplain.cpp
            ${CMAKE_SOURCE_DIR}/src/circuit.cpp)

    add_test(NAME lp-example COMMAND $<TARGET_FILE:lp-example>)

    target_link_libraries(lp-example
            ${LP_LIBRARIES} ${GMP_LIBRARIES})

endif(LP_FOUND)

####################################################################################
## SHEEP-CL
##
## The "sheep-cl" executable, to run a specified context on a given circuit.
####################################################################################

add_executable(sheep-cl utilities/sheep-cl.cpp ${CMAKE_SOURCE_DIR}/src/circuit.cpp)

if (HElib_FOUND)
    target_link_libraries(sheep-cl ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})
endif(HElib_FOUND)

if (TFHE_FOUND)
    target_link_libraries(sheep-cl ${TFHE_LIBRARIES})
endif(TFHE_FOUND)

if (SEAL_FOUND)
    target_link_libraries(sheep-cl ${SEAL_LIBRARIES})
endif(SEAL_FOUND)

if (LP_FOUND)
    target_link_libraries(sheep-cl ${LP_LIBRARIES} ${GMP_LIBRARIES})
endif(LP_FOUND)

if (PALISADE_FOUND)
    target_link_libraries(sheep-cl ${PALISADE_LIBRARIES})
endif(PALISADE_FOUND)

add_test(NAME sheep-cl COMMAND $<TARGET_FILE:sheep-cl>)


####################################################################################
## SHEEP
##
## SHEEP as a static library to compile against
####################################################################################

add_library(sheep
        ${CMAKE_SOURCE_DIR}/src/circuit.cpp
        ${CMAKE_SOURCE_DIR}/src/circuit-util.cpp
        ${CMAKE_SOURCE_DIR}/src/simple-circuits.cpp)

if (HElib_FOUND)
    target_link_libraries(sheep ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})
endif(HElib_FOUND)

if (TFHE_FOUND)
    target_link_libraries(sheep ${TFHE_LIBRARIES})
endif(TFHE_FOUND)

if (SEAL_FOUND)
    target_link_libraries(sheep ${SEAL_LIBRARIES})
endif(SEAL_FOUND)

if (LP_FOUND)
    target_link_libraries(sheep ${LP_LIBRARIES} ${GMP_LIBRARIES})
endif(LP_FOUND)

if (PALISADE_FOUND)
    target_link_libraries(sheep ${PALISADE_LIBRARIES})
endif(PALISADE_FOUND)




####################################################################################
## RPN
##
## A Calculator that translates between Revers Polish Notation and Sheep Circuits
####################################################################################

add_executable(rpn
        rpn/rpn_main.cpp
        rpn/token.cpp
        rpn/rpn.cpp
        rpn/intop.cpp
        rpn/unop.cpp
        rpn/binop.cpp rpn/include/avlibs.hpp)

target_include_directories(rpn PUBLIC
        rpn/include
        ../../include)

target_link_libraries(rpn sheep)

if (HElib_FOUND)
    target_link_libraries(rpn ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})
endif(HElib_FOUND)

if (TFHE_FOUND)
    target_link_libraries(rpn ${TFHE_LIBRARIES})
endif(TFHE_FOUND)

if (SEAL_FOUND)
    target_link_libraries(rpn ${SEAL_LIBRARIES})
endif(SEAL_FOUND)

if (LP_FOUND)
    target_link_libraries(rpn ${LP_LIBRARIES} ${GMP_LIBRARIES})
endif(LP_FOUND)

if (PALISADE_FOUND)
    target_link_libraries(rpn ${PALISADE_LIBRARIES})
endif(PALISADE_FOUND)

add_subdirectory(rpn/tests)