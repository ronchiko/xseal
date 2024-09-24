import dataclasses
import json
from typing import Dict, Any

from installer.repository import XSealMainRepository

_VERSION_FILE_NAME = 'XSealVersions'

VersionsJson = Dict[str, Dict[str, Any]]


def _load_version_file_from_repository(repository: XSealMainRepository) -> VersionsJson:
    """
    Attempts to load the version files content from a repository.
    If the file doesn't exist, returns an empty dict.
    """
    if not repository.has_file(_VERSION_FILE_NAME):
        return {}

    with repository.open_file(_VERSION_FILE_NAME, 'r') as repository_stream:
        return json.load(repository_stream)


@dataclasses.dataclass
class XSealVersion:
    name: str
    path: str
    is_development_version: bool


class XSealVersionFile:
    """
    A virtual representation of XSeal's version file.

    To apply the changes made in this class, the `save` method must be called.
    """
    def __init__(self):
        self._repository = XSealMainRepository()
        self._data = _load_version_file_from_repository(self._repository)

    def is_version_defined(self, version) -> bool:
        """
        Checks if a given version is already registered in the version file.
        """
        return version.lower() in self._data

    def define_version(self, version: str, root_path: str, is_development: bool = False):
        """
        Defines a new version in this virtual versions file.
        """
        self._data[version] = {
            'path': root_path,
            'dev': int(is_development)
        }

    def __getitem__(self, version_name: str) -> XSealVersion:
        version_tree = self._data[version_name]
        return XSealVersion(
            name=version_name,
            path=version_tree['path'],
            is_development_version=bool(version_tree['dev'])
        )

    def save(self):
        """
        Commits the changes done in this class to the disk.
        """
        with self._repository.open_file(_VERSION_FILE_NAME, 'w+') as versions_stream:
            json.dump(self._data, versions_stream, indent=4)
