#!/usr/bin/env python3

from argparse import ArgumentParser
import os
from shutil import rmtree
import subprocess
import sys


def parse_args():
    ap = ArgumentParser()

    ap.add_argument(
        '-cb',
        '--clean-build',
        action='store_true',
        help='Remove contents of build directory before compiling',
    )

    default_source_path: str = os.path.join(os.environ['HOME'], 'ep')
    ap.add_argument(
        '-s',
        '--source-path',
        type=str,
        default=default_source_path,
        help='Path to project source dir (default: {})'.format(
            default_source_path
        ),
    )

    default_build_path: str = os.path.join(default_source_path, 'build')
    ap.add_argument(
        '-b',
        '--build-path',
        type=str,
        default=default_build_path,
        help='Path to build dir (default: {})'.format(default_build_path),
    )

    ap.add_argument(
        '-g',
        '--generator',
        type=str,
        choices={'Unix Makefiles', 'Ninja'},
        default='Ninja',
        help='CMake generator of choice (default: Ninja)',
    )

    ap.add_argument(
        '--dependencies',
        action='store_true',
        help='Use vcpkg to fetch dependencies',
    )

    ap.add_argument(
        '--unit-tests',
        action='store_true',
        help='Build unit tests. Tests will run automatically after build.',
    )

    ap.add_argument(
        '-cxx',
        '--cxx-compiler',
        type=str,
        choices={'clang++', 'g++'},
        default='clang++',
        help='C++ compiler to use',
    )

    threads = 4
    ap.add_argument(
        '-j',
        '--jobs',
        type=int,
        default=threads,
        help='Number of threads to use (default: {})'.format(threads),
    )

    return ap.parse_args()


if __name__ == '__main__':
    args = parse_args()

    try:
        project_source_path = os.path.join(args.source_path)

        if not os.path.exists(args.build_path):
            os.mkdir(args.build_path)

        project_build_path = os.path.join(args.build_path)
        generator = args.generator

        script_path = os.path.join(os.getcwd(), 'scripts', 'build.py')

        if (
            os.path.join(project_source_path, 'scripts', 'build.py')
            != script_path
        ):
            raise RuntimeError('Couldn\'t find project directory!')

        clean_before_build: bool = args.clean_build
        if clean_before_build:
            rmtree(project_build_path)

        cmake_args = [
            'cmake',
            '-DCMAKE_CXX_COMPILER={}'.format(args.cxx_compiler),
            '-S',
            project_source_path,
            '-B',
            project_build_path,
            '-G',
            generator,
            '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',
        ]

        if args.dependencies:
            cmake_args.extend(['-DVCPKG_DEPS=ON'])

        if args.unit_tests:
            cmake_args.extend(['-DUNIT_TESTS=ON'])

        p = subprocess.Popen(cmake_args)
        p.wait()

        p = subprocess.Popen(
            [
                'cmake',
                '--build',
                project_build_path,
                '--',
                '-j',
                '{}'.format(args.jobs),
            ]
        )
        p.wait()

    except Exception as exc:
        sys.exit(exc)
