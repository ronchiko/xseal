import dataclasses
import shutil

import installer.log
from installer.version import XSealVersionFile
from installer.repository import XSealMainRepository, XSealRepository, XSealInstallsRepository
from installer.package.package_protocol import PackageProtocol
from installer.premake import install_premake_module


class InstallerError(Exception):
    pass


@dataclasses.dataclass
class InstallerConfiguration:
    reinstall: bool
    virtual: bool

    @property
    def should_reinstall(self):
        return self.reinstall


class Installer:
    def __init__(self, name: str, package: PackageProtocol, configuration: InstallerConfiguration):
        self.__name = name
        self.__package = package
        self.__configuration = configuration
        self.__versions = XSealVersionFile()

    def run(self):
        self._delete_old_version_if_needed()
        package_repository = self._create_package_repository()

        installer.log.write(f'Creating package files at: {package_repository.path}')
        self.__versions.define_version(self.__name, package_repository.path)
        self.__package.create_package_files(package_repository)

        installer.log.write('Installing premake modules')
        install_premake_module(self.__package)

        installer.log.write(f'Successfully installed XSeal {self.__name}')
        self.__versions.save()

    def _create_package_repository(self) -> XSealRepository:
        if self.__configuration.virtual:
            main_repository = XSealMainRepository()
            version_repository = main_repository.open_sub_repository('versions', create_if_missing=True)
            return version_repository.open_sub_repository(self.__name, create_if_missing=True)

        return XSealInstallsRepository().open_sub_repository(self.__name, create_if_missing=True)

    def _delete_old_version_if_needed(self):
        if not self.__versions.is_version_defined(self.__name):
            return

        if not self.__configuration.should_reinstall:
            raise InstallerError(
                f"A version of XSeal with the name '{self.__name}' is already installed!\n\n"
                f"Please use the --reinstall option if you want to reinstall"
            )

        self._force_delete_old_version()

    def _force_delete_old_version(self):
        """
        Deletes the older version of this installment
        """
        _old_version_path = self.__versions[self.__name].path
        installer.log.write(f'Deleting old version of XSeal at path: {_old_version_path}')
        shutil.rmtree(_old_version_path)
