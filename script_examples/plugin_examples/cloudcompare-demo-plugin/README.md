# cloudcompare-demo-plugin

A simple demo plugin for CloudCompare-PythonPlugin. It showcases the autodiscovering mechanism.
The entry point is defined in the `pyproject.toml` file.

```toml
[project.entry-points."cloudcompare.plugins"]
autodiscovered-hello = "cloudcompare_demo_plugin.hello:HelloWorld"
```
-----

**Table of Contents**

- [Installation](#installation)
- [License](#license)

## Installation

```console
pip install cloudcompare-demo-plugin
```

## License

`cloudcompare-demo-plugin` is distributed under the terms of the [GPL3+](https://spdx.org/licenses/GPL-3.0-or-later.html) license.
