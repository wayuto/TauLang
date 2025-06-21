# Tau 编程语言 [English Documentation](README.md)

Tau 是一个用 C 语言实现的类 C 语法的解释型编程语言，支持变量声明、表达式、流程控制、函数等特性。项目采用 CMake 构建，适合学习编译原理、虚拟机实现等。

## 构建与运行

### 依赖
- GCC 或 Clang
- CMake 3.0 及以上

### 构建步骤
```sh
# 克隆仓库
 git clone https://github.com/wayuto/TauLang.git
 cd Tau
# 构建
 mkdir -p build
 cd build
 cmake ..
 make
# 运行
 ./mc
```

## 基本语法（C Like）
Tau 语法类似 C 语言，支持如下结构：

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
