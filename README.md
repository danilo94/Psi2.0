# Psi 2.0

A tool for build a debug infrascture for CPU-FPGA intel accelerators

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

```
boost library
qt5 ( To build wire parser module )
yosys 0.4 ( Open Synthesis Suite )
Node JS ( for electron GUI )

```

### Installing


Firstly you need to install yosys dependency, for this you need to install or compile the yosys included in this package. For this you will run the following commands on yosys-yosys-0.4 folder:

```
make clean
```

For clean the repository

```
make
```
To compile yosys ( probably you will need to install the follow  Prerequisites:

```
 clang, bison, flex, libreadline-dev, gawk, tcl-dev, libffi-dev, git, graphviz, xdot, pkg-config, python3
```

To install these dependencies in ubuntu, you can run the follow command
```
sudo apt-get install build-essential clang bison flex \
	libreadline-dev gawk tcl-dev libffi-dev git \
	graphviz xdot pkg-config python3
```

Finally after compile, you will run the following comand to install yosys in your system.

```
make install
```

After install yosys, you need to compile Psi1.0 tool, you can do it using 

```
make
```
on /Psi1.0/build folder.


## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Qt Creator](https://www.qt.io/download) - Qt Creator provides a cross-platform, complete integrated development environment (IDE) for application developers to create applications for multiple desktop
* [Node JS](https://nodejs.org/) - Node.js® is a JavaScript runtime built on Chrome's V8 JavaScript engine.



## Authors

* **Danilo Almeida** 


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
