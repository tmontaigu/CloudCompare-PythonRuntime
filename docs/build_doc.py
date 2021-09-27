""" This script calls Sphinx's main function to build the docs, thus it takes
the exact same argument as the sphinx-build does.


This script is meant to be run from CloudCompare's commandline mode with the
-PYTHON_SCRIPT command.

We need to build docs this way, because the pycc
module needs the CC runtime to be running in order to be properly
imported, which is not the case when running sphinx-build
"""

from sphinx.cmd.build import main
import sys

code = main(sys.argv[1:])
if code != 0:
    sys.exit(code)
