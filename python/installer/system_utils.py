import os


def is_windows():
    _WINDOWS_PLATFORM_NAME = 'nt'
    return _WINDOWS_PLATFORM_NAME == os.name
