# from https://github.com/RDFLib/rdflib-hdt/blob/master/setup.py

from setuptools import find_packages, setup, Extension
from os import listdir
import pybind11


def list_files(path: str, extension=".cpp", exclude="S.cpp"):
    """List paths to all files that ends with a given extension"""
    return ["%s/%s" % (path, f) for f in listdir(path) if f.endswith(extension) and (not f.endswith(exclude))]


# pyHDT source files
sources = [
    "src/hdt.cpp",
    "src/hdt_document.cpp",
    "src/lazy_id_iterator.cpp"
]

# HDT source files
sources += list_files("hdt-cpp/libcds/src/static/bitsequence")
sources += list_files("hdt-cpp/libcds/src/static/coders")
sources += list_files("hdt-cpp/libcds/src/static/mapper")
sources += list_files("hdt-cpp/libcds/src/static/sequence")
sources += list_files("hdt-cpp/libcds/src/static/permutation")
sources += list_files("hdt-cpp/libcds/src/utils")
sources += list_files("hdt-cpp/libhdt/src/bitsequence")
sources += list_files("hdt-cpp/libhdt/src/dictionary")
sources += list_files("hdt-cpp/libhdt/src/hdt")
sources += list_files("hdt-cpp/libhdt/src/header")
sources += list_files("hdt-cpp/libhdt/src/huffman")
sources += list_files("hdt-cpp/libhdt/src/libdcs")
sources += list_files("hdt-cpp/libhdt/src/libdcs/fmindex")
sources += list_files("hdt-cpp/libhdt/src/rdf")
sources += list_files("hdt-cpp/libhdt/src/sequence")
sources += list_files("hdt-cpp/libhdt/src/triples")
sources += list_files("hdt-cpp/libhdt/src/util")
sources += list_files("hdt-cpp/libhdt/src/sparql")

# pybind11 + pyHDT + libcds + HDT-lib headers
include_dirs = [
    pybind11.get_include(),
    pybind11.get_include(True),
    "include/",
    "hdt-cpp/libhdt/include/",
    "hdt-cpp/libhdt/src/dictionary/",
    "hdt-cpp/libhdt/src/sparql/",
    "hdt-cpp/libcds/include/",
    "hdt-cpp/libcds/src/static/bitsequence",
    "hdt-cpp/libcds/src/static/coders",
    "hdt-cpp/libcds/src/static/mapper",
    "hdt-cpp/libcds/src/static/permutation",
    "hdt-cpp/libcds/src/static/sequence",
    "hdt-cpp/libcds/src/utils"
]

# Need to build in c++11 minimum
# TODO add a check to use c++14 or c++17 if available
extra_compile_args = ["-std=c++14"]

# build HDT extension
hdt_extension = Extension("hdt_python",
                          sources=sources,
                          include_dirs=include_dirs,
                          extra_compile_args=extra_compile_args,
                          language='c++')

setup(
    name="hdt_python",
    version="0.0.0",
    author="chat-wane",
    author_email="grumpy.chat.wane@gmail.com",
    url=None,
    description="HDT bindings for python.",
    long_description=None,
    keywords=["rdflib", "hdt", "rdf", "semantic web", "search"],
    license="MIT",
    packages=find_packages(exclude=["tests"]),
    ext_modules=[hdt_extension])

