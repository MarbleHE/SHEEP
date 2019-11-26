####################################################################################
## CLEAR
####################################################################################

add_executable(rpn_main
  rpn/rpn_main.cpp
  ${CMAKE_SOURCE_DIR}/src/circuit.cpp
  ${CMAKE_SOURCE_DIR}/src/circuit-util.cpp
  ${CMAKE_SOURCE_DIR}/src/simple-circuits.cpp)
target_sources(rpn_main
        PRIVATE
	rpn/token.cpp
        rpn/rpn.cpp
)
target_link_libraries(rpn_main ${LP_LIBRARIES} ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES} ${TFHE_LIBRARIES})

####################################################################################
## HElib
####################################################################################

if (HElib_FOUND)
  

endif(HElib_FOUND)

 if (TFHE_FOUND)
    
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
   

 endif(LP_FOUND)

