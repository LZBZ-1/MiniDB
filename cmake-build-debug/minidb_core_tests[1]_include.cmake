if(EXISTS "C:/Users/jalb2/CLionProjects/MiniDB/cmake-build-debug/minidb_core_tests[1]_tests.cmake")
  include("C:/Users/jalb2/CLionProjects/MiniDB/cmake-build-debug/minidb_core_tests[1]_tests.cmake")
else()
  add_test(minidb_core_tests_NOT_BUILT minidb_core_tests_NOT_BUILT)
endif()
