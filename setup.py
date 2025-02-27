import os
import sys

from setuptools import setup, Extension

try:
    import numpy
except ImportError:
    print(
        "Module `numpy` is a build dependency. Please add it to `pyproject.toml` in section `build-system.requires`."
    )
    sys.exit(1)


extra_compile_args = ["-O2"]
extra_link_args = []
options = {}

if sys.platform == "darwin":  # macOS
    extra_compile_args.append("-Xpreprocessor")
    extra_link_args.append("-lomp")
else:
    extra_compile_args.append("-fopenmp")
    extra_link_args.append("-fopenmp")

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
                "src/mis_finder/_core/mis.c", "src/mis_finder/_core/mis_impl.c", "src/mis_finder/_core/parse_numpy.c"
            ],
            include_dirs=[numpy.get_include()],
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args,
            define_macros=[('NPY_NO_DEPRECATED_API', 'NPY_1_7_API_VERSION')],
        )
    ],
    options=options,
)
