from invoke import task
from pathlib import Path
from tqdm import tqdm
from typing import List, Iterable

import itertools
CLANG_FORMAT_CMD = "clang-format -i {}"

DISALLOW_LIST = ['_skbuild']


def filter_paths(paths: Iterable[Path]) -> List[Path]:
    filtered_paths = []

    for path in paths:
        allowed = True
        for p in DISALLOW_LIST:
            if p in path.parts:
                allowed = False

        if allowed:
            filtered_paths.append(path)

    return filtered_paths


@task
def clang_format(c):
    sources = Path('.').rglob("*.cpp")
    headers = Path('.').rglob("*.h")

    all_sources = filter_paths(itertools.chain(sources, headers))

    print('Formating source files')
    for path in tqdm(all_sources, ncols=80):
        c.run(f"clang-format -i {path}")


@task
def cmake_format(c):
    cmakelists = list(Path('.').rglob('CMakeLists.txt'))
    cmakes = list(Path("./cmake").iterdir())

    all_cmakes = filter_paths(itertools.chain(cmakes, cmakelists))

    print('Formating CMakeLists.txt')
    for path in tqdm(all_cmakes, ncols=80):
        c.run(f'cmake-format -i {path}')


@task(clang_format, cmake_format)
def format(c):
    ...
