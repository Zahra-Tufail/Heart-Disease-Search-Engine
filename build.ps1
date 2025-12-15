$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$EmsdkPath = Join-Path $ScriptDir "emsdk"

Write-Host ">>> Heart Disease Search Engine Builder" -ForegroundColor Cyan

# 1. Setup Environment Variables specifically for this session
# We manually add the paths because `emsdk_env.bat` modifies the ephemeral environment of the batch process, not the parent Powershell.
$Env:EMSDK = $EmsdkPath
$Env:EMSDK_NODE = Join-Path $EmsdkPath "node/22.16.0_64bit/bin/node.exe"
$Env:EMSDK_PYTHON = Join-Path $EmsdkPath "python/3.13.3_64bit/python.exe"

# Add paths to process PATH
$NewPath = "$EmsdkPath;" + (Join-Path $EmsdkPath "upstream/emscripten") + ";" + (Join-Path $EmsdkPath "node/22.16.0_64bit/bin") + ";" + $Env:Path
$Env:Path = $NewPath

# 2. Check emcc availability
if (!(Get-Command "emcc.bat" -ErrorAction SilentlyContinue)) {
    Write-Host "Error: 'emcc.bat' not found in path. Construction failed." -ForegroundColor Red
    Write-Host "DEBUG Path: $Env:Path"
    exit 1
}

Write-Host ">>> Compiling C++ to WebAssembly..." -ForegroundColor Green

# 3. Compilation Command
# -I cpp: Add 'cpp' folder to include path
# --bind: Enable Embind
# -s WASM=1: Output WebAssembly
# -o frontend/project.js: Output target
$BuildCmd = "emcc.bat -I cpp cpp/bindings.cpp -o frontend/project.js --bind -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ""EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"" -O3"

Write-Host "Running: $BuildCmd"
Invoke-Expression $BuildCmd

if ($?) {
    Write-Host "`n>>> Build Successful!" -ForegroundColor Green
    Write-Host "Artifacts created:"
    Write-Host " - frontend/project.js"
    Write-Host " - frontend/project.wasm"
    Write-Host "`nYou can now run '.\run.ps1' to start the application."
}
else {
    Write-Host "`n>>> Build FAILED." -ForegroundColor Red
    exit 1
}
