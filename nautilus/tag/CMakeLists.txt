add_test_target(test_tag
    test/bitsegment.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/bitsegment_kokkos.cpp>
    test/format_standard_name.cpp
    #$<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/format_standard_name.cpp>
    test/pantag.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/pantag_kokkos.cpp>
)
