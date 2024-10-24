cmake_minimum_required(VERSION 3.12)

project(xcap-sample-repo 
  VERSION 0.0.1
  LANGUAGES CXX
  DESCRIPTION "XCAP-Sample-Repo"
  HOMEPAGE_URL "https://re-git.lanl.gov/xcap/ec/xcap-sample-repo"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

###############################################################################
## Library configuration
###############################################################################

include(GNUInstallDirs)

# mostly standard cmake for header-only library

add_library(${PROJECT_NAME} INTERFACE)

# add dependency to external library, like Kokkos
find_package(Kokkos)
target_link_libraries(${PROJECT_NAME} INTERFACE Kokkos::kokkos)

target_include_directories(${PROJECT_NAME}
INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(DIRECTORY ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN *.hpp
)

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}_Targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(EXPORT ${PROJECT_NAME}_Targets FILE ${PROJECT_NAME}-targets.cmake
        NAMESPACE XCAP:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file("${PROJECT_NAME}-configVersion.cmake"
                                 VERSION ${PROJECT_VERSION} COMPATIBILITY ExactVersion)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-configVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

add_library(XCAP::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

###############################################################################
## Formatting
###############################################################################

# enable clang-format support
option(ENABLE_CLANG_FORMAT "Enable Clang-Format support" OFF)

if(ENABLE_CLANG_FORMAT)
  find_package(ClangFormat 12.0.0 REQUIRED)

  set(GLOBS
      ${PROJECT_SOURCE_DIR}/xcap-sample-repo/[^\.]*.cpp    ${PROJECT_SOURCE_DIR}/xcap-sample-repo/[^\.]*.hpp
      ${PROJECT_SOURCE_DIR}/test/[^\.]*.cpp                ${PROJECT_SOURCE_DIR}/test/[^\.]*.hpp
  )

  file(GLOB_RECURSE FORMAT_SOURCES CONFIGURE_DEPENDS ${GLOBS})

  add_custom_target(format
    COMMAND ClangFormat::ClangFormat -style=file -i ${FORMAT_SOURCES})
endif()

## Documentation

option(ENABLE_DOCUMENTATION "Build documentation" OFF)

if(ENABLE_DOCUMENTATION)
  set(PROJECT_LATEX_FILE latex_guide)
  set(DOCS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/doc/sphinx)
  set(DOCS_BUILD_DIR ${CMAKE_BINARY_DIR}/doc/html)
  set(LATEX_SOURCE_DIR ${CMAKE_SOURCE_DIR}/doc/latex)

  # technically this is a violation of src and build directory separation
  # TODO avoid manipulating source directory
  # TODO each version should be its own PDF
  add_custom_target(latex ALL
    COMMAND pdflatex ${PROJECT_LATEX_FILE}.tex
    #COMMAND bibtex ${PROJECT_LATEX_FILE}.aux
    COMMAND pdflatex ${PROJECT_LATEX_FILE}.tex
    COMMAND pdflatex ${PROJECT_LATEX_FILE}.tex
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_LATEX_FILE}.pdf ${DOCS_SOURCE_DIR}/_static
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_LATEX_FILE}.pdf ${DOCS_SOURCE_DIR}/_templates
    WORKING_DIRECTORY ${LATEX_SOURCE_DIR}
    COMMENT "Building Latex document..."
  )

  add_custom_target(docs ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOCS_BUILD_DIR}
    COMMAND sphinx-multiversion --dump-metadata . ${DOCS_BUILD_DIR}
    COMMAND sphinx-multiversion . ${DOCS_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${DOCS_SOURCE_DIR}/index.html ${DOCS_BUILD_DIR}
    WORKING_DIRECTORY ${DOCS_SOURCE_DIR}
    COMMENT "Building Sphinx documentation..."
  )

   add_dependencies(docs latex)
endif()

###############################################################################
## Coverage
###############################################################################

option(ENABLE_COVERAGE "Enable Code-Coverage" OFF)

if(ENABLE_COVERAGE)
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

###############################################################################
## Testing
###############################################################################

include(CTest)  # this defines the BUILD_TESTING option

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
