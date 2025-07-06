# 目录说明

  当前目录存放所有C++项目，当前目录下文件和目录作用分别如下说明:

## 1. 文件说明

* .clang-format: 存放C++代码编码规范  
* .gitignore: 存放git忽略规则

* .gitmodules: git submodule

* build.sh: linux全项目编译脚本

* README.sh: 本文件, 目录说明

* vcpkg_requirements.bat: windows下vcpkg依赖包

## 2. 目录说明

  目录分为两种：公共目录和项目目录。公共目录存放所有C++项目工程都依赖的公共文件例如头文件库文件；除去公共目录，项目目录则存放各个具体项目的文件，一般而言，每个项目都有自己的git仓库。

### 公共目录

* include: 公共头文件

* lib: 公共库文件

* libsrc: 公共有代码的库文件

