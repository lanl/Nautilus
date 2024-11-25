# Copyright 2013-2022 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

from spack import *

class Nautilus(CMakePackage):
    """Nautilus library."""

    # Info
    homepage = "https://re-git.lanl.gov/xcap/ec/nautilus"
    git = "ssh://git@asc-git.lanl.gov:10022/xcap/ec/nautilus.git"
    maintainers = ["bkkrueger"]

    # Versions
    version("main", branch="main")
    # version("0.0.0", sha256="") # TODO: Release

    # Variants
    variant("coverage", default=False, description="Code Coverage Support")
    variant("doc",      default=False, description="Sphinx Documentation Support")
    variant("format",   default=False, description="Clang-Format Support")
    variant("test",     default=False, description="Build tests")

    # TODO: Do I need to propagate GPU architecture to Kokkos?

    # Documentation
    depends_on("py-sphinx", when="+doc")
    depends_on("py-sphinx-multiversion", when="+doc")
    depends_on("py-sphinx-rtd-theme@0.4.3", when="+doc")
    depends_on("py-sphinx-rtd-dark-mode@1.2.4", when="+doc")

    # Formatting
    depends_on("llvm@12.0.0+clang", when="+format")

    # Coverage
    depends_on("py-gcovr", when="+coverage")

    # Building and testing
    depends_on("catch2@3.0.1:", when="+test")
    depends_on("cmake@3.19:")
    depends_on("kokkos@3.7.01:", when="+kokkos")
    depends_on("ports-of-call@1.5.2:")

    def cmake_args(self):
        args = [
            self.define("NAUTILUS_BUILD_TESTING", self.run_tests and self.spec.satisfies("+test")),
            self.define_from_variant("NAUTILUS_BUILD_DOCUMENTATION", "doc"),
            self.define_from_variant("NAUTILUS_ENABLE_COVERAGE", "coverage"),
            self.define_from_variant("NAUTILUS_ENABLE_CLANG_FORMAT", "format"),
            self.define_from_variant("NAUTILUS_ENABLE_KOKKOS", "kokkos"),
        ]
        if self.spec.satisfies("+kokkos ^kokkos+rocm"):
            args.append(self.define("CMAKE_CXX_COMPILER", self.spec["hip"].hipcc))
        elif self.spec.satisfies("+kokkos"):
            args.append(self.define("CMAKE_CXX_COMPILER", self.spec["kokkos"].kokkos_cxx))
        return args
