import os
import sys

# FIXME Seems like __file__ in embedded is not defined
assert sys.argv[0] == os.path.join(os.getcwd(), "tests", "scripts", "cmdline_args.py")
assert sys.argv[1] == "--city"
assert sys.argv[2] == "dalaran"
