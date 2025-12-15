$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$FrontendDir = Join-Path $ScriptDir "frontend"

Write-Host ">>> Starting Local Server..." -ForegroundColor Cyan
Write-Host "Serving from: $FrontendDir"
Write-Host "URL: http://localhost:8000" -ForegroundColor Green
Write-Host "Press Ctrl+C to stop."

# Start Python HTTP server
python -m http.server 8000 --directory "$FrontendDir"
