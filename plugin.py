import pycc


pycc.RegisterAction("name", plugin_action)


def plugin_action(cc):
    pass


class pycc.PythonPlugin:
    """
    This is the interface a class-style plugin should implement 
    to be detected as a plugin

    This code shall be in C++ and follow the pybind11
    guideline to be properly subclassable
    """
    def setMainAppInterface(self, app) -> None:
        pass

    def registerActions(self) -> None:
        pass


class MyPythonPlugin(pycc.PythonPlugin):
    def __init__(self) -> None:
        super().__init__(self)
        self.count = 0
        self.app = None

    def setMainAppInterface(self, app) -> None:
        self.app = app

    def registerActions(self) -> None:
        pycc.RegisterAction("name", self.my_super_method)

    def my_super_method(self):
        count += 1
        print("Hello from Python, this action was used {self.count} times")
        
        

# How are Python plugins found and instanciated:
# 1. The user sets a path in some settings where python plugins can be found
#    (for testing purpose we should hardcod a default path that could aslo be kept later)
# 2. We list all .py files or dir (but we dont go inside dirs) in this dir
# 3. We import all these (How do we manage the interpreter state ??)
# 4. For class style plugin ->
#       a. Use pycc.PythonPlugin.__subclasses__ and use that to instanciate and and plugins to menu
# 5. For functions -> Users should use a pycc.RegisterAction so th fn are registered and added to the ui
# 6. Profit ?



# Use setuptools entry-namespace thing as a mechanism to load plugins ?
# Yes -> it is cool, but not as the only mechanism as it would require the plg
#        to be pip installable.


