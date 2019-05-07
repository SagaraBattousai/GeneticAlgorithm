#pragma once

/*
#ifdef EXPORT_STATISTICS
  #define STATISTICS_API __declspec(dllexport)
//#else
//  #define STATISTICS_API __declspec(dllimport)
#endif
*/

#include <vector>
#include <cstddef>
#include <random>
#include <chrono>
#include <limits>
#include <utility>
#include <numeric>

template<class Key>
std::vector<std::pair<Key, std::size_t>> vectorMap(const std::vector<Key>& vec)
{
  std::vector<std::pair<Key, std::size_t>> vectorMapper;

  for (std::size_t i = 0; i < vec.size(); i++)
  {
    std::pair<Key, std::size_t> pair(vec[i], i);
    vectorMapper.push_back(pair);
  }

  return vectorMapper;
};


template<class Score>
std::vector<std::size_t> probabilisticSample(const std::vector<Score> scores, std::size_t count)
{

  std::vector<std::pair<Score, std::size_t>> vectorMapper = vectorMap<Score>(scores);

  std::vector<std::size_t> sample;

  std::default_random_engine gen((unsigned)std::chrono::system_clock::now().time_since_epoch().count());

  std::shuffle(vectorMapper.begin(), vectorMapper.end(), gen);

  int i = 0;

  while (i < count)
  {
    Score totalScore = 0;

    std::for_each(vectorMapper.begin(), vectorMapper.end(), [&totalScore](std::pair<Score, std::size_t> pair) {totalScore += pair.first; });

    double min = std::numeric_limits<double>::min();
    if (totalScore == 0)
    {
      if (sample.size() < count)
      {

      }
      else
      {
        return sample;
      }
    }

    std::uniform_real_distribution<double> dist(min, (double) totalScore);

    auto it = vectorMapper.begin();

    double randomVariable = dist(gen);

    //Score or double??
    double theOne = randomVariable - it->first;

    while (theOne > 0)
    {
      ++it;
      theOne -= it->first;
    }

    sample.push_back(it->second);

    it->first = 0;
    i++;
  }

  return sample;
};


std::vector<std::size_t> uniformSample(int start, int end, std::size_t count)
{
  std::vector<std::size_t> sample;

  std::vector<std::size_t> range((end - start) + 1);

  std::iota(range.begin(), range.end(), start);

  std::default_random_engine gen((unsigned)std::chrono::system_clock::now().time_since_epoch().count());

  int i = 0;

  while (i < count)
  {
    std::uniform_int_distribution<std::size_t> dist(0, range.size() - 1);

    std::size_t index = dist(gen);

    sample.push_back(range[index]);

    range.erase(range.begin() + index);

    i++;
  }

  return sample;
}