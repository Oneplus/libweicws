/*
 *
 *
 *
 *
 *
 *
 *
 *
 */
#ifndef __CPPSTRLIB_H__
#define __CPPSTRLIB_H__

#include <iostream>
#include <vector>
#include <string>

std::string chomp(std::string str);
std::string cutoff(std::string str, std::string mark);
std::vector<std::string> split(std::string str);
std::vector<std::string> split(std::string str, std::string seperator);
std::string join(std::vector<std::string> sep);
std::string join(std::vector<std::string> sep, std::string separator);
bool startswith(const std::string &str, const std::string &head);
bool isInt(const std::string &str);
bool isDouble(const std::string &str);
int toInt(const std::string &str);
double toDouble(const std::string &str);
int charType(const std::string &str);
int charType_ext(const std::string &str);
bool isSBC(const std::string &str);
bool isDBC(const std::string &str);
bool isLetter(const std::string &str);
bool isPunc(const std::string &str);
bool isDigit(const std::string &str);
std::string SBC2DBC(const std::string &str);

#endif  // end for __CPPSTRLIB_H__
