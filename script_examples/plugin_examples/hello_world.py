import pycc

def hello_world():
    print("Hello from python")

pycc.RegisterAction(name="Hello World", target=hello_world)