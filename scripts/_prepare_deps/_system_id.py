import platform


def _is_linux(system: str) -> bool:
    return 'Linux' == system


def _is_windows(system: str) -> bool:
    return 'Windows' == system


def _is_macos(system: str) -> bool:
    return 'Darwin' == system


def _linux_id_fun():
    return platform.freedesktop_os_release


def _no_distro_id_fun():
    return platform.system().lower


def get_system_id_fun():
    system: str = platform.system()
    if _is_linux(system):
        return _linux_id_fun
    return _no_distro_id_fun
