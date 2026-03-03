from kessel.workflows import *
from kessel.workflows.base.cmake import CMake as CMakeWorkflow
from kessel.workflows.cmake import Build
from pathlib import Path

class Format(Build, CMakeWorkflow):
    steps = ["env", "configure", "check_format"]

    build_dir = environment(Path.cwd() / "build_format")
    spack_env = environment("oss/nautilus/format")
    project_spec = environment("nautilus+format")

    @collapsed
    def configure(self, args):
        """Configure"""
        cmake_args = [self.define("NAUTILUS_ENABLE_LIBRARY", False)]
        super().configure(args, cmake_args)

    def check_format(self, args):
        """Clang-Format"""
        super().build(args, targets=["format"])
        self.exec(f"git -C '{self.source_dir}' diff --exit-code --compact-summary")
