#include "catch2/catch_session.hpp"

#ifdef NAUTILUS_ENABLE_KOKKOS
#include <Kokkos_Core.hpp>
#endif

int main(int argc, char * argv[])
{
#ifdef NAUTILUS_ENABLE_KOKKOS
    Kokkos::initialize();
#endif

    int result;
    {
        // This is in a separate block to ensure that all Kokkos stuff is cleaned up before calling
        // finalize()
        result = Catch::Session().run(argc, argv);
    }

#ifdef NAUTILUS_ENABLE_KOKKOS
    Kokkos::finalize();
#endif
    return result;
}
