# importing our class in the top level __init__ is
# mandatory, so that after importing the module
# the `CountPlugin` class exists in the Python type system
# and the PythonPlugin can import & instantiate it
from .myplugin import CountPlugin
