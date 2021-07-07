from invoke import task
from pathlib import Path
from tqdm import tqdm

import itertools
CLANG_FORMAT_CMD = "clang-format -i {}"

@task
def clang_format(c):
    sources = Path('.').rglob("*.cpp")
    headers = Path('.').rglob("*.h")

    all_sources = list(itertools.chain(sources, headers))

    print('Formating source files')
    for path in tqdm(all_sources, ncols=80):
        c.run(f"clang-format -i {path}")


@task
def cmake_format(c):
    cmakelists = list(Path('.').rglob('CMakeLists.txt'))
    print('Formating CMakeLists.txt')
    for path in tqdm(cmakelists, ncols=80):
        c.run(f'cmake-format -i {path}')


@task(clang_format, cmake_format)
def format(c):
    ...
