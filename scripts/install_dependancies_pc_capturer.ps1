if ( $args.count -eq 0 )
{
    Write-Host "The first parameter should be the location where you want to install vcpkg"   
    exit
}
$vcpkg_directory=$args[0]
if (-Not (Test-Path -Path $vcpkg_directory)) {
    Write-Host "Supplied path does not exist"   
    exit
}
if (-Not (Test-Path -Path ($vcpkg_directory + "/vcpkg.exe"))) {
    Write-Host "vcpkg installed"   
    Push-Location -Path $vcpkg_directory
    git clone https://github.com/Microsoft/vcpkg.git
    Pop-Location
    $vcpkg_directory=$vcpkg_directory + "/vcpkg"
    
}
Push-Location -Path $vcpkg_directory
./vcpkg/bootstrap-vcpkg.sh
./vcpkg install draco
./vcpkg install pcl
./vcpkg install realsense2
Pop-Location