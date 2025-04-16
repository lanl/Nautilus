add_test_target(test_all
    test/nuclide.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/nuclide_kokkos.cpp>
    test/SZA.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/SZA_kokkos.cpp>
    test/zaid_tools.cpp
)
