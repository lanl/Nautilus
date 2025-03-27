add_test_target(test_all
    test/names.cpp
    #$<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/names_kokkos.cpp>
    test/nuclide.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/nuclide_kokkos.cpp>
    test/SZA.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/SZA_kokkos.cpp>
    test/zaid_tools.cpp
)

add_subdirectory(tag)
