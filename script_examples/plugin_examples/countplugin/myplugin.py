import pycc

from typing import List
from pathlib import Path

MAIN_ICON_PATH = str(Path(__file__).parent / "orange-square.png")
COUNT_ICON_BYTES = b'\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00 \x00\x00\x00 \x08\x06\x00\x00\x00szz\xf4\x00\x00\x00\tpHYs\x00\x00\x0e\xc4\x00\x00\x0e\xc4\x01\x95+\x0e\x1b\x00\x00\x00oIDATX\x85\xed\xd51\x0e\x80 \x10D\xd1\xeff\x0b\x0e`a\xe1a\xb9\xa1W1\x1c\xc0XjAB\xe3\xba&\xceT\x84f\x1fd\x08S\xa5n\xc0\x028`\\\xb9\xaf{9\x1e\xd8k\x0e\xac\xc0<\x18\x16\x952:ex\x8c\xf1U\x87\x03R#\x80: \x80\x00F\xff\xb3x\x15\x90\x1a\x01\x04\xd0+\x10\xe0\x13\x00\x95P\x80t\xc0\xbfK\xe8@\x03J\xd2\xfc\xfd\x04I\x84\r;\xd4\xf3>\xb5\x00\x00\x00\x00IEND\xaeB`\x82'
SELECTED_ENTITIES_ICON_PATH = str(Path(__file__).parent / "blue-square.png")


class CountPlugin(pycc.PythonPluginInterface):
    def __init__(self):
        pycc.PythonPluginInterface.__init__(self)
        self.count = 0
        self.app = pycc.GetInstance()

    def getIcon(self):
        return MAIN_ICON_PATH

    def getActions(self) -> List[pycc.Action]:
        return [
			# If no format is specified for the icon, Qt will try to guess is
            pycc.Action(name="Print Count", target=self.printCount, icon=COUNT_ICON_BYTES),
            pycc.Action(name="Print Selected Entities", target=self.printSelectedEntities, icon=SELECTED_ENTITIES_ICON_PATH),
        ]

    def printCount(self):
        self.count += 1
        print(f"This function has been called {self.count} times")

    def printSelectedEntities(self):
        print(f"Selected Entities: {self.app.getSelectedEntities()}")
