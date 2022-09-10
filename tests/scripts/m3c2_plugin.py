import pycc

try:
    from pycc.plugins import qM3C2
except ImportError:
    qM3C2 = None


def main():
    CC = pycc.GetCmdLineInstance()

    assert CC is not None
    cloudDescs = CC.clouds()
    print(cloudDescs)
    assert len(cloudDescs) == 1

    cloud = cloudDescs[0].pc

    m3c2_dialog = qM3C2.qM3C2Dialog(cloud, cloud)
    m3c2_dialog.setCorePointsCloud(cloud)

    # Dialog can be shown to chose options
    # m3c2_dialog.exe()

    allowsDialog = False;
    result = qM3C2.qM3C2Process.Compute(m3c2_dialog, allowsDialog);


if __name__ == '__main__':
    if qM3C2 is not None:
        main()
    else:
        exit(125)
