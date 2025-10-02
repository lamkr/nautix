# Nautix – Code Style Guide

## General
- All headers must start with `#pragma once`.
- Use UTF-8 encoding without BOM.
- Maximum line length: 100 characters.

## Naming
- Classes/structs: `PascalCase` (e.g., `DirectoryEntry`).
- Methods/functions: `camelCase` (e.g., `listDirectories`).
- Constants/enums: `UPPER_SNAKE_CASE` (e.g., `MAX_BUFFER_SIZE`).
- Variables: `camelCase`.

## Includes order
1. Corresponding header.
2. C++ standard library.
3. Third-party libraries.
4. Project headers.

## Formatting
- 4 spaces indentation.
- Always use braces, even for single-line `if`.
- Prefer `auto` when type is obvious.
- Use `const &` for parameters that should not be copied.
- Use `[[nodiscard]]` where return value must not be ignored.
