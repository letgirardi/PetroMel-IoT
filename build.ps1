$sourceDir = "$env:USERPROFILE\.node-red"
$destDir = "." 
$imageName = "my-iot-nodered"

$dockerUser = Read-Host -Prompt "Enter your Docker Hub Username (e.g., myuser)"
$tagVersion = Read-Host -Prompt "Enter the image version tag (e.g., v1.0)"

$fullImageName = "$dockerUser/$imageName`:$tagVersion"

Write-Host "--- Starting Node-RED Docker Automation ---" -ForegroundColor Yellow
Write-Host "Copying package.json and package-lock.json from $sourceDir..." -ForegroundColor Cyan
Copy-Item -Path "$sourceDir\package.json" -Destination $destDir -Force
Copy-Item -Path "$sourceDir\package-lock.json" -Destination $destDir -Force
Write-Host "Files copied successfully." -ForegroundColor Green

Write-Host "Proceeding to Docker build..." -ForegroundColor Green
docker build -t $imageName .

if ($LASTEXITCODE -ne 0) {
    Write-Host "Docker build failed. Stopping script." -ForegroundColor Red
    exit
}

Write-Host "--- Starting Image Publication ---" -ForegroundColor Yellow
Write-Host "Tagging local image '$($imageName)' as '$($fullImageName)'" -ForegroundColor Cyan

docker tag $imageName $fullImageName

Write-Host "Please ensure you have run 'docker login' recently." -ForegroundColor Magenta
Write-Host "Pushing image to Docker Hub (or configured registry)..." -ForegroundColor Cyan

docker push $fullImageName

if ($LASTEXITCODE -eq 0) {
    Write-Host "--- Successfully Published! ---" -ForegroundColor Green
    Write-Host "Image is available at: $fullImageName" -ForegroundColor Green
} else {
    Write-Host "--- Image Push Failed! ---" -ForegroundColor Red
    Write-Host "Check your 'docker login' status or tag name." -ForegroundColor Red
}

Write-Host "--- Automation Complete ---" -ForegroundColor Yellow