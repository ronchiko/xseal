import argparse


from installer.install import Installer, InstallerConfiguration
from installer.package.package_protocol import PackageProtocol
from installer.package.local_package import LocalPackage


def _parse_command_line_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('package_path', type=str, help='The path to engines source')
    parser.add_argument('distribution', type=str, help='The alias to this engines version')
    parser.add_argument('--dev', action='store_true', help='Should install the engine in development mode')
    parser.add_argument('--reinstall', action='store_true', help='Should re-install this version')

    return parser.parse_args()


def _make_package(arguments) -> PackageProtocol:
    if arguments.dev:
        return LocalPackage(
            source_path=arguments.package_path
        )

    raise NotImplementedError


def _make_configurations(arguments):
    return InstallerConfiguration(
        reinstall=arguments.dev or arguments.reinstall,
        virtual=arguments.dev
    )


def main():
    arguments = _parse_command_line_arguments()
    package = _make_package(arguments)
    configuration = _make_configurations(arguments)

    installer = Installer(
        name=arguments.distribution,
        package=package,
        configuration=configuration
    )
    installer.run()


if __name__ == '__main__':
    main()
