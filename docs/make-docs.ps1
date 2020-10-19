param(
    [Parameter(Mandatory = $true)]
    [String]$InstallDir,
    [string]$BuildType = "html",
    [string]$OutputDir = "_build"
)

$PluginDir = Join-Path -Resolve $InstallDir "plugins/Python/Lib/site-packages/"
if ($null -eq $env:pythonpath) {
    $env:pythonpath = $PluginDir
} elseif ($env:pythonpath.IndexOf($PluginDir) -eq -1) {
    $env:pythonpath = ";$PluginDir"
}

if ($env:path.IndexOf($InstallDir) -eq -1) {
    $env:path += ";$InstallDir"
    $env:path += ";$(Join-Path $InstallDir plugins)"
}


sphinx-build -b $BuildType . $OutputDir
