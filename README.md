# Tau [简体中文文档](README_CN.md)

Tau is an interpreted programming language implemented in C, featuring C-like syntax. It supports variable declarations, expressions, control flow, and functions. The project uses CMake for building and is suitable for learning compiler principles and virtual machine implementation.

## Build & Run

### Prerequisites
- GCC or Clang
- CMake 3.0 or higher

### Build Steps
```sh
# Clone the repository
git clone https://github.com/wayuto/TauLang.git
cd Tau
# Build
mkdir -p build
cd build
cmake ..
make
# Run
./mc
```

## Basic Syntax (C Like)
Tau syntax is similar to C and supports the following structures:

```c
int main() {
    int a = 10;
    int b = 20;
    if (a < b) {
        printf("a < b\n");
    } else {
        printf("a >= b\n");
    }
    while (a > 0) {
        a = a - 1;
    }
    return 0;
}
```

## Project Structure
- `main.c`         — Entry point
- `parser.c`       — Parser implementation
- `vm.c`           — Virtual machine logic
- `statement.c`    — Statement handling
- `expression.c`   — Expression evaluation
- `function.c`     — Function handling
- `declaration.c`  — Declarations
- `globals.h`      — Global definitions
- `hello.c`        — Example program
- `CMakeLists.txt` — Build configuration

## License
See [LICENSE](LICENSE) for details.

---
*Generated README.md — update as needed for your project specifics.*
