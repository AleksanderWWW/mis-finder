import os
import sys

from setuptools import setup, Extension


extra_compile_args = ["-O2"]
extra_link_args = []
options = {}

if sys.platform == "win32":
        extra_link_args.append("-static")
        if os.getenv("USE_MINGW_GCC", "true").lower()[0] in ("1", "t", "y"):
            options["build_ext"] = {"compiler": "mingw32"}

setup(
    name="mis_finder",
    packages=["mis_finder"],
    ext_modules=[
        Extension(
            "_mis_finder_core",
            sources=[
                "src/mis_finder/_core/mis.c", 
                "src/mis_finder/_core/mis_impl.c", 
                "src/mis_finder/_core/parse_list.c",
                "src/mis_finder/_core/utils.c",
            ],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args,
        )
    ],
    options=options,
)
