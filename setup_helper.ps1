Write-Host ">>> Checking Prerequisites..." -ForegroundColor Cyan

# Check Python
if (Get-Command "python" -ErrorAction SilentlyContinue) {
    Write-Host "[OK] Python is installed." -ForegroundColor Green
}
else {
    Write-Host "[X] Python is missing!" -ForegroundColor Red
    Write-Host "Installing Python via Winget (Silent)..."
    winget install -e --id Python.Python.3.11 --silent --accept-source-agreements --accept-package-agreements
    Write-Host "Please RESTART your terminal after this script finishes to apply changes." -ForegroundColor Yellow
}

# Check Git
if (Get-Command "git" -ErrorAction SilentlyContinue) {
    Write-Host "[OK] Git is installed." -ForegroundColor Green
}
else {
    Write-Host "[X] Git is missing!" -ForegroundColor Red
    Write-Host "Installing Git via Winget (Silent)..."
    winget install -e --id Git.Git --silent --accept-source-agreements --accept-package-agreements
    Write-Host "Please RESTART your terminal after this script finishes to apply changes." -ForegroundColor Yellow
}

Write-Host "`n>>> Downloading Emscripten SDK..." -ForegroundColor Cyan
if (!(Test-Path "emsdk")) {
    if (Get-Command "git" -ErrorAction SilentlyContinue) {
        git clone https://github.com/emscripten-core/emsdk.git
        if (!$?) {
            Write-Host "Git clone failed. If you just installed Git, you MUST restart your terminal first." -ForegroundColor Red
        }
    }
    else {
        Write-Host "Git is not available in the current session yet. Please RESTART your terminal/IDE and run this script again to download Emscripten." -ForegroundColor Red
    }
}
else {
    Write-Host "emsdk folder already exists. Skipping clone." -ForegroundColor Yellow
}

Write-Host "`n>>> Instructions to Finish Setup:" -ForegroundColor Magenta
Write-Host "1. Close this terminal and open a new one."
Write-Host "2. Run: cd emsdk"
Write-Host "3. Run: .\emsdk install latest"
Write-Host "4. Run: .\emsdk activate latest"
Write-Host "5. Run: .\emsdk_env.bat"
