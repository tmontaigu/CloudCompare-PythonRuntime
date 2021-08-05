param(
    [Parameter(Mandatory = $true)]
    [String]$CloudCompareExePath,
    [string]$BuildType = "html",
    [string]$OutputDir = "_build"
)

&doxygen .\qCC_db_doxygen_file
&doxygen .\CCCoreLib_doxygen_file
&"$CloudCompareExePath" -SILENT -PYTHON_SCRIPT build_doc.py -b $BuildType . $OutputDir
