@pushd .
cd /d %DEVROOT%
devenv ALL_BUILD.vcproj /rebuild
@popd
