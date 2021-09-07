import pycc

from typing import List


class CountPlugin(pycc.PythonPluginInterface):
    def __init__(self):
        pycc.PythonPluginInterface.__init__(self)
        self.count = 0
        self.app = pycc.GetInstance()

    def getActions(self) -> List[pycc.Action]:
        return [
            pycc.Action(name="Print Count", target=self.printCount),
            pycc.Action(name="Print Selected Entities", target=self.printSelectedEntities),
        ]

    def printCount(self):
        self.count += 1
        print(f"This function has been called {self.count} times")

    def printSelectedEntities(self):
        print(f"Selected Entities: {self.app.getSelectedEntities()}")
