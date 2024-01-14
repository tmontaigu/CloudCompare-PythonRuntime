// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonRuntime                       #
// #                                                                        #
// #  This program is free software; you can redistribute it and/or modify  #
// #  it under the terms of the GNU General Public License as published by  #
// #  the Free Software Foundation; version 2 of the License.               #
// #                                                                        #
// #  This program is distributed in the hope that it will be useful,       #
// #  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
// #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
// #  GNU General Public License for more details.                          #
// #                                                                        #
// #                   COPYRIGHT: Thomas Montaigu                           #
// #                                                                        #
// ##########################################################################

#include <GenericIndexedCloud.h>
#include <PointCloud.h>
#include <PointCloudTpl.h>

#include "wrappers.h"

void define_PointCloud(py::module &cccorelib)
{

    using CCCoreLib::GenericIndexedCloudPersist;
    using CCCoreLib::PointCloud;
    using CCCoreLib::PointCloudTpl;

    DEFINE_POINTCLOUDTPL(GenericIndexedCloudPersist, const char *, cccorelib, "__pointCloudTplCCCoreLib");
    py::class_<PointCloud, CCCoreLib::PointCloudTpl<GenericIndexedCloudPersist, const char *>>(cccorelib,
                                                                                               "PointCloud")
        .def(py::init<>())
        .def("reserveNormals", &PointCloud::reserveNormals, "newCount"_a)
        .def("addNormal", &PointCloud::addNormal, "normal"_a);
}
