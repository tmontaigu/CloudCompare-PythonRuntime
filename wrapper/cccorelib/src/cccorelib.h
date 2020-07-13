#ifndef CLOUDCOMPAREPROJECTS_CCCORELIB_H
#define CLOUDCOMPAREPROJECTS_CCCORELIB_H

#include <pybind11/pybind11.h>

#include <DgmOctree.h>
#include <GenericDistribution.h>
#include <TrueKdTree.h>
#include <AutoSegmentationTools.h>

PYBIND11_MAKE_OPAQUE(std::vector<unsigned>)
PYBIND11_MAKE_OPAQUE(CCCoreLib::ReferenceCloudContainer)

PYBIND11_MAKE_OPAQUE(CCCoreLib::DgmOctree::NeighbourCellsSet)
PYBIND11_MAKE_OPAQUE(CCCoreLib::DgmOctree::NeighboursSet)
PYBIND11_MAKE_OPAQUE(CCCoreLib::GenericDistribution::ScalarContainer)

PYBIND11_MAKE_OPAQUE(std::vector<int>)
PYBIND11_MAKE_OPAQUE(CCCoreLib::TrueKdTree::LeafVector)


#endif //CLOUDCOMPAREPROJECTS_CCCORELIB_H
