###############################################################################
# Requirements for test building
###############################################################################

#find_package(Catch2 REQUIRED)

###############################################################################
# add tests, this may get more complex as the testing capability
# is built out
###############################################################################

add_executable(dummy_test dummy.cpp)

#target_link_libraries(dummy_test PRIVATE nautilus Catch2::Catch2)
target_link_libraries(dummy_test PRIVATE nautilus)

#include(Catch)
#catch_discover_tests(dummy_test)
add_test(NAME test1 COMMAND dummy_test 7)
add_test(NAME test2 COMMAND dummy_test 5)
add_test(NAME test3 COMMAND dummy_test 8)
