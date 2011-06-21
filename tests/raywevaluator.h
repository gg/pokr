#ifndef POKR_RAYWEVALUATOR_H__
#define POKR_RAYWEVALUATOR_H__

#include <cstddef>
#include <string>
#include <vector>
#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

class RayWEvaluator : private boost::noncopyable {
 public:
  static const std::size_t kNumEntries = 32487834;
  typedef std::vector<int> LUT;

  RayWEvaluator(const std::string& lut_filename);

  int Evaluate(int c1, int c2, int c3, int c4, int c5) const;
  int Evaluate(int c1, int c2, int c3, int c4, int c5, int c6) const;
  int Evaluate(int c1, int c2, int c3, int c4, int c5, int c6, int c7) const;

 private:
  LUT lut_;
};

void InitializeLUT(const std::string& lut_filename, RayWEvaluator::LUT& lut);

#endif  // POKR_RAYWEVALUATOR_H__
