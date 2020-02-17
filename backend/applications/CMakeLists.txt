####################################################################################
## CLEAR
####################################################################################

add_executable(circuit-parser
        examples/circuit-parser.cpp ${Sheep_SOURCE_DIR}/src/circuit.cpp)

add_executable(circuit-example
        examples/circuit-example.cpp ${Sheep_SOURCE_DIR}/src/circuit.cpp)

add_test(NAME circuit-example COMMAND $<TARGET_FILE:circuit-example>)

add_executable(simple-circuit-maker
        utilities/simple-circuit-maker.cpp ${Sheep_SOURCE_DIR}/src/circuit.cpp)

add_executable(bitonic-sorting-circuit
        examples/bitonic-sorting-circuit.cpp
        ${Sheep_SOURCE_DIR}/src/circuit.cpp
        ${Sheep_SOURCE_DIR}/src/circuit-util.cpp
        ${Sheep_SOURCE_DIR}/src/simple-circuits.cpp)

add_executable(clear-context-example
        examples/clear-context-example.cpp ${Sheep_SOURCE_DIR}/src/circuit.cpp)

add_test(NAME clear-context-example COMMAND $<TARGET_FILE:clear-context-example>)

####################################################################################
## HElib
####################################################################################

if (HElib_FOUND)
    add_executable(helib-context-example
            examples/helib-context-example.cpp
            ${Sheep_SOURCE_DIR}/src/circuit.cpp)

    add_test(NAME helib-context-example COMMAND $<TARGET_FILE:helib-context-example>)

    target_link_libraries(helib-context-example
            ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})

    add_executable(bitonic-sort-helib
            examples/bitonic-sort-helib.cpp
            ${Sheep_SOURCE_DIR}/src/circuit.cpp
            ${Sheep_SOURCE_DIR}/src/circuit-util.cpp
            ${Sheep_SOURCE_DIR}/src/simple-circuits.cpp)

    target_link_libraries(bitonic-sort-helib
            ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})

    add_test(NAME bitonic-sort-helib COMMAND bitonic-sort-helib)

endif(HElib_FOUND)

if (TFHE_FOUND)
    add_executable(tfhe-example
            examples/tfhe-example.cpp
            ${Sheep_SOURCE_DIR}/src/circuit.cpp)

    add_test(NAME tfhe-example COMMAND $<TARGET_FILE:tfhe-example>)

    target_link_libraries(tfhe-example
            ${TFHE_LIBRARIES})


    add_executable(bitonic-sort-tfhe
            examples/bitonic-sort-tfhe.cpp
            ${Sheep_SOURCE_DIR}/src/circuit.cpp
            ${Sheep_SOURCE_DIR}/src/circuit-util.cpp
            ${Sheep_SOURCE_DIR}/src/simple-circuits.cpp)
    target_link_libraries(bitonic-sort-tfhe ${TFHE_LIBRARIES})

    add_test(NAME bitonic-sort-tfhe COMMAND bitonic-sort-tfhe)

endif(TFHE_FOUND)

if (SEAL_FOUND)
    #     add_executable(seal-example
    #                      examples/seal-example.cpp
    # 		     ${Sheep_SOURCE_DIR}/src/circuit.cpp)
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
            ${Sheep_SOURCE_DIR}/src/circuit.cpp)
    add_test(NAME lp-context-example COMMAND $<TARGET_FILE:lp-context-example>)
    target_link_libraries(lp-context-example
            ${LP_LIBRARIES} ${GMP_LIBRARIES})
    add_executable(lp-example
            examples/lpplain.cpp
            ${Sheep_SOURCE_DIR}/src/circuit.cpp)

    add_test(NAME lp-example COMMAND $<TARGET_FILE:lp-example>)

    target_link_libraries(lp-example
            ${LP_LIBRARIES} ${GMP_LIBRARIES})

endif(LP_FOUND)

####################################################################################
## SHEEP-CL
##
## The "sheep-cl" executable, to run a specified context on a given circuit.
####################################################################################

add_executable(sheep-cl utilities/sheep-cl.cpp ${Sheep_SOURCE_DIR}/src/circuit.cpp)

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
        ${Sheep_SOURCE_DIR}/src/circuit.cpp
        ${Sheep_SOURCE_DIR}/src/circuit-util.cpp
        ${Sheep_SOURCE_DIR}/src/simple-circuits.cpp)

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


####################################################################################
## RPN Tests
####################################################################################
if(NOT APP_TESTING)
    set(APP_TESTING FALSE)
endif()
if(APP_TESTING)
    # Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                     ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
      include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    # Now simply link against gtest or gtest_main as needed. Eg
    add_executable(tests rpn/tests/tests.cpp
            rpn/token.cpp
            rpn/rpn.cpp
            rpn/intop.cpp
            rpn/unop.cpp
            rpn/binop.cpp rpn/include/avlibs.hpp)
    target_include_directories(tests PUBLIC
            rpn/include
            ../../include)
    target_link_libraries(tests gtest_main)

    target_link_libraries(tests sheep)

    add_test(NAME rpn_tests COMMAND tests)
endif()

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

