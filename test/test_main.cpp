#include "catch2/catch_session.hpp"

#include <Kokkos_Core.hpp>

int main(int argc, char * argv[])
{
    Kokkos::initialize();

    int result;
    {
        // This is in a separate block to ensure that all Kokkos stuff is cleaned up before calling
        // finalize()
        result = Catch::Session().run(argc, argv);
    }

    Kokkos::finalize();
    return result;
}
