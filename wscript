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
VERSION = "2.0.0"


def configure(conf):
    conf.set_cxx_std(17)


def build(bld):
    bld.post_mode = Build.POST_LAZY

    # Find the source directory for the external library
    src_dir = bld.dependency_node("libassert-source")

    # Declare the temporary build directory for the external library
    # it is best to keep it under the project build directory
    # TODO: Figure out how to make this temporary again.
    build_dir = bld.bldnode.make_node("libassert_build")

    # Declare the install directory for the external library
    install_dir = build_dir.make_node("install")

    # Declare the include directory for the external library
    include_dir = install_dir.make_node("include")

    lib_dir = install_dir.make_node("lib")
    lib64_dir = install_dir.make_node("lib64")

    # Build the external library through an external process
    bld(
        rule=CMakeBuildTask,
        target=build_dir.make_node("flag.lock"),
        install_dir=install_dir,
        source=src_dir,
    )

    # once it is done create a second build group
    bld.add_group()

    #if platform.system() == "Windows":
    #    lib_name = "assert"
    #else:
    #    lib_name = "assert"

    bld.read_stlib("assert", paths=[lib_dir, lib64_dir], export_includes=[include_dir])
    bld.read_stlib("cpptrace", paths=[lib_dir, lib64_dir], export_includes=[include_dir])
    bld.read_stlib("dwarf", paths=[lib_dir, lib64_dir], export_includes=[include_dir])
    bld.read_stlib("zstd", paths=[lib_dir, lib64_dir], export_includes=[include_dir])

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


def CMakeBuildTask(task):
    CMAKE_BUILD_TYPE = "Release"
    if task.env["stored_options"]["cxx_debug"]:
        CMAKE_BUILD_TYPE = "Debug"

    # This is the directory where the external library will be installed the
    # task.outputs[0] is the flag file that will be created once the external
    # library is installed
    output_dir = task.outputs[0].parent

    # This is the directory where the external library source code is located
    source_dir = task.inputs[0]

    # The install dir is passed as a parameter to the task
    install_dir = task.generator.install_dir

    # remove the output directory if it exists
    shutil.rmtree(output_dir.abspath())

    # create the output directory
    os.makedirs(output_dir.abspath())

    flags = []
    # Our waf mkspec hardcodes the windows runtime to be mutlithreaded static i.e. /MT
    # so we need to pass this to cmake as well so that it can link properly
    # See https://cmake.org/cmake/help/latest/prop_tgt/MSVC_RUNTIME_LIBRARY.html
    if platform.system() == "Windows":
        flags.append("-DCMAKE_POLICY_DEFAULT_CMP0091:STRING=NEW")
        if CMAKE_BUILD_TYPE == "Debug":
            flags.append("-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug")
        elif CMAKE_BUILD_TYPE == "Release":
            flags.append("-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded")
        # For 32 bit builds we need to pass -A Win32 for cross compiling with mkspec
        if task.env["DEST_CPU"] == "x86":
            flags.append("-A Win32")

    # SRT cmake flags
    flags += [
        f"-DCMAKE_BUILD_TYPE={CMAKE_BUILD_TYPE}",
    ]
    flags = " ".join(flags)

    # Run all commands in the output directory
    cwd = output_dir.abspath()

    try:
        task.generator.bld.cmd_and_log(
            f"cmake {flags} -S {source_dir}", cwd=cwd, quiet=0, output=0
        )
        task.generator.bld.cmd_and_log(
            f"cmake --build . --parallel --config {CMAKE_BUILD_TYPE}",
            cwd=cwd,
            quiet=0,
            output=0,
        )

        task.generator.bld.cmd_and_log(
            f"cmake --install . --prefix {install_dir} --config {CMAKE_BUILD_TYPE}",
            cwd=cwd,
            quiet=0,
            output=0,
        )

    except Errors.WafError as e:
        Logs.error(e.stderr)
        return -1

    Logs.info(f"Installed lib to {output_dir}")

    # write a lock file so that a rebuild occurs if files are removed manually
    task.outputs[0].write("ok")
