import pycc


def merge(clouds):
    total_num_points = sum(cloud.size() for cloud in clouds)

    merge_result = pycc.ccPointCloud("MergeResult")
    merge_result.reserve(total_num_points)

    for cloud_idx, cloud in enumerate(clouds):
        for point_idx in range(cloud.size()):
            merge_result.addPoint(cloud.getPoint(point_idx))

    pos = 0
    for cloud in clouds:
        for i in range(cloud.getNumberOfScalarFields()):
            scalarFieldName = cloud.getScalarFieldName(i)
            idx = merge_result.getScalarFieldIndexByName(scalarFieldName)

            if idx == -1:
                idx = merge_result.addScalarField(scalarFieldName)
                if idx == -1:
                    raise RuntimeError("Failed to add ScalarField")

            scalarField = cloud.getScalarField(i)
            sf = merge_result.getScalarField(idx)
            sf.asArray()[pos: pos + scalarField.size()] = scalarField.asArray()[:]
            sf.computeMinAndMax()

        pos += cloud.size()

    return merge_result


def main():
    CC = pycc.GetInstance()
    clouds = CC.getSelectedEntities()

    merged_cloud = merge(clouds)
    CC.addToDB(merged_cloud)


if __name__ == '__main__':
    main()
