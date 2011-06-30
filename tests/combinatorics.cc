#include "combinatorics.h"

namespace {

// Returns the largest n such that C(n, k) <= limit.
//
// Helper function for GetKCombination().
int FindLargestN(int k, int limit) {
  int n = k;
  std::size_t coeff = 1, prev_coeff = 0;

  while (coeff <= limit) {
    coeff = Choose(++n, k);
    prev_coeff = coeff;
  }

  return n - 1;
}

}  // unnamed namespace

// boost::math::binomial_coeffient requires n >= k
// see: http://mathworld.wolfram.com/Choose.html
std::size_t Choose(std::size_t n, std::size_t k) {
  if (n < k) return 0;
  return boost::math::binomial_coefficient<double>(n, k);
}

// see: http://mathworld.wolfram.com/Multichoose.html
std::size_t Multichoose(std::size_t n, std::size_t k) {
  return Choose(n + k - 1, k);
}

// Uses the algorithm described in the section titled "Finding the k-combination
// for a given number":
// https://secure.wikimedia.org/wikipedia/en/wiki/Combinatorial_number_system
std::vector<int> GetKCombination(int k, int pos) {
  std::vector<int> kcombination;
  for (; k > 0; --k) {
    int n = FindLargestN(k, pos);
    kcombination.push_back(n);
    pos -= Choose(n, k);
  }
  return kcombination;
}
