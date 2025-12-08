$dockerUser = "bdeyvid"
$imageName = "my-iot-nodered"
$tagVersion = "v0.1" 
$containerName = "my-custom-nodered"
$hostPort = 1880
$containerPort = 1880

$fullImageName = "$dockerUser/$imageName`:$tagVersion"

Write-Host "--- Starting Node-RED Deployment Automation ---" -ForegroundColor Yellow

Write-Host "Checking for existing container '$containerName'..." -ForegroundColor Cyan

docker stop $containerName 2>$null | Out-Null

docker rm $containerName 2>$null | Out-Null

Write-Host "Cleanup complete." -ForegroundColor Green

Write-Host "Pulling image: $fullImageName" -ForegroundColor Cyan
docker pull $fullImageName

if ($LASTEXITCODE -ne 0) {
    Write-Host "--- ERROR: Docker pull failed! ---" -ForegroundColor Red
    Write-Host "Verify your image name and ensure you have run 'docker login'." -ForegroundColor Red
    exit 1
}

Write-Host "Running container '$containerName' on port $hostPort..." -ForegroundColor Cyan
docker run -d `
    --name $containerName `
    -p "$hostPort`:$containerPort" `
    $fullImageName

if ($LASTEXITCODE -eq 0) {
    Write-Host "--- Deployment Successful! ---" -ForegroundColor Green
    Write-Host "Access Node-RED at: http://localhost:$hostPort" -ForegroundColor Green
} else {
    Write-Host "--- ERROR: Docker run failed! ---" -ForegroundColor Red
    exit 1
}

Write-Host "--- Automation Complete ---" -ForegroundColor Yellow