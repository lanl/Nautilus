# Copyright Spack Project Developers. See COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

from spack.package import *

class Nautilus(CMakePackage, CudaPackage, ROCmPackage):
    """Nautilus library."""

    # Info
    homepage = "https://re-git.lanl.gov/xcap/oss/nautilus"
    git = "ssh://git@asc-git.lanl.gov:10022/xcap/oss/nautilus.git"
    maintainers = ["bkkrueger"]

    # Versions
    version("main", branch="main")
    # version("0.0.0", sha256="") # TODO: Release

    # Variants
    variant("coverage", default=False, description="Code Coverage Support")
    variant("doc",      default=False, description="Sphinx Documentation Support")
    variant("format",   default=False, description="Clang-Format Support")
    variant("kokkos",   default=False, description="Enable Kokkos Support")
    variant("test",     default=False, description="Build tests")

    # Documentation
    depends_on("py-sphinx", when="+doc")
    depends_on("py-sphinx-multiversion", when="+doc")
    depends_on("py-sphinx-rtd-theme", when="+doc")
    depends_on("py-sphinx-rtd-dark-mode", when="+doc")

    # Formatting
    depends_on("llvm@17.0.6+clang", when="+format")

    # Coverage
    depends_on("py-gcovr", when="+coverage")

    # Building and testing
    depends_on("catch2@3.0.1:", when="+test")
    depends_on("cmake@3.19:")
    depends_on("kokkos@3.7.01:", when="+kokkos")
    depends_on("kokkos+cuda+cuda_constexpr", when="+kokkos+cuda")
    depends_on("ports-of-call@1.7.1:")

    # Require the architecture information when a GPU variant is active
    conflicts("cuda_arch=none",     when="+cuda", msg="CUDA architecture is required")
    conflicts("amdgpu_target=none", when="+rocm", msg="ROCm architecture is required")

    # Propagate cuda_arch or amdgpu_target requirement to dependencies
    for _flag in CudaPackage.cuda_arch_values:
        depends_on(f"kokkos cuda_arch={_flag}", when=f"+kokkos+cuda cuda_arch={_flag}")
    for _flag in ROCmPackage.amdgpu_targets:
        depends_on(f"kokkos amdgpu_target={_flag}", when=f"+kokkos +rocm amdgpu_target={_flag}")

    def cmake_args(self):
        args = [
            self.define("NAUTILUS_BUILD_TESTS", self.run_tests and self.spec.satisfies("+test")),
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
