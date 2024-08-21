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
VERSION = "2.0.0"


def configure(conf):
    conf.set_cxx_std(17)


def build(bld):
    if platform.system() == "Windows":
        use = []
    else:
        use = ["assert", "cpptrace", "dwarf", "zstd"]

    bld.stlib(
        target="verify",
        features="cxx",
        source=["src/verify.cpp"],
        export_includes=[bld.path.find_dir("include")],
        includes=[bld.path.find_dir("include")],
        use=use,
    )

    if bld.is_toplevel():
        bld.program(
            features="cxx test",
            source=bld.path.ant_glob("test/**/*.cpp"),
            target="verify_tests",
            use=["verify", "gtest"],
        )


def prepare_release(ctx):
    # Rewrite versions
    with ctx.rewrite_file(filename="CMakeLists.txt") as f:
        pattern = r"VERSION \d+\.\d+\.\d+ # Managed by waf"
        replacement = "VERSION {} # Managed by waf".format(VERSION)

        f.regex_replace(pattern=pattern, replacement=replacement)
