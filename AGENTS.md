# Repository Guidelines

## Project Structure & Modules
- Root hosts multiple C++ projects. Common assets:
  - `include/` shared headers, `lib/` shared libs, `libsrc/` third‑party and internal libs (e.g., `libzce`, `libQWT`).
  - `bin/` build outputs by platform/config (e.g., `bin/x64/Debug`).
- Applications:
  - `xApc/` Qt6 desktop app (CMake + VS solution), has `tests/`.
  - `xRto/`, `HostVM/InstallXpress/TaiJiMPC/` Windows solutions and tooling.
- Submodules live under `libsrc/` (see `.gitmodules`).

## Build, Test, and Dev Commands
- Init deps and submodules:
  - `git submodule update --init --recursive`
  - Windows vcpkg (set `VCPKG_ROOT`): `vcpkg install ...` via `vcpkg_requirements.bat`.
- xApc (CMake, Visual Studio 2022):
  - Configure: `cmake -S xApc -B xApc/build -G "Visual Studio 17 2022" -A x64`
  - Build: `cmake --build xApc/build --config Debug`
  - Run tests (Windows): `xApc\tests\test_windows.bat`
- Linux helpers: `./build.sh` builds key libs with `scons`.

## Coding Style & Naming
- Formatting: `.clang-format` (Google-based), 4‑space indent, K&R braces, column limit 100, no tab, no include sorting. Run: `clang-format -i <files>`.
- C++: Classes `UpperCamelCase` (e.g., `MainWindow`), functions `lowerCamelCase`, constants `kPascalCase`. Match file names to classes (`ClassName.h/.cpp`).
- Headers in `include/` are public; prefer `#pragma once` and narrow includes.

## Testing Guidelines
- xApc uses Qt Test (`QTest`); GoogleTest available via vcpkg.
- Place tests under `<project>/tests/`; name `*_test.cpp` or descriptive suites (e.g., `unit_test.cpp`).
- Run Qt tests via the built binary or CTest if integrated.

## Commits & Pull Requests
- Commits: concise, imperative present tense (e.g., "add xOpt unit", "fix lock read"). Group related changes; reference issues if applicable.
- PRs: include purpose, scope, build/run notes, and screenshots for UI. Link issues and note any submodule or vcpkg impact.

## Security & Configuration Tips
- Keep secrets out of VCS. Use environment variables for credentials.
- Ensure Qt6, MSVC 2022, and `vcpkg` are installed on Windows. On Linux, install `scons` and required toolchains.
