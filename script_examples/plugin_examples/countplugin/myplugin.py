import pycc


class CountPlugin(pycc.PythonPluginInterface):
    def __init__(self):
        pycc.PythonPluginInterface.__init__(self)
        self.count = 0
        self.app = pycc.GetInstance()

    def registerActions(self):
        pycc.RegisterAction(name='Print Count', target=self.printCount)
        pycc.RegisterAction(name='Print Selected Entities', target=self.printSelectedEntities)

    def printCount(self):
        self.count += 1
        print(f"This function has been called {self.count} times")

    def printSelectedEntities(self):
        print(f"Selected Entities: {self.app.getSelectedEntities()}")