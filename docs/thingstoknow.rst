Things to know
==============

Memory management, object lifetime
----------------------------------

Python and C++ have a different memory handling mechanism,
python has a runtime and objects are garbage collected (reference count is used in the case of CPython)
while C++ has a minimal runtime and no garbage collection, this means that extra care needs to be taken when
using both languages at the same time, especially since CloudCompare also has its own way of managing objects
lifetime.

CloudCompare assumes ownership of the entities in its "DB Tree",