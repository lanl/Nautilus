# Copyright 2013-2022 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

from spack import *


class Nautilus(CMakePackage):
    """Nautilus library."""

    homepage = "https://re-git.lanl.gov/xcap/ec/nautilus"
    git = "ssh://git@asc-git.lanl.gov:10022/xcap/ec/nautilus.git"
    #url = ""

    version("main", branch="main")

    variant('test', default=False, description='Enable test package installs')
    
    variant("doc", default=False, description="Sphinx Documentation Support")
    variant("format", default=False, description="Clang-Format Support")
    variant("coverage", default=False, description="Code Coverage Support")

    depends_on("cmake@3.12:")
    depends_on("catch2@3.0.1", when="+test")

    # this Spack package inherits its GPU configuration from Kokkos
    depends_on("kokkos")

    depends_on("py-sphinx", when="+doc")
    depends_on("py-sphinx-multiversion", when="+doc")
    depends_on("py-sphinx-rtd-theme@0.4.3", when="+doc")
    depends_on("py-sphinx-rtd-dark-mode@1.2.4", when="+doc")
    depends_on("texlive", when="+doc")

    depends_on("llvm@12.0.0+clang", when="+format")
    depends_on("py-gcovr", when="+coverage")

    def cmake_args(self):
        args = [
            self.define("BUILD_TESTING", self.run_tests),
            self.define_from_variant("ENABLE_DOCUMENTATION", "doc"),
        ]
        if self.spec.satisfies("^kokkos+rocm"):
            args.append(self.define("CMAKE_CXX_COMPILER", self.spec["hip"].hipcc))
        else:
            args.append(self.define("CMAKE_CXX_COMPILER", self.spec["kokkos"].kokkos_cxx))
        return args
