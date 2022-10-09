plugins
=======

qM3C2
-----

When compiled with the ``PLUGIN_STANDARD_QM3C2=ON``
cmake variable, the wrappers that makes CloudCompare's qM3C2
available through python will be compiled and installed.


.. note::

    In stand alone mode, this plugin requires a ``QApplication``
    to be running. see :func:`pycc.InitQApplication`
    (In plugin mode, CloudCompare is the running ``QApplication``)


qM3C2Dialog
___________

.. autoclass:: pycc.plugins.qM3C2.qM3C2Dialog
    :members:
    :undoc-members:
    :show-inheritance:


qM3C2Process
___________

.. autoclass:: pycc.plugins.qM3C2.qM3C2Process
    :members:
    :undoc-members:

