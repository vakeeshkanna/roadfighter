@pushd .
cd /d %DEVROOT%
devenv ALL_BUILD.vcproj /build
@popd
