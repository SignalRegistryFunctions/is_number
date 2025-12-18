#

## Build
```
cmake -B build
cmake --build build --config Release
```

## Test
```
cmake -B build
cmake --build build --config Debug
cd build
ctest.exe -C Debug
```

## Install
```
if [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
  cmake.exe -B build -DCMAKE_INSTALL_PREFIX="$LOCALAPPDATA"/SignalRegistry  
else
  cmake.exe -B build -DCMAKE_INSTALL_PREFIX=/usr/local/SignalRegistry  
fi
cmake --build build --config Release --target INSTALL

```