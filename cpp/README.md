## Build and run from project directory
```shell
  cmake -S . -B cmake-build-debug
  cmake --build cmake-build-debug
  make -C cmake-build-debug
  cmake-build-debug/src/nautix
```

## Run test
```shell
  cmake -S ./tests -B cmake-build-debug/tests -DCMAKE_BUILD_TYPE=Debug -LA -N
  cmake --build cmake-build-debug/tests -- VERBOSE=1
  ctest --test-dir cmake-build-debug/tests --output-on-failure
  #ctest --build-run-dir cmake-build-debug/tests
  ctest --build-exe-dir cmake-build-debug/tests
  
  ./tests/test_list_directories
```

## Zip only project sources
```shell
find . \( -name "*.cpp" -o -name "*.h" -o -name "CMakeLists.txt" \) -print | zip -r my_archive.zip -@ -x "cmake-build-debug/*"
```


## Requeriments
- libstdc++ >= 23 (or clang/llvm with support)