from conans import ConanFile, CMake, tools

class AfvNativeConan(ConanFile):
    name = "AFV-Native"
    version = "0.9.2"
    license = "3-Clause BSD"
    author = "Chris Collins <kuroneko@sysadninjas.net>"
    url = "https://github.com/xsquawkbox/AFV-Native"
    description = "Portable, Native Implementation of the AFV Interface"
    topics = ("vatsim", "afv", "voice", "portable")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "*:shared": False,
        "*:fPIC": True,
        "libcurl:with_openssl": True,
        "libevent:with_openssl": False,
        "libsoundio*:enable_jack": False,
        "libsoundio*:enable_pulseaudio": True,
        "libsoundio*:enable_alsa": True,
    }
    generators = "cmake"
    requires = [
        "msgpack/3.2.0@bincrafters/stable",
        "jsonformoderncpp/3.7.0@vthiery/stable",
        "openssl/1.1.1d",
        "libcurl/7.66.0@bincrafters/stable",
        "libevent/2.1.11@bincrafters/stable",
        "libopus/1.3.1@xsquawkbox/testing",
        "speexdsp/1.2.0@xsquawkbox/devel",
        "libsoundio/2.0.0@xsquawkbox/devel",
    ]
    build_requires = [
        "gtest/1.8.1@bincrafters/stable",
        "glew/2.2.0rc2@xsquawkbox/devel", # used for the test application.
        "sdl2/2.0.9@bincrafters/stable", # also used for the test application
    ]
    exports_sources = [
        "docs/*",
        "!docs/api/*",
        "examples/*",
        "extern/*",
        "!extern/*/.git",
        "include/*",
        "src/*",
        "test/*",
        "CMakeLists.txt",
        "Doxyfile",
        "README.md",
        "COPYING.md",
        "!*/imgui.ini",
        "!*/afv.log",
    ]

    def imports(self):
        #self.copy("*.dll", "bin", "bin")
        #self.copy("*.dylib", "lib", "lib")
        #self.copy("*.so*", "lib", "lib")
        if self.settings.build_type == 'Debug':
            self.copy("*.pdb", "lib", "bin")

    def configure(self):
        if self.settings.os == 'Windows':
            self.options['libcurl'].with_winssl = False
        elif self.settings.os == 'Macos':
            self.options['libcurl'].darwin_ssl = False

    def source(self):
        pass

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["afv_native"]
