# Nautix

Explorer-like file manager for Linux GNOME, written in **C++23** using **DDD** principles.

## Features
- Directory listing (use case based).
- Sorting by name, dates, owner.
- Clear architecture with domain/application/infra/ui layers.
- Fully tested with Catch2 (TDD/BDD).

## Build
```bash
git clone https://github.com/lamkr/nautix.git
cd nautix
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

## Run tests
```bash
ctest --output-on-failure --test-dir build/tests
```

## Contributing

See CONTRIBUTING.md
