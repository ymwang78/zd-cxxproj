# C++ Projects Structure

This directory contains all C++ projects with a shared structure for common dependencies and individual project directories.

## Project Structure

### Files
- `.clang-format`: C++ code formatting standards
- `.gitignore`: Git ignore rules
- `.gitmodules`: Git submodule configuration
- `build.sh`: Linux build script for all projects
- `README.md`: Directory documentation
- `vcpkg_requirements.bat`: Windows vcpkg dependency packages

### Directories
- `include/`: Common header files shared across projects
- `lib/`: Common library files
- `libsrc/`: Common library source code
- Individual project directories (each with their own git repository)

## Common Commands

### Build
```bash
# Build all projects on Linux
./build.sh
```

### Development
```bash
# Format code according to project standards
clang-format -i <file>

# Check git status
git status

# Update submodules
git submodule update --init --recursive
```

### Project Management
- Each project directory typically has its own git repository
- Common dependencies are shared through the `include/`, `lib/`, and `libsrc/` directories
- Use the centralized build script for consistent compilation across projects