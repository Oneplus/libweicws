# LIBWEICWS

[![Build Status](https://travis-ci.org/Oneplus/libweicws.png?branch=master)](https://travis-ci.org/Oneplus/libweicws)
[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/Oneplus/libweicws/trend.png)](https://bitdeli.com/free "Bitdeli Badge")
[![coverity scan build status](https://scan.coverity.com/projects/1433/badge.svg)](https://scan.coverity.com/projects/1433/)

### TOC

 1. Introduction
 2. Prerequisites
 3. Building
 4. Example
 5. License
 6. ChangLog
 7. Models

## 1. INTRODUCTION

 ***libweicws*** is a micro blog oriented Chinese word segmentation system. It's
 the system we submit to Task 1: 
 [Micro-blog word segmentation](http://www.cipsc.org.cn/clp2012/task1.html)
 on 2012 CLP back-offs. This system achieve F-score of ***94.04%*** on the bake-off
 test data. The algorithm is presented in 
 [this paper](http://ir.hit.edu.cn/~yjliu/publication/clp2012.pdf)
 in detail.
 
 We want to devolop a library that can achieve some certain accuracy on micro
 blog corpus. We also want to make it compatiable crossing differen platform. 
 What's more, we are planning to provide different programming language
 interfaces including Java and Python.
 
 Model and data are still under construction for publish.
 
## 2. PREREQUISITES

 This project requires:
  * Cross-platform Make (CMake) v2.8.0+
  * GNU Make or equivalent.
  * GCC or an alternative, reasonably conformant C++ compiler. MSVC can also work fine with it
  * [PCRE-8.32](http://www.pcre.org/)
  * [crfsuite-0.12](http://www.chokkan.org/software/crfsuite/)

NOTE: You don't need to link libpcre and libcrfsuit to this project, we have
integrated above two libraries into it and hacked the source of their makefile
a tiny little bit.

## 3. BUILDING
 
 This project uses the Cross-platform Make (CMake) build system. However, we
 have conveniently provided a wrapper configure script and Makefile so that
 the typical build invocation of "./configure" followed by "make" will work.
 For a list of all possible build targets, use the command "make help".

 __In Unix (GCC)__:
 ```
 ./configue
 make
 ```

 __In Windows (MSVC)__:
 ```
 mkdir build
 cd build
 cmake ..
 ```
 Open the Visual Studio solution `libweicws.sln`, and build it like other projects.

 NOTE: Users of CMake may believe that the top-level Makefile has been
 generated by CMake; it hasn't, so please do not delete that file.

## 4. Example

 You can refer the [demo.cpp](https://github.com/Oneplus/libweicws/blob/master/test/demo.cpp).
 It's an example of loading model and conducting segmentation over Chinese sentence.

## 5. LICENSE

 Open source licensing is under the [full GPL](http://www.gnu.org/licenses/lgpl.txt), which allows many free uses.
 For distributors of proprietary software, commercial licensing with a ready-to-sign
 agreement is available.
 
## 6. ChangeLog

 2013-01-07
 
  * publish first version of the library. 

## 7. Model

 Model suiteable for current version can be downloaded from [here](http://pan.baidu.com/share/link?shareid=397022&uk=3305192391).
 
 Histories:
 
 * [libweicws-model-0.0.1](http://pan.baidu.com/share/link?shareid=397022&uk=3305192391)

## TODO

 * Improve the performance
 * Multi-thread support
 * (?)Iteratior model over the result

