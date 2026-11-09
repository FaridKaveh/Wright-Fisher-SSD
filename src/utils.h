#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

template <typename T> void printVector(const std::vector<T> &vec);

template <typename T>
void printVectorOfVectors(const std::vector<std::vector<T>> &vecOfVecs);

template <typename T> int countNonZeroEntries(const std::vector<T> &vec);

template <typename T> int binarySearch(const T &num, const std::vector<T> &vec);

template <typename T> void addVectors(const T &summand, T &vec);

template <typename T>
void writeToCSV(const std::vector<T> &data, const std::string &ofile,
                const char &mode);

#include "template_utils.tpp"

#endif
