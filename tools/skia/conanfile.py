"""
编写Skia的跨平台打包脚本

参考文档

https://docs.conan.io/en/latest/
"""
from conans import ConanFile, CMake, tools


class SkiaConan(ConanFile):
    name = 'Skia'
    version = '0.1.0'
    license = 'LICENSE'
    url = 'https://skia.org/'
    description = 'Skia Graphics Library'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {'shared': [True, False]}
    default_options = {'shared': False}
    generators = 'cmake'

    def source(self):
        return None

    def configure_ninja(self):
        if self.settings.os not in ['iOS', 'Macos', 'Linux']:
            raise Exception("Binary does not exist for these settings")

        return None

    def build(self):
        return None

    def package(self):
        return None

    def package_info(self):
        return None
