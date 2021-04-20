from skbuild import setup

# The information here can also be placed in setup.cfg - better separation of
# logic and declaration, and simpler if you include description/version in a file.
setup(
    name="cccorelib",
    version="0.0.1",
    author="Thomas Montaigu",
    author_email="thomas.montaigu@laposte.net",
    description="Python bindings for CCCoreLib",
    packages=["cccorelib"],
    package_data={
      'cccorelib': ['*.pyi'],
    },
    include_package_data=True,
    cmake_args=['-GNinja'],
    install_requires=["numpy"],
    zip_safe=False,
)
