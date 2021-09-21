import os
import sys

print(sys.argv)

assert sys.argv[0] == __file__, "arv0 not equal"
assert sys.argv[1] == "--city", "argv1 not equal"
assert sys.argv[2] == "dalaran", "argv2 not equal"
