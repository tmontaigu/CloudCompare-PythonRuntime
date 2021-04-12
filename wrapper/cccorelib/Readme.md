# Building the wheel

First set `CMAKE_PREFIX_PATH` with the path to Qt
```pwsh
$env:CMAKE_PREFIX_PATH+=";C:\Qt\5.15.2\msvc2019_64"
``` 

Then

```
# (add -v to get a more vebose output to debug)
pip wheel .

# or directly install
pip install .
```