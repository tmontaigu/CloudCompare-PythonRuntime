from skbuild import setup

# The information here can also be placed in setup.cfg - better separation of
# logic and declaration, and simpler if you include description/version in a file.
setup(
    name="pycc",
    version="0.0.1",
    author="Thomas Montaigu",
    author_email="thomas.montaigu@laposte.net",
    description="Python bindings for CloudCompare libs",
    long_description="",
    packages=["pycc"],
    cmake_args=['-GNinja', '-DOPTION_BUILD_CCVIEWER=OFF'],
    zip_safe=False,
)
