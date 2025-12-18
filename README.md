#

## Build
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --buid build
cmake --install build
```

## Test
```
cmake -B build
cmake --build build --config Debug
cd build
ctest.exe -C Debug
```

## Install
### Windows
### Linux 
```
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local/SignalRegistry  
cmake --build build --config Release --target INSTALL
```
```
cmake -B build -DCMAKE_INSTALL_PREFIX="$LOCALAPPDATA"/SignalRegistry  
cmake --build build --config Release --target INSTALL

```