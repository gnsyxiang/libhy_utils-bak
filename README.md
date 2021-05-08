# README

## 依赖

* `autotools`
* `java`
* `doxygen`
* `graphviz`

> 安装方法，详见[README.md](https://github.com/gnsyxiang/autotools_demo_lib/blob/main/README.md)

## 使用说明

* 执行`./autogen.sh`，生成`configure`脚本

* 三部曲编译源码并生成对应文件

```shell
$ ./configure --prefix=`pwd`/install
$ make
$ make install
```

## 配置相关选项

* `--prefix=PREFIX`: 指定安装路径

* `--enable-doxygen-doc`: 生成html帮助文档

* `PKG_CONFIG_PATH`: 使用`pkg-config`检测依赖库

## 交叉编译

```shell
$ ./configure \
    --prefix=`pwd`/install \
    CC=arm-himix200-linux-gcc \
    CXX=arm-himix200-linux-g++ \
    --host=arm-himix200-linux
$ make
$ make install
```

