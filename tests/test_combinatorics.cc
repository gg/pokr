#include <boost/test/unit_test.hpp>

#include "combinatorics.h"

BOOST_AUTO_TEST_SUITE(CombinatoricsTests)

BOOST_AUTO_TEST_CASE(TestChoose) {
  BOOST_CHECK_EQUAL(Choose(0, 3), 0);
  BOOST_CHECK_EQUAL(Choose(4, 5), 0);
  BOOST_CHECK_EQUAL(Choose(10, 3), 120);
  BOOST_CHECK_EQUAL(Choose(52, 5), 2.59896e6);
  BOOST_CHECK_EQUAL(Choose(52, 7), 1.3378456e8);
}

BOOST_AUTO_TEST_CASE(TestMultichoose) {
  BOOST_CHECK_EQUAL(Multichoose(0, 2), 0);
  BOOST_CHECK_EQUAL(Multichoose(1, 2), 1);
  BOOST_CHECK_EQUAL(Multichoose(5, 9), 715);
  BOOST_CHECK_EQUAL(Multichoose(9, 5), 1287);
  BOOST_CHECK_EQUAL(Multichoose(13, 4), 1820);
}

BOOST_AUTO_TEST_CASE(TestGetKCombination) {
  // using the example from Wikipedia under "Finding the k-combination for
  // a given number":
  // https://secure.wikimedia.org/wikipedia/en/wiki/Combinatorial_number_system
  std::vector<int> kcombination = GetKCombination(5, 72);
  BOOST_CHECK_EQUAL(kcombination.size(), 5);
  BOOST_CHECK_EQUAL(kcombination[0], 8);
  BOOST_CHECK_EQUAL(kcombination[1], 6);
  BOOST_CHECK_EQUAL(kcombination[2], 3);
  BOOST_CHECK_EQUAL(kcombination[3], 1);
  BOOST_CHECK_EQUAL(kcombination[4], 0);
}

BOOST_AUTO_TEST_SUITE_END()
