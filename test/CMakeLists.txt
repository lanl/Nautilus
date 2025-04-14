###############################################################################
# Requirements for test building
###############################################################################

###############################################################################
# add tests, this may get more complex as the testing capability
# is built out
###############################################################################

# -------------------------------------------------------------------------------------------------
# nautilus_testing: flags and dependencies for all tests

# add an interface library for test-specific things
add_library(nautilus_testing INTERFACE)
target_link_libraries(nautilus_testing INTERFACE
    Catch2::Catch2
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:Kokkos::kokkos>
    nautilus
)
target_compile_definitions(nautilus_testing INTERFACE CATCH_CONFIG_FAST_COMPILE)

# Set the Ports-of-Call portability strategy
if (NAUTILUS_ENABLE_KOKKOS)
    target_compile_definitions(nautilus_testing INTERFACE PORTABILITY_STRATEGY_KOKKOS)
endif()

if(NOT NAUTILUS_SILENCE_WARNINGS)
    # turn on strict warnings and error
    # NOTE: The ROCm 6.2.1 installation on some machines (RZAdams and RZVernal at least) will
    #       inject a `--gcc-toolchain` command-line argument into Clang, which is a deprecated
    #       option.  Clang will warn about unused command-line arguments, so you get multiple
    #       instances of that warning.  Then `-Werror` promotes those to errors.  We can turn these
    #       back to non-fatal warnings with `-Wno-error=unused-command-line-argument` or we can
    #       turn off that particular warning with `-Wno-unused-command-line-argument`.
    target_compile_options(nautilus_testing INTERFACE
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},GNU>:-Werror;-Wall;-pedantic-errors;-Wunused-parameter>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},AppleClang>:-Werror;-Wall;-Wunused-parameter>
        $<$<NOT:$<BOOL:${WIN32}>>:$<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},Clang>:-Werror;-Wall;-Wno-unused-command-line-argument>>
        $<$<BOOL:${WIN32}>:$<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},Clang>:-Wall;-Wno-c++98-compat;-Wno-c++98-compat-pedantic>>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},Intel>:-Werror;-w2;-Wunused-variable;-Wunused-parameter;-diag-disable=remark>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},PGI>:>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},MSVC>:/permissive->
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},XL>:>
    )
else()
    # turn off warnings
    target_compile_options(nautilus_testing INTERFACE
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},GNU>:-w>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},AppleClang>:>
        $<$<NOT:$<BOOL:${WIN32}>>:$<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},Clang>:>>
        $<$<BOOL:${WIN32}>:$<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},Clang>:>>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},Intel>:>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},PGI>:>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},MSVC>:>
        $<$<IN_LIST:${CMAKE_CXX_COMPILER_ID},XL>:>
    )
endif()

# -------------------------------------------------------------------------------------------------
# test_main: tests get added here

add_executable(test_main test_main.cpp)
target_link_libraries(test_main PRIVATE nautilus_testing)

include(Catch)
catch_discover_tests(test_main)

# -------------------------------------------------------------------------------------------------
# test_general: very basic test (such as "Is Catch2 available?")

add_test_target(test_general test_general.cpp)

