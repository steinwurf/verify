#! /usr/bin/env python
# encoding: utf-8

import shutil
import os
import platform
import waflib
import hashlib
import os.path

from waflib import Build, Errors, Logs

APPNAME = "verify"
VERSION = "6.0.0"


def options(ctx):

    ctx.load("cmake")

    opts = ctx.add_option_group("Verify Options")
    opts.add_option(
        "--no-use-system-zlib",
        default=False,
        dest="no_use_system_zlib",
        action="store_true",
        help="Whether to not use system zlib when building verify",
    )
    opts.add_option(
        "--no_libassert",
        default=False,
        dest="no_libassert",
        action="store_true",
        help="Whether to not use libassert when building verify",
    )


def configure(ctx):

    ctx.load("cmake")

    if ctx.options.no_libassert:
        ctx.env.CMAKE_CONFIGURE_ARGS += [
            "-DSTEINWURF_LIBASSERT=OFF",
        ]

    if ctx.is_toplevel():
        ctx.cmake_configure()


def build(ctx):

    ctx.load("cmake")

    if ctx.is_toplevel():
        ctx.cmake_build()


def prepare_release(ctx):
    # Rewrite versions
    with ctx.rewrite_file(filename="CMakeLists.txt") as f:
        pattern = r"VERSION \d+\.\d+\.\d+ # Managed by waf"
        replacement = "VERSION {} # Managed by waf".format(VERSION)

        f.regex_replace(pattern=pattern, replacement=replacement)
