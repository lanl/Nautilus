from kessel.workflows import *
from kessel.workflows.base.cmake import CMake as CMakeWorkflow
from kessel.workflows.cmake import Build
from pathlib import Path

class Docs(Build, CMakeWorkflow):
    steps = ["env", "configure", "build"]

    build_dir = environment(Path.cwd() / "build_docs")
    spack_env = environment("oss/nautilus/doc")
    project_spec = environment("nautilus+doc")

    @collapsed
    def configure(self, args):
        """Configure"""
        cmake_args = [self.define("NAUTILUS_ENABLE_LIBRARY", False)]
        super().configure(args, cmake_args)

    def build(self, args):
        """Build Documentation"""
        super().build(args, targets=["docs"])
