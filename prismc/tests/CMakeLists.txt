
#   Copyright 2026 Prism Compiler Authors
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

project(tests LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set(SOURCES
    file_test.cpp
)

add_executable(run_gtests ${SOURCES})

target_include_directories(run_gtests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../dist/prism/include)

llvm_map_components_to_libnames(LLVM_LIBS 
    core
    support
)

if(WIN32)
    set(LLVM_LIBS ${LLVM_LIBS} psapi shell32 ole32 uuid version dbghelp)
endif()

target_link_libraries(run_gtests PRIVATE prismc GTest::gtest_main ${LLVM_LIBS})

include(GoogleTest)
gtest_discover_tests(run_gtests)