cmake_minimum_required (VERSION 2.6)

project (skr)

#
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake -DCMAKE_BUILD_TYPE=Release ..
#
#message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
#IF(CMAKE_BUILD_TYPE MATCHES Debug)
#  message("Debug build.")
#  set(CMAKE_CXX_FLAGS "-g -O0 -Wall")
#ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
#  message("Release build.")
#  set(CMAKE_CXX_FLAGS "-O2 -Wall")
#ELSE()
#  message("Some other build type.")
#ENDIF()


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/") 
message("Debug build.")
set(CMAKE_CXX_FLAGS "-g -O0 -Wall")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER_ARG1 "-std=c++0x ")
set(CHAIN_SRC_ROOT "/Users/stefan/git/skr")
set(CHAIN_NAME "bitcoin")
set(MAKE_JOBS "-j2")

set(BITCOIN_LIBRARIES
  "${CHAIN_SRC_ROOT}/src/libbitcoin_common.a"
  "${CHAIN_SRC_ROOT}/src/libbitcoin_consensus.a"
  "${CHAIN_SRC_ROOT}/src/libbitcoin_util.a"
  "${CHAIN_SRC_ROOT}/src/crypto/libbitcoin_crypto_base.a")

list(GET BITCOIN_LIBRARIES 0 BITCOIN_LIBRARIE)
if (NOT EXISTS ${BITCOIN_LIBRARIE})
  message(STATUS "building ${CHAIN_NAME}...")
  execute_process(WORKING_DIRECTORY "${CHAIN_SRC_ROOT}" COMMAND ./autogen.sh)
  execute_process(WORKING_DIRECTORY "${CHAIN_SRC_ROOT}" COMMAND ./configure --with-gui=no --disable-wallet --disable-tests --disable-bench)
  execute_process(WORKING_DIRECTORY "${CHAIN_SRC_ROOT}" COMMAND make ${MAKE_JOBS})
endif()

message(STATUS "Reach here")

foreach(BITCOIN_LIBRARIE ${BITCOIN_LIBRARIES})
  if (NOT EXISTS ${BITCOIN_LIBRARIE})
    message(FATAL_ERROR "${BITCOIN_LIBRARIE} not exists!")
  endif()
endforeach()


#
# libsecp256k1
#
set(secp256k1_DIR "${CHAIN_SRC_ROOT}/src/secp256k1")
set(secp256k1_LIBRARIES "${secp256k1_DIR}/.libs/libsecp256k1.a")
if (NOT EXISTS ${secp256k1_LIBRARIES})
  message(STATUS "build secp256k1...")
  execute_process(WORKING_DIRECTORY "${secp256k1_DIR}" COMMAND ./autogen.sh)
  execute_process(WORKING_DIRECTORY "${secp256k1_DIR}" COMMAND ./configure --enable-module-recovery)
  execute_process(WORKING_DIRECTORY "${secp256k1_DIR}" COMMAND make ${MAKE_JOBS})
endif()

message("-- CMAKE_OBJCOPY: ${CMAKE_OBJCOPY}")
if(NOT CMAKE_OBJCOPY)
  message("-- CMAKE_OBJCOPY not found, debug info will not separated from targets")
  mark_as_advanced(CMAKE_OBJCOPY)
endif()
include_directories(src test ${CHAIN_SRC_ROOT}/src ${CHAIN_SRC_ROOT}/src/config ${CHAIN_SRC_ROOT}/src/secp256k1/include
                    ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${LIBZMQ_INCLUDE_DIR}
                    ${LIBEVENT_INCLUDE_DIR})

file(GLOB_RECURSE TEST_SOURCES ../src/play.cpp)
add_executable(skr ${TEST_SOURCES})
target_link_libraries(skr ${BITCOIN_LIBRARIES})

