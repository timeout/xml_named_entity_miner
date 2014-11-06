
#tests/CMakelists.txt

set(dir_test_dir_fixtures 
    dummy_directory/sub_directory_1
    dummy_directory/sub_directory_2)
set(dir_test_file_fixtures 
    file_1
    file_2
    file_3
    )

foreach (p ${dir_test_dir_fixtures})
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
        tests/${p})
endforeach()
foreach (f ${dir_test_file_fixtures})
    execute_process(COMMAND ${CMAKE_COMMAND} -E touch tests/dummy_directory/${f})
endforeach()

include_directories(${gtest_SOURCE_DIR}/include)
set(test_sources gtest_main.cpp
    pathname_test.cpp
    dir_test.cpp
    xml_doc_test.cpp
    )
add_executable(run_tests ${test_sources})
target_link_libraries(run_tests gtest_main pathname_lib dir_lib xml_lib)
