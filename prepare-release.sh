#!/bin/bash

set -eo pipefail

ARTIFACT=workbench-release.zip

# Test if we have all required programs
which 7z zig > /dev/null

echo "Building linux executable"
zig build -Dtarget=x86_64-native-gnu -Drelease-safe -Dstrip

echo "Building windows executable"
# see https://github.com/ziglang/zig/issues/6492
zig build -Dtarget=x86_64-windows-gnu -Dstrip # -Drelease-safe

rm -f "${ARTIFACT}"
rm -rf release
mkdir -p release

cp -r examples release/
cp -r templates release/
cp LICENSE MANUAL.txt release/
cp zig-cache/bin/cg-workbench release/
cp zig-cache/bin/cg-workbench.exe release/

cd release
7z a "../${ARTIFACT}" *
cd ..
