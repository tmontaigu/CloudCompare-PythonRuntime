Runtime
=======


.. important::

    As the functions and types below allow to interact with the CloudCompare app,
    they are only available in the python embedded within the plugin and not available
    when using standalone pycc module.


.. autofunction:: pycc.GetInstance

    This function returns an object that allows to interact with the running CC instance
    to get access to already loaded PointCloud and Meshes.

    .. warning::

        on command line mode, the object returned will be of type :class:`pycc.ccCommandLineInterface`.

        on gui mode, the object returned will be of type :class:`pycc.ccPythonInstance`


.. autofunction:: pycc.GetCmdLineInstance

    Returns the :class:`pycc.ccCommandLineInterface` object if the script is ran from
    the command line mode, otherwise returns `None`.

.. autofunction:: pycc.GetGUIInstance

    Returns the :class:`pycc.ccPythonInstance` object if the script is ran from
    the command line mode, otherwise returns `None`.

.. autofunction:: pycc.RegisterAction


.. autoclass:: pycc.PythonPluginInterface
    :members:
    :undoc-members:


.. autofunction:: pycc.RunInThread

    Runs the function in a different thread to keep the UI
    responsive and returns the result of the function when the
    thread is done.

    Example:

    .. code:: Python

        import time

        def expensive_sum(a, b):
            time.sleep(10)
            return a + b

        # if we ran expensive_sum directly, CloudCompare would
        # become unresponsive for 10 seconds
        result = pycc.RunInThread(expensive_sum, 1, 2)
        assert result == 3



ccPythonInstance
________________

.. autoclass:: pycc.ccPythonInstance
    :members:
    :undoc-members:

ccCommandLineInterface
______________________

.. autoclass:: pycc.ccCommandLineInterface
    :members:
    :undoc-members:
