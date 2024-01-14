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

#ifndef PYTHON_PLUGIN_CCCORELIB_H
#define PYTHON_PLUGIN_CCCORELIB_H

#include <pybind11/pybind11.h>

#include <AutoSegmentationTools.h>
#include <DgmOctree.h>
#include <GenericDistribution.h>
#include <TrueKdTree.h>

PYBIND11_MAKE_OPAQUE(std::vector<unsigned>)
PYBIND11_MAKE_OPAQUE(CCCoreLib::ReferenceCloudContainer)

PYBIND11_MAKE_OPAQUE(CCCoreLib::DgmOctree::NeighbourCellsSet)
PYBIND11_MAKE_OPAQUE(CCCoreLib::DgmOctree::NeighboursSet)
PYBIND11_MAKE_OPAQUE(CCCoreLib::GenericDistribution::ScalarContainer)

PYBIND11_MAKE_OPAQUE(CCCoreLib::TrueKdTree::LeafVector)

#endif // PYTHON_PLUGIN_CCCORELIB_H
