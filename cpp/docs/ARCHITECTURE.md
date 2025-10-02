# Nautix Architecture

Nautix is designed following Domain-Driven Design (DDD).

## Layers
- **domain/**: core business logic and entities (e.g., `Directory`, `IMetadataProvider`).
- **application/**: use cases orchestrating domain logic (e.g., `ListDirectories`).
- **infra/**: technical services, filesystem integration, metadata provider implementation.
- **ui/**: GTKmm-based user interface (future).

## Dependency rules
- `application` depends on `domain`.
- `infra` depends on `domain`.
- `ui` depends on `application`.
- `domain` has no dependencies on other layers.

## Key principles
- Test-Driven Development (TDD).
- Use cases are always tested in isolation.
- Fake providers (e.g., `FakeMetadataProvider`) live in `tests/`.
