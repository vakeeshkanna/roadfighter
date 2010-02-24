@pushd .
cd /d %DEVROOT%/build
devenv ALL_BUILD.vcproj /rebuild "release"
@popd
