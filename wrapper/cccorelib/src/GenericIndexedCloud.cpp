#include <pybind11/pybind11.h>

#include <GenericIndexedCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_GenericIndexedCloud(py::module &cccorelib)
{
	py::class_<CCCoreLib::GenericIndexedCloud, CCCoreLib::GenericCloud>(cccorelib, "GenericIndexedCloud")
			.def("getPoint", [](CCCoreLib::GenericIndexedCloud &self, const unsigned index)
			{
				if ( index < self.size())
				{
					return *self.getPoint(index);
				} else
				{
					throw std::out_of_range(std::string("index ") + std::to_string(index) + " is out of range");
				}
			})
			.def("getPoint", [](CCCoreLib::GenericIndexedCloud &self, const unsigned index, CCVector3 &P)
			{
				if ( index < self.size())
				{
					self.getPoint(index, P);
				} else
				{
					throw std::out_of_range(std::string("index ") + std::to_string(index) + " is out of range");
				}
			});
}