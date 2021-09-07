import pycc


def hello_world():
    print("Hello from python")


class HelloWorld(pycc.PythonPluginInterface):
    def __init__(self):
        pycc.PythonPluginInterface.__init__(self)

    def getActions(self):
        return [
            pycc.Action(name="Hello World", target=hello_world)
        ]
