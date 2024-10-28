#include <cstdio>
#include <nautilus/dummy.hpp>

int main(int argc, char * argv[])
{
    Kokkos::initialize(argc, argv);

    printf("Hello World on Kokkos execution space %s\n", Kokkos::DefaultExecutionSpace::name());

    Kokkos::parallel_for(
        "HelloWorld", 15, KOKKOS_LAMBDA(const int i) {
            Kokkos::printf("Hello from i = %i\n", i);
        });

    Kokkos::finalize();
    return 0;
}
