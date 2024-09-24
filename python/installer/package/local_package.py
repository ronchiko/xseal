import os.path

from installer.package.package_protocol import PackageProtocol
from installer.repository import XSealRepository


class LocalPackage(PackageProtocol):
    """
    Package for a local XSeal with the source installed.
    """
    def __init__(self, source_path):
        self.__path = source_path

    def create_package_files(self, repository: XSealRepository) -> None:
        repository.create_symlink('bin', self._binaries_directory_path, is_directory=True)
        repository.create_symlink('include', self._include_directory_path, is_directory=True)
        repository.create_symlink('thirdparty', self._third_party_directory_path, is_directory=True)

        repository.create_file('link.txt', self.__path)

    def install_premake_module(self, repository: XSealRepository) -> None:
        if repository.has_file('xseal'):
            repository.delete('xseal')

        repository.create_symlink('xseal', self._premake_module_path, is_directory=True)
        repository.create_file('link.txt', self.__path)

    @property
    def _include_directory_path(self) -> str:
        return os.path.join(self.__path, 'src')

    @property
    def _binaries_directory_path(self) -> str:
        return os.path.join(self.__path, 'bin')

    @property
    def _third_party_directory_path(self) -> str:
        return os.path.join(self.__path, 'thirdparty')

    @property
    def _premake_module_path(self) -> str:
        return os.path.join(self.__path, 'premake', 'xseal')
