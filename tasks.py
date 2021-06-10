from invoke import task
from pathlib import Path

import itertools
CLANG_FORMAT_CMD = "clang-format -i {}"

@task
def clang_format(c):
    sources = Path('.').rglob("*.cpp")
    headers = Path('.').rglob("*.h")
    for path in itertools.chain(sources, headers):
        c.run(f"clang-format -i {path}")


@task
def cmake_format(c):
    cmakelists = Path('.').rglob('CMakeLists.txt')
    for path in cmakelists:
        c.run(f'cmake-format -i {path}')


@task(clang_format, cmake_format)
def format(c):
    ...
