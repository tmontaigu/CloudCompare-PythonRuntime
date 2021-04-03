param(
    [Parameter(Mandatory = $true)]
    [String]$CloudCompareInstallFolder
)

# Generating requires the wrappers to have been build as non-embedded and without the runtime part
# the stubgen tool from mypy is used to generate the stubs


$SavedLoc = Get-Location
$StubGenLoc = Join-Path Get-Location stubfiles


$PluginsFolder = Join-Path -Resolve $CloudCompareInstallFolder plugins
$PythonEnvDir = Join-Path -Resolve $PluginsFolder Python

&conda activate $PythonEnvDir

$env:path += ";$CloudCompareInstallFolder;$PluginsFolder;$PythonEnvDir"



Set-Location $CloudCompareInstallFolder
stubgen -m cccorelib -m pycc -o $StubGenLoc



Set-Location $SavedLoc
# The runtime part of the pycc wrapper wannot be generated,
# so we wrote it by hand and append it to the generated pycc stub
Get-Content stubfiles/runtime.pyi | Out-File -Append stubfiles/pycc.pyi
