# WIN-GETCHILDREN-JS

Getting Folder's children with attributes: Node.js (libuv) function fs.readdir(path[, options], callback) returns parent folder's children names/paths without attributes.

Since native Windows Functions `FindFirstFileExW` and `FindNextFileW` return `WIN32_FIND_DATAW wfd` we developed a small C++ addon returning the children's paths and also attributes as JSON Array.

###### Visual Studio Build Tools 2019

Manually: https://visualstudio.microsoft.com/fr/visual-cpp-build-tools/

Visual Studio Build Tools > 2017 are needed for win-permissions-js (C++ 17)!!!

```powershell
$buildToolsUrl="https://aka.ms/vs/16/release/vs_buildtools.exe"
$buildToolsPath=Join-Path $base_install_path 'BuildTools'

# Big mistake here, we should use Microsoft.VisualStudio.Component.VC.Tools.x86.x64 and Microsoft.VisualStudio.Component.VC.v141.x86.x64
# instead of Microsoft.VisualStudio.Workload.AzureBuildTools

[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
Invoke-WebRequest -Uri $buildToolsUrl -OutFile "vs_buildtools.exe"
Start-Process -Wait -NoNewWindow -FilePath .\vs_buildtools.exe -ArgumentList '--quiet', '--wait', '--norestart', '--nocache', $('--installPath {0}' -f $buildToolsPath),
    '--add Microsoft.VisualStudio.Component.VC.CoreBuildTools',
    '--add Microsoft.VisualStudio.Component.VC.Redist.14.Latest',
    '--add Microsoft.VisualStudio.Component.Windows10SDK',
    '--add Microsoft.VisualStudio.Component.TestTools.BuildTools',
    '--add Microsoft.VisualStudio.Component.VC.ASAN',
    '--add Microsoft.VisualStudio.Component.VC.CMake.Project',
    '--add Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
    '--add Microsoft.VisualStudio.Component.Windows10SDK.18362'

$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

```
###### Build Tools (Visual Studio Build Tools 2017 + Python 2.7) for gyp

https://www.npmjs.com/package/windows-build-tools

```powershell

npm config set proxy http://proxy.test.org:8080
npm config set https-proxy http://proxy.test.org:8080

npm install --global --production --add-python-to-path windows-build-tools

#Refreshing PATH Variable after build tools installation
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
```

Usage is quite simple:
```javascript
import getChildren from "win-getchildren-js";
try {
    const children = getChildren({folderPath});
} catch(error) {
    //... You should catch errors though
}
```
