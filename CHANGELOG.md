# Changelog

All notable changes to Nautilus will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to a four-part versioning scheme described in the documentation.

## [2.0.0] - 2026-08-26

### Breaking Changes

- **Upgraded to C++20** (from C++17). Downstream projects must now use a C++20-compatible compiler.
- **Removed `zaid_tools.hpp`** entirely. This header was verified to be unused in known downstream codes.
- **Removed `chemsym_to_zaid()` function**. Users should replace calls with EntityTag conversions:
  ```cpp
  // Old code:
  int sza = chemsym_to_zaid(input_string);
  
  // New code:
  using namespace nautilus::entity_tag;
  int sza = to_NDI_SZA(from_NDI_short_string(input_string));
  ```
- **Updated comparison operators** to use C++20 spaceship operator (`operator<=>`). This changes the implementation but not the interface of comparison operations.

### Added

- **EntityTag system**: A new comprehensive identifier system for atomic nuclei and particles with support for:
  - Standard entities (particles, elementals, nuclides)
  - User-defined entities
  - Unknown entities
  - Versioning support for future evolution
- **Format conversion support**: Bidirectional converters between EntityTag and multiple commonly-used formats:
  - Standard format (human-readable names and symbols)
  - MCNP format (full ZAID, particle symbols)
  - NDI format (FPID, short string, ZAID, SZA)
  - IC format (chemical symbol)
- **Comprehensive documentation**: New documentation pages covering concepts, EntityTag usage, and all supported formats
- **`[[nodiscard]]` attributes** on accessor methods throughout the codebase for better safety
- **GPU testing support**: Added GPU_CHECK machinery and Kokkos-based tests for EntityTag

### Changed

- **Version numbering** has been brought into line with Semantic Versioning.
- **CMake version compatibility** relaxed from `ExactVersion` to `SameMajorVersion`, following semantic versioning best practices. This allows different minor/patch versions to work together (e.g., 2.0.0 with 2.1.0).
- **Modernized C++20 features**: Replaced SFINAE with `requires` constraints in `identifiers.hpp`
- **Code organization**: Moved utility functions to new `nautilus/util/` directory (`gpu_check.hpp`, `string_processing.hpp`)
- **CI/CD infrastructure**: Updated to use Kessel workflows and newer deployment infrastructure
- **Clang-format**: Updated to version 20.1.8

### Fixed

- **Known CMake bugs** from the 1.0.1.x series. Documentation now excludes 1.0.1.x tags.
- **Debug build issues** with various assertions and includes
- **GPU_CHECK** bug fixes for device-side testing

### Documentation

- Added `concepts.rst` explaining atomic nuclei, nuclides, elementals, and particles
- Added `entity_tag.rst` with EntityTag overview and core features
- Added format-specific documentation: `format_standard.rst`, `format_ic.rst`, `format_mcnp.rst`, `format_ndi.rst`
- Updated build documentation to specify C++20 requirement
- Improved inline documentation with file-level summaries

## [1.0.1.4] - Previous Release

Earlier releases are documented in the git history.

---

[2.0.0]: https://github.com/lanl/nautilus/compare/v1.0.1.4...v2.0.0
