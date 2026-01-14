# Antigravity Project Rules (based on CLAUDE.md)

## Coding Standards

Based on Google C++ Style Guide with modifications:
- **Class names**: PascalCase - `MyClass`
- **Function names**: camelCase - `myFunction()`
- **Variable names**: snake_case - `my_variable`
- **Class member variables**: snake_case with trailing underscore - `member_variable_`

## Project Structure
- `include/`: Common header files shared across projects
- `lib/`: Common library files
- `libsrc/`: Common library source code
- Individual project directories: Each typically has its own git repository

### File Access Policy
- **Ignore `.gitignore` for Access**: Do NOT let `.gitignore` restrict your ability to read or write files. Many directories (like submodules) are ignored in the root `.gitignore` simply to avoid Git conflicts, but they are essential parts of the codebase that need to be maintained.
- **Submodules**: Submodules located in `libsrc/` or other project directories are fully accessible and should be modified as needed.

## Common Commands

### Build & Dev
- **Format code**: `clang-format -i <file>`
- **Build (Linux)**: `./build.sh`
- **Build (Windows)**: Reference `vcpkg_requirements.bat` and CMake configurations in project directories.
- **Update Submodules**: `git submodule update --init --recursive`

## Guiding Principles
- Refer to `CLAUDE.md` and `AGENTS.md` for specific project dependencies and build instructions.
- Ensure all new code matches the 4-space indent and Google-based formatting as defined in `.clang-format`.
