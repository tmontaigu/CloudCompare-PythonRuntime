import pytest
import sys
import os

abspath = os.path.dirname(os.path.abspath(__file__))

if __name__ == '__main__':
    retcode = pytest.main([os.path.join(abspath, "../wrapper/cccorelib/tests")])
    if retcode != 0:
        sys.exit(retcode)

    retcode = pytest.main([os.path.join(abspath, "../wrapper/pycc/tests")])
    if retcode != 0:
        sys.exit(retcode)
