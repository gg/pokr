#include <cstddef>
#include <ctime>
#include <iostream>
#include <set>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/random.hpp>
#include <boost/test/unit_test.hpp>

#include "combinatorics.h"
#include "raywevaluator.h"

namespace {

typedef std::vector<int> CardIndices;

struct NoCardIndexConverter {
  void operator()(CardIndices& /*indices*/) {
    return;
  }
};

void ConvertToRayWCardIndices(CardIndices& indices) {
  BOOST_FOREACH(int& card_index, indices) {
    ++card_index;
  }
}

std::vector<CardIndices> GenerateHands(
    const int cards_per_hand, const int num_hands_to_generate,
    boost::function<void (CardIndices&)> ConvertCardIndices) {
  std::size_t num_possible_hands = Choose(52, cards_per_hand);

  typedef boost::mt19937 RNGType;
  typedef boost::uniform_int<> Distribution;
  RNGType rng(std::time(0));
  Distribution hand_index_distribution(0, num_possible_hands - 1);
  boost::variate_generator<RNGType, Distribution>
      random_hand_index(rng, hand_index_distribution);

  std::set<int> hand_indices;
  while (hand_indices.size() < num_hands_to_generate) {
    hand_indices.insert(random_hand_index());
  }

  std::vector<CardIndices> generated_hands;
  BOOST_FOREACH(int hand_index, hand_indices) {
    CardIndices card_indices = GetKCombination(cards_per_hand, hand_index);
    ConvertCardIndices(card_indices);
    generated_hands.push_back(card_indices);
  }

  return generated_hands;
}

template <typename Evaluator>
void EvaluateHands(const Evaluator& evaluator,
                   const std::vector<CardIndices>& hands) {
  BOOST_FOREACH(const CardIndices& hand, hands) {
    evaluator.Evaluate(hand[0], hand[1], hand[2], hand[3], hand[4], hand[5],
                       hand[6]);
  }
}

boost::posix_time::time_duration Benchmark(boost::function<void()> function) {
  using boost::posix_time::ptime;
  using boost::posix_time::microsec_clock;
  using boost::posix_time::time_duration;

  ptime t_start = microsec_clock::universal_time();

  function();

  ptime t_end = microsec_clock::universal_time();

  time_duration elapsed = t_end - t_start;

  return elapsed;
}

int GetRayWHandType(int hand_ranking) {
  return hand_ranking >> 12;
}

template <typename Evaluator>
void VerifyEvaluatorCorrectness(
    Evaluator& evaluator,
    boost::function<void(CardIndices&)> ConvertCardIndices,
    boost::function<int(int)> GetHandType) {
  int count = 0;
  std::vector<int> cards(7, 0);
  std::vector<int> hand_type_counts(10, 0);

	for (int c0 = 0; c0 < 46; c0++) {
		for (int c1 = c0+1; c1 < 47; c1++) {
 			for (int c2 = c1+1; c2 < 48; c2++) {
				for (int c3 = c2+1; c3 < 49; c3++) {
 					for (int c4 = c3+1; c4 < 50; c4++) {
						for (int c5 = c4+1; c5 < 51; c5++) {
 							for (int c6 = c5+1; c6 < 52; c6++) {
                cards[0] = c0;
                cards[1] = c1;
                cards[2] = c2;
                cards[3] = c3;
                cards[4] = c4;
                cards[5] = c5;
                cards[6] = c6;
                ConvertCardIndices(cards);
                int ranking = evaluator.Evaluate(cards[0], cards[1], cards[2],
                                                 cards[3], cards[4], cards[5],
                                                 cards[6]);
                hand_type_counts[GetHandType(ranking)]++;

								++count;
							}
						}
					}
				}
			}
		}
	}

  // See: http://en.wikipedia.org/wiki/Poker_probability#Frequency_of_7-card_poker_hands
  BOOST_CHECK_EQUAL(count, 133784560);  // C(52,7) total 7-card hands
  BOOST_CHECK_EQUAL(hand_type_counts[0], 0);          // Invalid Hand
  BOOST_CHECK_EQUAL(hand_type_counts[1], 23294460);   // No Pair
  BOOST_CHECK_EQUAL(hand_type_counts[2], 58627800);   // One Pair
  BOOST_CHECK_EQUAL(hand_type_counts[3], 31433400);   // Two Pair
  BOOST_CHECK_EQUAL(hand_type_counts[4], 6461620);    // Three of a Kind
  BOOST_CHECK_EQUAL(hand_type_counts[5], 6180020);    // Straight
  BOOST_CHECK_EQUAL(hand_type_counts[6], 4047644);    // Flush
  BOOST_CHECK_EQUAL(hand_type_counts[7], 3473184);    // Full House
  BOOST_CHECK_EQUAL(hand_type_counts[8], 224848);     // Four of a Kind
  BOOST_CHECK_EQUAL(hand_type_counts[9], 41584);      // Straight Flush
}

}  // unnamed namespace

BOOST_AUTO_TEST_SUITE(EvaluatorBenchmarks)

BOOST_AUTO_TEST_CASE(BenchmarkRayWEvaluator) {
  std::cout << "Benchmarking RayWEvaluator..." << std::endl;

  using boost::bind;
  using boost::ref;

  const int cards_per_hand = 7;
  const int num_hands = 1e6;

  std::vector<CardIndices> hands = GenerateHands(cards_per_hand, num_hands,
                                                 bind(ConvertToRayWCardIndices,
                                                      _1));

  RayWEvaluator evaluator("raywlut.dat");

  VerifyEvaluatorCorrectness(evaluator, bind(ConvertToRayWCardIndices, _1),
                             bind(GetRayWHandType, _1));

  boost::posix_time::time_duration elapsed =
    Benchmark(bind(EvaluateHands<RayWEvaluator>, ref(evaluator), ref(hands)));
  /*
  RayWEvaluator::LUT lut(RayWEvaluator::kNumEntries, 0);
  InitializeLUT("raywlut.dat", lut);

  boost::posix_time::time_duration elapsed =
    Benchmark(boost::bind(EvaluateAll7CardHands,
                          boost::ref(lut)));
                          */

  long long hands_per_second = (num_hands * 1e6)/elapsed.total_microseconds();
  std::cout << hands_per_second << " hands/second" << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
