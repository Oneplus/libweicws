#LIBWEICWS PYTHON INTERFACE

## 1.Python extension (binding) of the C++ libweicws.

This directory contains source codes for building python extension of the C++ library.

The python binding code is now generated with [SWIG](http://www.swig.org). The code file "weicws.i" is an "interface file" which is the input to SWIG.

## 2.Building the module
Make sure that you have installed the LIBWEICWS in advanced.

First , simply type the following command:

* $ python setup.py build

If no error occurs the proceed with :

* $ python setup.py install

to install the python extension to your python site-package directory.

then you can type:

* import weicws

in your python code to use the module in your own program.

## 3.Environment:
gcc version 4.6.3

g++ version 4.6.3

SWIG version 2.0.4

## 
There are still some bugs to get it works. We'll solve them later.
