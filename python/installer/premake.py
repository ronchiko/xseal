import os
import subprocess

import installer.log
from installer.system_utils import is_windows
from installer.package.package_protocol import PackageProtocol
from installer.repository import XSealMainRepository, XSealRepository


_PREMAKE_ENVIRONMENT_VARIABLE_NAME = 'PREMAKE_PATH'

_REMOVE_PREMAKE_PATH = fr'REG DELETE HKCU\Environment /V {_PREMAKE_ENVIRONMENT_VARIABLE_NAME} /f'
_ADD_PREMAKE_PATH = fr'REG ADD HKCU\Environment /V {_PREMAKE_ENVIRONMENT_VARIABLE_NAME} /f /t {{type}} /d "{{value}}"'


def _update_premake_paths(new_paths):
    if _PREMAKE_ENVIRONMENT_VARIABLE_NAME in os.environ:
        subprocess.check_call(_REMOVE_PREMAKE_PATH)

    installer.log.write(f'Updating PREMAKE_PATH to be "{new_paths}"')
    subprocess.check_call(_ADD_PREMAKE_PATH.format(type='REG_EXPAND_SZ', value=new_paths))


def _should_extend_premake_environment_variable(path):
    path_string = os.environ.get(_PREMAKE_ENVIRONMENT_VARIABLE_NAME, '')
    paths = path_string.split(';')
    if '' in paths:
        paths.remove('')

    return path not in paths, ';'.join(paths + [path])


def _create_premake_repository():
    return XSealMainRepository().open_sub_repository('premake', create_if_missing=True) \
        if is_windows() else XSealRepository('/usr/local/share/premake')


def _install_on_windows(repository_path: str):
    should_update, new_paths = _should_extend_premake_environment_variable(repository_path)
    if should_update:
        _update_premake_paths(new_paths)


def install_premake_module(package: PackageProtocol):
    repository = _create_premake_repository()
    package.install_premake_module(repository)

    if is_windows():
        _install_on_windows(repository.path)
