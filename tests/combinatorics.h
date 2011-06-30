#ifndef POKR_COMBINATORICS_H__
#define POKR_COMBINATORICS_H__

#include <cstddef>
#include <vector>
#include <boost/math/special_functions/binomial.hpp>

// Returns the number of k-combinations from a set S of size n, otherwise termed
// "n Choose k".
//
// see: https://secure.wikimedia.org/wikipedia/en/wiki/Combinations#Number_of_k-combinations
// see: http://mathworld.wolfram.com/Choose.html
std::size_t Choose(std::size_t n, std::size_t k);

// Returns the number of k-multicombinations of size k from a set S of size n,
// otherwise termed "n multichoose k".
//
// see: https://secure.wikimedia.org/wikipedia/en/wiki/Combinations#Number_of_combinations_with_repetition
std::size_t Multichoose(std::size_t n, std::size_t k);

// Returns the k-combination at position pos, using the combinatorial number
// system. The k-combination elements will be in decreasing order.
//
// See the section titled "Finding the k-combination for a given number":
// https://secure.wikimedia.org/wikipedia/en/wiki/Combinatorial_number_system
std::vector<int> GetKCombination(int k, int pos);

#endif // POKR_COMBINATORICS_H__
