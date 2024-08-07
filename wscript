#! /usr/bin/env python
# encoding: utf-8

import shutil
import os
import platform
import waflib
import hashlib
import os.path

from waflib.Build import BuildContext

APPNAME = "verify"
VERSION = "1.1.0"


def configure(conf):
    conf.set_cxx_std(17)


def build(bld):
    bld.stlib(
        target="verify",
        features="cxx",
        source=["src/verify.cpp"],
        export_includes=[bld.path.find_dir("include")],
        includes=[bld.path.find_dir("include")],
        use=["assert", "cpptrace", "dwarf", "zstd"],
    )

    if bld.is_toplevel():
        bld.program(
            target="example",
            features="cxx test",
            source="example.cpp",
            use=["verify"],
        )
