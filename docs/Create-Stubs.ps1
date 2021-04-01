param(
    [Parameter(Mandatory = $true)]
    [String]$CloudCompareInstallFolder
)

# Generating requires the wrappers to have been build as non-embedded and without the runtime part
# the stubgen tool from mypy is used to generate the stubs

conda activate $CloudCompareInstallFolder/plugins/Python

$env:path += ";$CloudCompareInstallFolder;$CloudCompareInstallFolder/plugins"

stubgen -m cccorelib -m pycc -o stubfiles

# The runtime part of the pycc wrapper wannot be generated,
# so we wrote it by hand and append it to the generated pycc stub
Get-Content stubfiles/runtime.pyi | Out-File -Append stubfiles/pycc.pyi
