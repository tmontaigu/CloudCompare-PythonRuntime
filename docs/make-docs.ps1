param(
    [Parameter(Mandatory = $true)]
    [String]$CloudCompareExePath,
    [string]$BuildType = "html",
    [string]$OutputDir = "_build"
)

&"$CloudCompareExePath" -SILENT -PYTHON_SCRIPT build_doc.py -b $BuildType . $OutputDir
