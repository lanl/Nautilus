Building Nautilus
===================================================================================================

Nautilus source code can be downloaded from `GitHub <https://github.com/lanl/nautilus>`.
Development primarily occurs on a LANL internal server, with releases mirrored to GitHub.  If you
have access to LANL internal systems, you can find the development repository at
https://re-git.lanl.gov/xcap/oss/nautilus.

Nautilus depends on the following libraries:

- `Ports-of-Call`_ is required
- `Kokkos`_ is optional, but allows Nautilus to run on GPUs
- `Catch2`_ is optional, but allows building unit tests

.. _`Ports-of-Call`: https://github.com/lanl/ports-of-call
.. _`Kokkos`: https://github.com/kokkos/kokkos
.. _`Catch`: https://github.com/catchorg/Catch2
