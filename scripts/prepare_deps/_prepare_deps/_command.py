from typing import List


class Command:
    def __init__(
        self, needs_elevation: bool, executable: str, flags: str, args: str
    ):
        self._needs_elevation: bool = needs_elevation
        self._exec: str = executable
        self._flags: str = flags
        self._args: str = args

    @property
    def needs_elevation(self) -> bool:
        return self._needs_elevation

    @property
    def exec(self) -> str:
        return self._exec

    @property
    def flags(self) -> str:
        return self._flags

    @property
    def args(self) -> str:
        return self._args

    def _items(self) -> List[str]:
        return [c for c in [self.exec, self.flags, self.args] if c]

    def __str__(self) -> str:
        return ''.join(item + ' ' for item in self._items())

    def get(self) -> List[str]:
        return self._items()
