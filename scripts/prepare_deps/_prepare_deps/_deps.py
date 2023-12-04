from typing import List, Final

arch_deps: Final[List[str]] = [
    'libx11',
    'mesa',
    'libxrender',
    'libxi',
    'libxkbcommon-x11',
    'wayland',
]

debian_deps: Final[List[str]] = [
    'libx11-xcb-dev',
    'libglu1-mesa-dev',
    'libxrender-dev',
    'libxi-dev',
    'libxkbcommon-x11-dev',
    'libwayland-dev',
    'libxkbcommon-dev=1.5.0-1',
    'autoconf',
    'autoconf-archive',
    'libtool',
    'libxcb*-dev',
]
