set(GTEST_DIR "gtest-1.7.0")

add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/gtest)
set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -w")

include_directories(SYSTEM ${GTEST_DIR}/gtest/include
                            ${GTEST_DIR}/include)

function(add_gtest target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} gtest_main )
    add_test(${target} ${target})

    add_custom_command(TARGET ${target}
                        POST_BUILD
                        COMMAND ${target}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "running ${target}" VERBATIM)
endfunction()
