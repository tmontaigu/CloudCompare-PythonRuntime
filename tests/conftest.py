# from https://icetutor.com/question/how-to-pass-arguments-in-pytest-by-command-line/
# https://stackoverflow.com/questions/40880259/how-to-pass-arguments-in-pytest-by-command-line

def pytest_addoption(parser):
    parser.addoption("--cloudcompare_exe", action="store")


def pytest_generate_tests(metafunc):
    # This is called for every test. Only get/set command line arguments
    # if the argument is specified in the list of test "fixturenames".
    option_value = metafunc.config.option.cloudcompare_exe
    if 'cloudcompare_exe' in metafunc.fixturenames and option_value is not None:
        metafunc.parametrize("cloudcompare_exe", [option_value])
