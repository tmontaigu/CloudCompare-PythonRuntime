param(
    [Parameter(Mandatory = $true)]
    [String]$InstallDir,
    [string]$BuildType = "html",
    [string]$OutputDir = "_build"
)

$PluginDir = Join-Path -Resolve $InstallDir "plugins/Python/Lib/site-packages/"
if ($null -eq $env:pythonpath) {
    $env:pythonpath = $PluginDir
} elseif ($env:pythonpath.indexOf($PluginDir) -eq -1) {
    $env:pythonpath = ";$PluginDir"
}

sphinx-build -b $BuildType . $OutputDir
