import os
from typing import IO

from installer.system_utils import is_windows

_WINDOWS_REPOSITORY_PATH = os.path.join('%AppData%', 'XSeal')
_WINDOWS_INSTALLS_PATH = os.path.join(r'C:\Program Files', 'XSeal', 'Versions')

_UNIX_REPOSITORY_PATH = os.path.join('~', '.xseal')
_UNIX_INSTALLS_PATH = '/usr/bin/xseal'


class XSealRepository:
    """
    A class that represents the local XSeal repository
    """
    def __init__(self, path: str):
        self._path = os.path.expandvars(path)
        if not os.path.exists(self._path):
            os.mkdir(self._path)

    @property
    def path(self):
        return self._path

    def has_file(self, name: str) -> bool:
        """
        Returns true if the repository has a file with a given name.
        """
        _full_path = os.path.join(self.path, name)
        return os.path.exists(_full_path)

    def open_file(self, name: str, mode: str = 'r', create_if_missing: bool = False) -> IO:
        """
        Opens or creates a files in the repository

        :param name: The name of the file
        :param mode: The mode to open the file in.
        :param create_if_missing: Should create the file if it's missing.
        """
        _full_path = os.path.join(self.path, name)
        if not self.has_file(name) and create_if_missing:
            open(_full_path, 'w+').close()

        return open(_full_path, mode=mode)

    def create_file(self, name: str, data: str) -> None:
        """
        Creates a new file and writes some data into it.
        """
        with self.open_file(name, mode='w+', create_if_missing=True) as file_stream:
            file_stream.write(data)

    def open_sub_repository(self, name: str, create_if_missing: bool = False) -> 'XSealRepository':
        """
        Opens or creates a sub-repository of this repository by its name

        :param name: The name of the sub-repository
        :param create_if_missing: Should create the sub-repository directory if it doesn't exist
        """
        _full_path = os.path.join(self.path, name)
        if not os.path.exists(_full_path) and create_if_missing:
            os.mkdir(_full_path)

        if not os.path.isdir(_full_path):
            raise RuntimeError('Sub-Repositories must be directories!')

        return XSealRepository(path=_full_path)

    def create_symlink(self, name: str, target_path: str, is_directory: bool) -> None:
        """
        Creates a new symbolic link in this repository.

        :param name: The name to give the symbolic link.
        :param target_path: The path this symbolic link points to.
        :param is_directory: Does the symlink point to a directory?
        """
        _full_path = os.path.join(self.path, name)
        os.symlink(target_path, _full_path, target_is_directory=is_directory)

    def delete(self, name: str):
        """
        Deletes a file from this repository
        """
        _full_path = os.path.join(self.path, name)
        os.remove(_full_path)

class XSealMainRepository(XSealRepository):
    def __init__(self):
        super().__init__(_WINDOWS_REPOSITORY_PATH if is_windows() else _UNIX_REPOSITORY_PATH)


class XSealInstallsRepository(XSealRepository):
    def __init__(self):
        super().__init__(_WINDOWS_INSTALLS_PATH if is_windows() else _UNIX_INSTALLS_PATH)
