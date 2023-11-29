from collections.abc import Callable, Mapping
from ._command import Command
from ._deps import *
from ._system import *
from ._system_id import get_system_id_fun
from typing import Dict, List


def _build_arch_like() -> System:
    commands: List[Commands] = [
        Command(
            needs_elevation=True,
            executable='pacman',
            flags='-Suy',
            args=''.join([d + ' ' for d in arch_deps]),
        )
    ]

    s: ArchLike = ArchLike(commands)

    s.issue_extra_command(
        Command(
            needs_elevation=True,
            executable='pacman',
            flags='-U',
            args='https://archive.archlinux.org/packages/l/libxkbcommon/libxkbcommon-1.5.0-1-x86_64.pkg.tar.zst',
        )
    )

    return s


def _build_debian_like() -> System:
    commands: List[Command] = [
        Command(
            needs_elevation=True,
            executable='apt',
            flags='install -y',
            args=''.join([d + ' ' for d in debian_deps]),
        )
    ]

    s: DebianLike = DebianLike(commands)

    return s


_system_builders: Dict[str, Callable[[], System]] = {
    'arch': _build_arch_like,
    'debian': _build_debian_like,
}


def build_system() -> System:
    sys_info = get_system_id_fun()()()
    sys_type: str = ''
    if isinstance(sys_info, Mapping):
        if 'ID_LIKE' in sys_info:
            sys_type = sys_info['ID_LIKE']
        elif 'ID' in sys_info:
            sys_type = sys_info['ID']
    elif isinstance(sys_info, str):
        sys_type = sys_info

    return _system_builders[sys_type]()
