## Build and run from project directory
```shell
  cmake -S . -B cmake-build-debug
  cmake --build cmake-build-debug
  make -C cmake-build-debug
  cmake-build-debug/src/nautix
```

## Run test
```shell
  cmake -S ./tests -B cmake-build-debug/tests
  cmake --build cmake-build-debug/tests
  ctest --test-dir cmake-build-debug/tests
  #ctest --build-run-dir cmake-build-debug/tests
  ctest --build-exe-dir cmake-build-debug/tests
  
  ./tests/test_list_directories
```
