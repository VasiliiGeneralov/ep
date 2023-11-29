#!/usr/bin/env python3

from _prepare_deps._build_system import build_system
from _prepare_deps._system import System
import subprocess
import sys


def _prompt_user():
    print()
    ans: str = input('Proceed? [y/N]: ')
    if ans and 'y' == ans[0].lower():
        return
    sys.exit('Aborting.')


def _any_of(iterable, pred) -> bool:
    for item in iterable:
        if pred(item):
            return True
    return False


def _print_system(s: System):
    print('\nSystem: {}'.format(s))
    if _any_of(s.commands, lambda c: c.needs_elevation):
        print(
            '\nWARNING!!!\nSome commands will require super user elevation!\n'
        )
    print('Following commands will be executed:')
    for command in s.commands:
        print('\t* {}'.format(command))


if __name__ == '__main__':
    s: System = build_system()
    _print_system(s)
    _prompt_user()

    for command in s.commands:
        args = []
        if command.needs_elevation:
            args = s.elevation_command.get()

        args.extend(command.get())
        args = [e for arg in args for e in arg.split()]

        p = subprocess.Popen(args)

        try:
            p.wait()
        except subprocess.TimeoutExpired:
            p.kill()
