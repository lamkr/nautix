# Contributing to Nautix

Thank you for contributing to Nautix! 🚀

## Environment setup
- Linux (Ubuntu recommended).
- Compiler: GCC 13+ or Clang 16+.
- CMake 3.28+.
- GTKmm 4.0.
- Catch2 (fetched automatically).

## Branching model
- `main` – stable releases.
- `develop` – integration branch.
- `feature/*` – feature branches.

## Commit messages
Use [Conventional Commits](https://www.conventionalcommits.org/).

Examples:
- `feat: add sorting by owner`
- `fix: correct memory leak in Directory`
- `test: add unit tests for FakeMetadataProvider`

## Tests
Before pushing:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --output-on-failure --test-dir build/tests
```

## Pull requests
- Make sure code is formatted with clang-format.
- Run `clang-tidy` and fix warnings.
- Add/update unit tests for new code.


---

# Build and run from project directory

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
