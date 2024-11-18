add_library(test_all OBJECT
    test/dummy.cpp
)

target_include_directories(test_all PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(test_all PRIVATE nautilus_testing)
target_sources(test_main PRIVATE $<TARGET_OBJECTS:test_all>)
