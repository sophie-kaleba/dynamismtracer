# dynamismtracer

This package implements dynamic analysis to study dynamism in R.

## Dependencies

For building and installation, the following dependencies are needed:
- [R-dyntrace](https://github.com/PRL-PRG/R-dyntrace "R-dyntrace")
- [dynalyzer](https://github.com/PRL-PRG/dynalyzer "dynalyzer")

For development, the following dependencies are needed:
- [git](https://git-scm.com/ "git")
- [clang-format ≥ 7.0](https://clang.llvm.org/docs/ClangFormat.html "clang-format")


## Administration

### Clone

To download the project, execute the following command:

```shell
$ git clone https://github.com/PRL-PRG/dynamismtracer.git
$ cd dynamismtracer
```

### Setup

To setup the project for development, execute the following command inside the project directory:

```shell
$ make init
```

### Build

To build and install this package, execute the following command inside the project directory:

```shell
$ make install
```

### Clean

To clean build artifacts, execute the following command inside the project directory:
```shell
$ make clean
```
