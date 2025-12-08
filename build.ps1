

$sourceDir = "$env:USERPROFILE\.node-red"

$destDir = "." 

Write-Host "--- Starting Node-RED Docker Build Automation ---" -ForegroundColor Yellow

Write-Host "Copying package.json and package-lock.json from $sourceDir..." -ForegroundColor Cyan
Copy-Item -Path "$sourceDir\package.json" -Destination $destDir -Force
Copy-Item -Path "$sourceDir\package-lock.json" -Destination $destDir -Force

Write-Host "Files copied successfully. Proceeding to Docker build." -ForegroundColor Green

docker build -t my-iot-nodered --no-cache .

Write-Host "--- Build Complete! ---" -ForegroundColor Yellow