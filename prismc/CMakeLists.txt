
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
#

set(SOURCES
    src/prismc.cpp

    src/file/file.cpp
    src/lexer/lexer.cpp
    src/lexer/token.cpp
    src/parser/parser.cpp
    src/parser/ast.cpp
    src/ir_generator/ir_generator.cpp
    src/obj_compiler/obj_compiler.cpp
    src/linker/linker.cpp
)

add_library(prismc SHARED ${SOURCES})

target_include_directories(prismc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
 
target_compile_definitions(prismc PRIVATE ${LLVM_DEFINITIONS})
target_compile_definitions(prismc PRIVATE PRISMC_EXPORTS)
target_compile_definitions(prismc PRIVATE -DLLVM_BUILD_STATIC)

set(LLVM_LIBS 
    ${LLVM_AVAILABLE_LIBS}
)

if(WIN32)
    set(LLVM_LIBS ${LLVM_LIBS} psapi shell32 ole32 uuid version dbghelp shlwapi)
endif()

target_link_libraries(prismc PRIVATE ${LLVM_LIBS})

set(DIST_INCLUDE "${CMAKE_SOURCE_DIR}/dist/prism/include")

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/copy_and_rename.cmake" "
    file(REMOVE_RECURSE \"${DIST_INCLUDE}\")
    file(COPY \"${CMAKE_CURRENT_SOURCE_DIR}/include/\" DESTINATION \"${DIST_INCLUDE}\")
    if(EXISTS \"${DIST_INCLUDE}/prismc.hpp\")
        file(RENAME \"${DIST_INCLUDE}/prismc.hpp\" \"${DIST_INCLUDE}/prismc\")
    endif()
")

add_custom_target(copy_headers ALL
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/copy_and_rename.cmake"
)

add_dependencies(prismc copy_headers)