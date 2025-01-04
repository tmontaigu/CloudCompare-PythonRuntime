import pycc

MAIN_ICON_BYTES = b'\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00 \x00\x00\x00 \x08\x06\x00\x00\x00szz\xf4\x00\x00\x00\tpHYs\x00\x00\x0e\xc4\x00\x00\x0e\xc4\x01\x95+\x0e\x1b\x00\x00\x00rIDATX\x85\xed\xd5\xb1\r\x80 \x10\x85\xe1\xdf\x0b\x05\x03XX\xb8\xae\xab\xb8\x8e\xa5k\x18\x060\x94Z\x90\xd0x\x9e\x89\xefU\x84\xe6>\xc8#\x0c\xeb\xbel\xc0\x04$\xc0\xb8r_\xb7r>\xb0W\x120\x03cg\x98Wr\xef\x94\xee1\xfaW\xed\x0e\x08\x8d\x00\xea\x80\x00\x02\x18\xed\xcf\xe2U@h\x04\x10@\xaf@\x80O\x00TB\x01\xc2\x01\xff.a\x02\n\x90\x83\xe6\x1f\x15\x88\xd1\x0e=Zl\x86\xd1\x00\x00\x00\x00IEND\xaeB`\x82'

ICON_BYTES = b'\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00 \x00\x00\x00 \x08\x06\x00\x00\x00szz\xf4\x00\x00\x00\tpHYs\x00\x00\x0e\xc4\x00\x00\x0e\xc4\x01\x95+\x0e\x1b\x00\x00\x00nIDATX\x85\xed\xd51\x0e\x80 \x10D\xd1/\xd9\x82\x03XXx\xff\xebx\x15\xc3\x01\xc4R\x0b\x92m\\\xd7\xc4\x99\x8a\xd0\xec\x83\x0ca\xea\x9d\rX\x00\x03\nW\xee\xebQ\x8e\x07\xf6\x9a\x01+0;\xc3\xa2R\xbdS\x86\xa7\xe0_u8 5\x02\xa8\x03\x02\x08P\x18\x7f\x16\xaf\x02R#\x80\x00z\x05\x02|\x02\xa0\x12\n\x90\x0e\xf8w\t\rh@M\x9a\xbf\x9fI7\x0e9\x19U)\xd1\x00\x00\x00\x00IEND\xaeB`\x82'



class PickingListener(pycc.ccPickingListener):
    def __init__(self):
        super().__init__()

    def onItemPicked(self, item):
         # Use ccLog instead of Python's print to display messages
         # in CC's console
         pycc.ccLog.Print("onItemPicked called")
         pycc.ccLog.Print(f"clickPoint: {item.clickPoint.x(), item.clickPoint.y()}")
         pycc.ccLog.Print(f"entity: {item.entity}")
         pycc.ccLog.Print(f"itemIndex: {item.itemIndex}")
         pycc.ccLog.Print(f"P3D: {item.P3D}")        
         pycc.ccLog.Print(f"uvw: {item.uvw}")
         pycc.ccLog.Print(f"entityCenter: {item.entityCenter}")



class PickerPlugin(pycc.PythonPluginInterface):
    def __init__(self):
        pycc.PythonPluginInterface.__init__(self)
        self.app = pycc.GetInstance()
        self.listener = PickingListener()
        self.isListening = False

    def startPicking(self):
        if self.isListening:
            return
        self.app.pickingHub().addListener(self.listener)
        self.isListening = True

    def stopPicking(self):
        if not self.isListening:
            return
        self.app.pickingHub().removeListener(self.listener)
        self.isListening = False


    def getIcon(self):
        return MAIN_ICON_BYTES

    def getActions(self):
        return [
            pycc.Action(
                name="Start Picking",
                target=self.startPicking,
                icon=(ICON_BYTES, "PNG")
            ),
            pycc.Action(
                name="Stop Picking",
                target=self.stopPicking,
                icon=(ICON_BYTES, "PNG")
            )
        ]
