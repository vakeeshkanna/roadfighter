@pushd .
cd /d %DEVROOT%/build
devenv ALL_BUILD.vcproj /build "release"
@popd
