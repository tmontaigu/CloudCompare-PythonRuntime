#include <pybind11/pybind11.h>

#include <ccPickingHub.h>
#include <ccPickingListener.h>

namespace py = pybind11;
using namespace pybind11::literals;

// Trampoline class to allow Python classes to inherit from ccPickingListener
class PickingListenerTrampoline : public ccPickingListener
{
  public:
    /* Inherit the constructors */
    using ccPickingListener::ccPickingListener;

    virtual ~PickingListenerTrampoline() = default;

    void onItemPicked(const PickedItem &pickedItem) override
    {
        PYBIND11_OVERRIDE_PURE(void,              /* Return type */
                               ccPickingListener, /* Parent class */
                               onItemPicked, /* Name of function in C++ (must match Python name) */
                               pickedItem    /* Argument(s) */
        );
    }
};

void define_ccPickingHub(py::module &m)
{
    py::class_<ccPickingListener, PickingListenerTrampoline> listenerClass(
        m, "ccPickingListener", R"doc(
        Point/triangle picking listener interface
        
        .. code:: Python

            import pycc

            class PickingListener(pycc.ccPickingListener):
                def __init__(self):
                    super().__init__()
        )doc");
    listenerClass.def(py::init<>())
        .def("onItemPicked",
             &ccPickingListener::onItemPicked,
             "pi"_a,
             R"doc(Method called whenever an item is picked)doc");

    py::class_<ccPickingListener::PickedItem>(listenerClass, "PickedItem")
        .def_readonly("clickPoint",
                      &ccPickingListener::PickedItem::clickPoint,
                      R"doc(Position of the user click)doc")
        .def_readonly("entity",
                      &ccPickingListener::PickedItem::entity,
                      R"doc(Picked entity (if any))doc",
                      py::return_value_policy::reference)
        .def_readonly("itemIndex",
                      &ccPickingListener::PickedItem::itemIndex,
                      R"doc(e.g. point or triangle index)doc")
        .def_readonly(
            "P3D", &ccPickingListener::PickedItem::P3D, R"doc(Picked point in 3D (if any))doc")
        .def_readonly("uvw",
                      &ccPickingListener::PickedItem::uvw,
                      R"doc(Picked point barycentric coordinates (if picked on a triangle))doc")
        .def_readonly(
            "entityCenter",
            &ccPickingListener::PickedItem::entityCenter,
            R"doc(The point doesn't correspond to a real 'item' but to the entity center)doc");

    py::class_<ccPickingHub>(m, "ccPickingHub", R"doc(Point/triangle picking hub)doc")
        .def("listenerCount",
             &ccPickingHub::listenerCount,
             R"doc(Returns the number of currently registered listeners)doc")
        .def(
            "addListener",
            [](ccPickingHub &self,
               py::object &pyListener,
               bool exclusive = false,
               bool autoStartPicking = true,
               ccGLWindowInterface::PICKING_MODE mode =
                   ccGLWindowInterface::POINT_OR_TRIANGLE_PICKING)
            {
                auto *listener = pyListener.cast<ccPickingListener *>();
                if (!self.addListener(listener, exclusive, autoStartPicking, mode))
                {
                    throw std::runtime_error("Failed to add listener");
                }
                pyListener.inc_ref();
            },
            "listener"_a,
            "exclusive"_a = false,
            "autoStartPicking"_a = true,
            "mode"_a = ccGLWindowInterface::POINT_OR_TRIANGLE_PICKING,
            R"doc(
            Adds a listener

            Parameters
            ----------

            listener: listener to be registered
            exclusive: prevents new listeners from registering
            autoStartPicking: automatically enables the picking mode on the active window (if any)
            mode: sets the picking mode (warning: may be rejected if another listener is currently registered with another mode)

            Raises
            ------

            RuntimeError: if failed to add listener
            )doc")
        .def(
            "removeListener",
            [](ccPickingHub &self, py::object &pyListener, bool autoStopPickingIfLast = true)
            {
                auto *listener = pyListener.cast<ccPickingListener *>();
                self.removeListener(listener);
                // TODO if the user did not give a pyListener that was previously added, this will
                // crash we wrongly dec ref, CC should return true/false to tell if it was removed
                pyListener.dec_ref();
            },
            "listener"_a,
            "autoStopPickingIfLast"_a = true,
            R"doc(
                 Removes a listener

                 Parameters
                 ----------

                listener: listener to be removed
                autoStopPickingIfLast: automatically disables the picking mode on the active window (if any) if no other listener is registered
            )doc")
        .def("togglePickingMode",
             &ccPickingHub::togglePickingMode,
             "state"_a,
             R"doc(Manual start / stop of the picking mode on the active window)doc")
        .def("activeWindow",
             &ccPickingHub::activeWindow,
             R"doc(Returns the currently active window)doc")
        .def(
            "isLocked",
            &ccPickingHub::isLocked,
            R"doc(Returns whether the picking hub is locked  (i.e. an exclusive listener is registered))doc");
}