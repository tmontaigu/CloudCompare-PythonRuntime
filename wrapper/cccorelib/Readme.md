# Building the wheel

```
# First set `CMAKE_PREFIX_PATH` with the path to Qt
$env:CMAKE_PREFIX_PATH+=";C:\Qt\5.15.2\msvc2019_64"

# (add -v to get a more vebose output to debug)
pip wheel .

# or directly install
pip install .
```

Or

```pwsh
python .\setup.py install -- -DCMAKE_PREFIX_PATH="C:\Qt\5.15.2\msvc2019_64"
```
