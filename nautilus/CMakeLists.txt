add_test_target(test_all
    test/dummy.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/dummy_kokkos.cpp>
)
