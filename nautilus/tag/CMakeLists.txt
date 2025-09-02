add_test_target(test_tag
    test/bitsegment.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/bitsegment_kokkos.cpp>
    test/format_ic_chemsym.cpp      # Not currently expected to run on GPUs
    test/format_ndi.cpp             # Not currently expected to run on GPUs
    test/format_standard_name.cpp   # Not currently expected to run on GPUs
    test/names.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/names_kokkos.cpp>
    test/pantag.cpp
    $<$<BOOL:${NAUTILUS_ENABLE_KOKKOS}>:test/pantag_kokkos.cpp>
    #TODO: test/string_processing.cpp
)
