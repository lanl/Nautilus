###################################################################################################
# Configuration
###################################################################################################

cmake_minimum_required(VERSION 3.19)

project(nautilus
    VERSION 1.0.1.3
    LANGUAGES NONE
    DESCRIPTION "Nautilus nuclear utilities library"
    HOMEPAGE_URL "https://github.com/lanl/nautilus"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

###################################################################################################
# Formatting
###################################################################################################

if(NAUTILUS_ENABLE_CLANG_FORMAT)
    find_package(ClangFormat 17.0.6 REQUIRED)

    set(GLOBS
        ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/[^\.]*.cpp ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/[^\.]*.hpp
        ${PROJECT_SOURCE_DIR}/examples/[^\.]*.cpp        ${PROJECT_SOURCE_DIR}/examples/[^\.]*.hpp
        ${PROJECT_SOURCE_DIR}/test/[^\.]*.cpp            ${PROJECT_SOURCE_DIR}/test/[^\.]*.hpp
    )

    file(GLOB_RECURSE FORMAT_SOURCES CONFIGURE_DEPENDS ${GLOBS})

    add_custom_target(format COMMAND ClangFormat::ClangFormat -style=file -i ${FORMAT_SOURCES})
endif()

###################################################################################################
# Documentation
###################################################################################################

if(NAUTILUS_BUILD_DOCUMENTATION)
    set(DOCS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/doc/sphinx)
    set(DOCS_BUILD_DIR ${CMAKE_BINARY_DIR}/doc/html)
    add_custom_target(docs ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DOCS_BUILD_DIR}
        COMMAND sphinx-multiversion --dump-metadata . ${DOCS_BUILD_DIR}
        COMMAND sphinx-multiversion . ${DOCS_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${DOCS_SOURCE_DIR}/index.html ${DOCS_BUILD_DIR}
        WORKING_DIRECTORY ${DOCS_SOURCE_DIR}
        COMMENT "Building Sphinx documentation..."
    )
endif()

###################################################################################################
# Library configuration
###################################################################################################

option(NAUTILUS_BUILD_DOCUMENTATION "Build documentation"         OFF)
option(NAUTILUS_BUILD_TESTS         "Build tests and examples"    ON)
option(NAUTILUS_ENABLE_CLANG_FORMAT "Enable Clang-Format support" OFF)
option(NAUTILUS_ENABLE_COVERAGE     "Enable Code-Coverage"        OFF)
option(NAUTILUS_ENABLE_KOKKOS       "Enable Kokkos support"       OFF)
option(NAUTILUS_ENABLE_LIBRARY      "Enable library build"        ON)
set(NAUTILUS_WARNINGS "silent" CACHE STRING "Control compiler warnings: silent (no warnings), nonfatal (standard warnings), fatal (warnings promoted to errors)")
set_property(CACHE NAUTILUS_WARNINGS PROPERTY STRINGS "fatal" "nonfatal" "silent")

if(NAUTILUS_ENABLE_LIBRARY)
    enable_language(CXX)

    add_library(${PROJECT_NAME} INTERFACE)
    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

    target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

    target_include_directories(${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    find_package(ports-of-call 1.7.1 REQUIRED)
    target_link_libraries(${PROJECT_NAME} INTERFACE ports-of-call::ports-of-call)

    if(NAUTILUS_ENABLE_KOKKOS)
        find_package(Kokkos 3.7.01 REQUIRED)
        target_compile_definitions(${PROJECT_NAME} INTERFACE NAUTILUS_ENABLE_KOKKOS)
        target_link_libraries(${PROJECT_NAME} INTERFACE Kokkos::kokkos)
    else()
        message(STATUS "Building without Kokkos: excluding Nautilus Kokkos tests")
    endif()

endif()

###################################################################################################
# Installation
###################################################################################################

if(NAUTILUS_ENABLE_LIBRARY)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(DIRECTORY ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN *.hpp
    )

    install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}_Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(EXPORT ${PROJECT_NAME}_Targets FILE ${PROJECT_NAME}-targets.cmake
        NAMESPACE nautilus:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    write_basic_package_version_file(
        "${PROJECT_NAME}-configVersion.cmake"
        VERSION ${PROJECT_VERSION} COMPATIBILITY ExactVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-configVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
endif()

###################################################################################################
# Coverage
###################################################################################################

if(NAUTILUS_ENABLE_LIBRARY)
    if(NAUTILUS_ENABLE_COVERAGE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")

        include(FindPackageHandleStandardArgs)
        find_program(GCOVR_BINARY gcovr)
        find_package_handle_standard_args(GCOVR DEFAULT_MSG GCOVR_BINARY)

        if(GCOVR_FOUND)
            add_custom_target(
                coverage_xml
                COMMAND ${GCOVR_BINARY} -s -x -r ${CMAKE_SOURCE_DIR} --filter ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/ --object-directory=${CMAKE_BINARY_DIR} -o coverage.xml
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating XML coverage report..."
            )

            set(COVERAGE_HTML_DIR ${CMAKE_BINARY_DIR}/coverage)

            add_custom_target(coverage_html_folder
                COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_HTML_DIR})

            add_custom_target(
                coverage_html
                COMMAND ${GCOVR_BINARY} --html --html-details -r ${CMAKE_SOURCE_DIR} --filter ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/ --object-directory=${CMAKE_BINARY_DIR} -o ${COVERAGE_HTML_DIR}/index.html
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating HTML coverage report..."
            )
            add_dependencies(coverage_html coverage_html_folder)
        endif()
    endif()
endif()

###################################################################################################
# Testing
###################################################################################################

if(NAUTILUS_ENABLE_LIBRARY)
    include(CTest)  # this defines the BUILD_TESTING option

    if(NAUTILUS_BUILD_TESTS)
        find_package(Catch2 3.0.1 REQUIRED)

        function(add_test_target TEST_NAME)
            add_library(${TEST_NAME} OBJECT ${ARGN})
            target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
            target_link_libraries(${TEST_NAME} PRIVATE nautilus_testing)
            target_sources(test_main PRIVATE $<TARGET_OBJECTS:${TEST_NAME}> )
        endfunction()

        add_subdirectory(test)
        add_subdirectory(examples)
        add_subdirectory(${PROJECT_NAME})
    endif()
endif()

###################################################################################################
# Configuration Summary
###################################################################################################

if(NAUTILUS_ENABLE_LIBRARY)
    include(config_summary)

    config_summary_header("Nautilus" "nautilus")

    config_summary_block("CMake Options")
    config_summary_variable("CMAKE_BUILD_TYPE")
    config_summary_variable("CMAKE_CXX_COMPILER")
    config_summary_variable("CMAKE_CXX_COMPILER_VERSION")
    config_summary_variable("CMAKE_CXX_FLAGS")

    config_summary_block("Dependencies")
    config_summary_dependency("Catch2"        "Catch2"      ${NAUTILUS_BUILD_TESTS})
    config_summary_dependency("ClangFormat"   "ClangFormat" ${NAUTILUS_ENABLE_CLANG_FORMAT})
    config_summary_dependency("Kokkos"        "Kokkos"      ${NAUTILUS_ENABLE_KOKKOS})
    config_summary_dependency("Ports-of-Call" "ports-of-call")

    config_summary_block("User Options")
    config_summary_option("NAUTILUS_BUILD_DOCUMENTATION")
    config_summary_option("NAUTILUS_BUILD_TESTS")
    config_summary_option("NAUTILUS_ENABLE_CLANG_FORMAT")
    config_summary_option("NAUTILUS_ENABLE_COVERAGE")
    config_summary_option("NAUTILUS_ENABLE_KOKKOS")
    config_summary_option("NAUTILUS_ENABLE_LIBRARY")
    config_summary_option("NAUTILUS_WARNINGS" "nonfatal" "fatal")

    config_summary_print()
endif()
