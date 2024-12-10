add_test_target(test_all
    test/isotope.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/isotope_kokkos.cpp>
    test/SZA.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/SZA_kokkos.cpp>
    test/zaid_tools.cpp
)
