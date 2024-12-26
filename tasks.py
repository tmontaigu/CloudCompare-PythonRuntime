from invoke import task
from pathlib import Path

try:
    from tqdm import tqdm
except ModuleNotFoundError:
    def tqdm(iter, *args, **kwargs):
        return iter
from typing import List, Iterable

import itertools

CLANG_FORMAT_CMD = "clang-format -i {}"

# List of folder names to be ignoreed when formatting
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
    sources = Path('./src/').rglob("*.cpp")
    headers = Path('./src/').rglob("*.h")

    wrapper_sources =  Path('./wrapper/').rglob("*.cpp")
    wrapper_headers =  Path('./wrapper/').rglob("*.h")


    all_sources = filter_paths(itertools.chain(sources, headers, wrapper_headers, wrapper_sources))

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

VENV_PYTHON_EXEC = "./venv/bin/python"

@task
def create_venv(c):
    if not Path("venv").exists():
        c.run("python3 -m venv venv")
        c.run(f"{VENV_PYTHON_EXEC} -m pip install -r requirements-dev.txt")
    else:
        print("venv already exists")

@task
def install_ccorelib(c):
    c.run(f"{VENV_PYTHON_EXEC} -m pip install --verbose wrapper/cccorelib")

@task
def install_pycc(c):
    c.run(f"{VENV_PYTHON_EXEC} -m pip install --verbose wrapper/pycc")

@task
def test_cccorelib(c):
    c.run(f"{VENV_PYTHON_EXEC} -m pytest wrapper/cccorelib/tests")

@task
def test_pycc(c):
    c.run(f"{VENV_PYTHON_EXEC} -m pytest wrapper/pycc/tests")
