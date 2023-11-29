from abc import ABC, abstractproperty, abstractmethod
from ._command import Command
from typing import List


class System(ABC):
    def __init__(self, commands: List[Command]):
        self._commands: List[Command] = []
        for command in commands:
            self._add_command(command)

    @abstractmethod
    def __str__(self) -> str:
        pass

    def _add_command(self, command: Command):
        self.commands.append(command)

    @abstractproperty
    def elevation_command(self) -> Command:
        pass

    @property
    def commands(self) -> List[Command]:
        return self._commands

    def issue_extra_command(self, command: Command):
        print(
            'The \'{}\' system will issue an extra command:\n\t* {}'.format(
                self, command
            )
        )
        self._add_command(command)


class ArchLike(System):
    _elevation_command: Command = Command(False, 'sudo', '', '')

    def __init__(self, commands: List[Command]):
        super().__init__(commands)

    @property
    def elevation_command(self) -> Command:
        return ArchLike._elevation_command

    def __str__(self) -> str:
        return 'arch-like'


class DebianLike(System):
    _elevation_command: Command = Command(False, 'sudo', '', '')

    def __init__(self, commands: List[Command]):
        super().__init__(commands)

    @property
    def elevation_command(self) -> Command:
        return DebianLike._elevation_command

    def __str__(self) -> str:
        return 'debian-like'
