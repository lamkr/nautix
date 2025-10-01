## Build and run from project directory

### Clean build directory, if necessary
```shell
  rm -rf cmake-build-debug
  mkdir cmake-build-debug
  cd cmake-build-debug
```

### Build and run
```shell
  cmake -S . -B cmake-build-debug
  cmake --build cmake-build-debug -j
  make -C cmake-build-debug
  cmake-build-debug/src/nautix
```

## Run test
```shell
  cmake ..
  cmake --build . -j -- VERBOSE=1
  ctest --output-on-failure
```

```shell
  cmake -S ./tests -B cmake-build-debug/tests -DCMAKE_BUILD_TYPE=Debug
  cmake --build cmake-build-debug/tests -j -- VERBOSE=1
  ctest --test-dir cmake-build-debug/tests --output-on-failure
```

## Zip only project sources
```shell
find . \( -name "*.cpp" -o -name "*.h" -o -name "CMakeLists.txt" \) -print | zip -r my_archive.zip -@ -x "cmake-build-debug/*"
```

## Requeriments
- libstdc++ >= 23 (or clang/llvm with support)