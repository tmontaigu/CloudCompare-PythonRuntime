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

#include <pybind11/pybind11.h>

#include <DistanceComputationTools.h>
#include <ReferenceCloud.h>
#include <ScalarField.h>
#include <GenericIndexedCloudPersist.h>
#include <PointCloud.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_DistanceComputationTools(py::module &cccorelib)
{
    py::class_<CCCoreLib::DistanceComputationTools> DistanceComputationTools(cccorelib,
                                                                             "DistanceComputationTools");

    py::class_<CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams>(
        DistanceComputationTools, "Cloud2CloudDistancesComputationParams")
        .def(py::init<>())
        .def_readwrite(
            "octreeLevel",
            &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::octreeLevel,
            R"doc(
	        Level of subdivision of the octree at witch to apply the distance computation algorithm
	        If set to 0 (default) the algorithm will try to guess the best level automatically.
	)doc")
        .def_readwrite(
            "maxSearchDist",
            &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::maxSearchDist,
            R"doc(
	        Maximum search distance (true distance won't be computed if greater)
	        Set to -1 to deactivate (default).

			Not compatible with closest point set determination (see CPSet)
	)doc")
        .def_readwrite(
            "multiThread",
            &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::multiThread,
            R"doc(
	        Whether to use multi-thread or single thread mode

			If maxSearchDist > 0, single thread mode will be forced.
	)doc")
        .def_readwrite(
            "maxThreadCount",
            &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::maxThreadCount,
            R"doc(
            Maximum number of threads to use (0 = max)
	)doc")
        .def_readwrite(
            "localModel",
            &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::localModel,
            R"doc(
            Type of local 3D modeling to use

			Default: NO_MODEL. Otherwise see CC_LOCAL_MODEL_TYPES.
	)doc")
        .def_readwrite("useSphericalSearchForLocalModel",
                       &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::
                           useSphericalSearchForLocalModel,
                       R"doc(
			Whether to use a fixed number of neighbors or a (sphere) radius for nearest neighbors search

			For local models only (i.e. ignored if localModel = NO_MODEL).
	)doc")
        .def_readwrite(
            "kNNForLocalModel",
            &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::kNNForLocalModel,
            R"doc(
			Number of neighbors for nearest neighbors search (local model)

			For local models only (i.e. ignored if localModel = NO_MODEL).
			Ignored if useSphericalSearchForLocalModel is true.
	)doc")
        .def_readwrite(
            "radiusForLocalModel",
            &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::radiusForLocalModel,
            R"doc(
			Radius for nearest neighbors search (local model)

			For local models only (i.e. ignored if localModel = NO_MODEL).
			Ignored if useSphericalSearchForLocalModel is true.
	)doc")
        .def_readwrite("reuseExistingLocalModels",
                       &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::
                           reuseExistingLocalModels,
                       R"doc(
			Whether to use an approximation for local model computation

			For local models only (i.e. ignored if localModel = NO_MODEL).
			Ignored if useSphericalSearchForLocalModel is true.
	)doc")
        .def_readwrite("CPSet",
                       &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::CPSet,
                       R"doc(
			Container of (references to) points to store the "Closest Point Set"

			The Closest Point Set corresponds to (the reference to) each compared point's closest neighbor.
			Not compatible with max search distance (see maxSearchDist)
	)doc")
        .def_property(
            "splitDistances",
            [](CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams &self) {
                return py::make_tuple(self.splitDistances[0], self.splitDistances[1], self.splitDistances[2]);
            },
            [](CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams &self,
               const py::sequence &list)
            {
                self.splitDistances[0] = list[0].cast<CCCoreLib::ScalarField *>();
                self.splitDistances[1] = list[1].cast<CCCoreLib::ScalarField *>();
                self.splitDistances[2] = list[2].cast<CCCoreLib::ScalarField *>();
            },
            R"doc(Split distances (one scalar field per dimension: X, Y and Z))doc")
        .def_readwrite(
            "resetFormerDistances",
            &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::resetFormerDistances,
            R"doc(
			 Whether to keep the existing distances as is (if any) or not

			By default, any previous distances/scalar values stored in the 'enabled' scalar field will be
			reset before computing them again.
	)doc");

    DistanceComputationTools.def_static("computeCloud2CloudDistances",
                                        &CCCoreLib::DistanceComputationTools::computeCloud2CloudDistances,
                                        "compareCloud"_a,
                                        "referenceCloud"_a,
                                        "params"_a,
                                        "progressCb"_a = nullptr,
                                        "compOctree"_a = nullptr,
                                        "refOctree"_a = nullptr,
                                        R"doc(
			Computes the 'nearest neighbor' distances between two point clouds (formerly named "Hausdorff distance")

			The main algorithm and its different versions (with or without local modeling) are described in
			Daniel Girardeau-Montaut's PhD manuscript (Chapter 2, section 2.3). It is the standard way to compare
			directly two dense point clouds.

			The current scalar field of the compared cloud should be enabled. By default it will be reset to
			NAN_VALUE but one can avoid this by defining the Cloud2CloudDistancesComputationParams::resetFormerDistances
			parameters to false. But even in this case, only values above Cloud2CloudDistancesComputationParams::maxSearchDist
			will remain untouched.

			Max search distance (Cloud2CloudDistancesComputationParams::maxSearchDist > 0) is not compatible with the
			determination of the Closest Point Set (Cloud2CloudDistancesComputationParams::CPSet)

			Parameters
			----------
			comparedCloud:the compared cloud (the distances will be computed for each point of this cloud)
			referenceCloud: the reference cloud (the nearest neigbhor will be determined among these points)
			params:	distance computation parameters
			progressCb:the client application can get some notification of the process progress through this callback mechanism (see GenericProgressCallback)
			compOctree:the pre-computed octree of the compared cloud (warning: both octrees must have the same cubical bounding-box - it is automatically computed if 0)
			refOctree: the pre-computed octree of the reference cloud (warning: both octrees must have the same cubical bounding-box - it is automatically computed if 0)

			Returns
			-------
			 0 if ok, a negative value otherwise
)doc");


	py::class_<CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams>(cccorelib, "Cloud2MeshDistancesComputationParams", R"doc(
		Cloud-to-mesh distances computation parameters
)doc")
	.def_readwrite("octreeLevel", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::octreeLevel,  R"doc(
		The level of subdivision of the octree at witch to apply the algorithm
)doc")
	.def_readwrite("maxSearchDist", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::maxSearchDist,  R"doc(
		Max search distance (acceleration)

		Default value: 0. If greater than 0, then the algorithm won't compute distances over this value
)doc")
	.def_readwrite("useDistanceMap", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::useDistanceMap, R"doc(
		Use distance map (acceleration)

		If true the distances will be approximated by a Distance Transform.

		Incompatible with signed distances or Closest Point Set.
)doc")
	.def_readwrite("signedDistances", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::signedDistances, R"doc(
		Whether to compute signed distances or not

		If true, the computed distances will be signed (in this case, the Distance Transform can't be used
		and therefore useDistanceMap will be ignored)
)doc")
	.def_readwrite("flipNormals", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::flipNormals, R"doc(
		Whether triangle normals should be computed in the 'direct' order (true) or 'indirect' (false)
)doc")
	.def_readwrite("multiThread", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::multiThread, R"doc(
		Whether to use multi-thread or single thread mode (if maxSearchDist > 0, single thread mode is forced)
)doc")
	.def_readwrite("maxThreadCount", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::maxThreadCount, R"doc(
		Maximum number of threads to use (0 = max)
)doc")
	.def_readwrite("CPSet", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::CPSet, R"doc(
		Cloud to store the Closest Point Set

		The cloud should be initialized but empty on input. It will have the same size as the compared cloud on output.

		Not compatible with maxSearchDist > 0.
)doc")
	.def_readwrite("robust", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::robust, R"doc(
		Whether to compute distances in a robust way (trying to detect edge cases)

		Computation will be slightly slower, and a little bit more memory will be required
)doc")
	.def(py::init<>());

	DistanceComputationTools.def_static("computeCloud2MeshDistances",
                                        &CCCoreLib::DistanceComputationTools::computeCloud2MeshDistances,
                                        "pointCloud"_a,
                                        "mesh"_a,
                                        "params"_a,
                                        "progressCb"_a = nullptr,
                                        "cloudOctree"_a = nullptr,
                                        R"doc(
			Computes the distances between a point cloud and a mesh

			The algorithm, inspired from METRO by Cignoni et al., is described
			in Daniel Girardeau-Montaut's PhD manuscript (Chapter 2, section 2.2).
			It is the general way to compare a point cloud with a triangular mesh.

			Parameters
			----------
			pointCloud: the compared cloud (the distances will be computed on these points)
			mesh: the reference mesh (the distances will be computed relatively to its triangles)
			params:	distance computation parameters
			progressCb:the client application can get some notification of the process progress through this callback mechanism (see GenericProgressCallback)
			cloudOctree: the pre-computed octree of the compared cloud
				(warning: its bounding box should be equal to the union of both point cloud and mesh bbs
				and it should be cubical - it is automatically computed if 0)

			Returns
			-------
			 0 if ok, a negative value otherwise
)doc");

    py::enum_<CCCoreLib::DistanceComputationTools::ERROR_MEASURES>(DistanceComputationTools,
                                                                   "ERRPOR_MEASURES")
        .value("RMS", CCCoreLib::DistanceComputationTools::ERROR_MEASURES::RMS)
        .value("MAX_DIST_68_PERCENT",
               CCCoreLib::DistanceComputationTools::ERROR_MEASURES::MAX_DIST_68_PERCENT)
        .value("MAX_DIST_95_PERCENT",
               CCCoreLib::DistanceComputationTools::ERROR_MEASURES::MAX_DIST_95_PERCENT)
        .value("MAX_DIST_99_PERCENT",
               CCCoreLib::DistanceComputationTools::ERROR_MEASURES::MAX_DIST_99_PERCENT)
        .value("MAX_DIST", CCCoreLib::DistanceComputationTools::ERROR_MEASURES::MAX_DIST)
        .export_values();
}
