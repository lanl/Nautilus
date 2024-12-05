add_test_target(test_all
    test/dummy.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/dummy_kokkos.cpp>

    test/isotope.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/isotope_kokkos.cpp>
    test/SZA.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/SZA_kokkos.cpp>
    test/zaid_tools.cpp
)
