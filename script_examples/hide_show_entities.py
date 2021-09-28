""" Hides all Selected entities, and re-shows them with a delay
"""
import cccorelib
import pycc

from time import sleep

cc = pycc.GetInstance()


entities = cc.getSelectedEntities()

for entity in entities:
    entity.setVisible(False)

cc.updateUI()
cc.redrawAll()

pycc.RunInThread(sleep, 5)

for entity in entities:
    entity.setVisible(True)

    cc.updateUI()
    cc.redrawAll()

    print(f"{entity} shown")
    pycc.RunInThread(sleep, 5)

print("Done")
