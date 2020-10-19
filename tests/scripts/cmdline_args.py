import os
import sys

print(sys.argv)
# FIXME Seems like __file__ in embedded is not defined
assert sys.argv[0] == os.path.join(os.getcwd(), "tests", "scripts", "cmdline_args.py"), "arv0 not equal"
assert sys.argv[1] == "--city", "argv1 not equal"
assert sys.argv[2] == "dalaran", "argv2 not equal"
