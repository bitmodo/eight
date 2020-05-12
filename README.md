# Eight
[![License](https://img.shields.io/github/license/bitmodo/eight)](https://github.com/bitmodo/eight/blob/master/LICENSE)

Eight is a programming language ecosystem.
Not only is it a language, but it is also a framework for developing new languages.
It includes a machine code compiler, JIT compiler, and interpreter.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.
See deployment for notes on how to deploy the project on a live system.

### Prerequisites

First, you need to clone the repository.
To do that, make sure you have Git installed first.
Then, you can run the following:

```console
~# git clone https://github.com/bitmodo/eight.git eight

~# cd eight
```

### Building

Once you have the repository, you can build the project into it's different parts.
We use the [Meson build system](https://mesonbuild.com/) with [Ninja](https://ninja-build.org/), so make sure you have that installed first.
Next, run the following commands:

```console
~# meson build

~# ninja -C build
```

## Built With

 * [LLVM](https://llvm.org/) - The compiler toolchain

## Contributing

Please read [CONTRIBUTING.md](https://github.com/bitmodo/.github/blob/master/CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/bitmodo/eight/blob/master/LICENSE) file for details.
