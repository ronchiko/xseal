from typing import Protocol

from installer.repository import XSealRepository


class PackageProtocol(Protocol):
    def create_package_files(self, repository: XSealRepository) -> None:
        raise NotImplementedError

    def install_premake_module(self, repository: XSealRepository) -> None:
        raise NotImplementedError
