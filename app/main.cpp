#include <iostream>
#include "gene.h"
#include <string>
#include <array>
#include <random>
#include <cstddef>
#include <cmath>
#include <vector>
#include "geneticAlgorithm.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <limits>
#include <numeric>

double expected(int bit_length)
{
  long long max = (((long long)1) << bit_length) - 1;

  return (double) max / 2;
}

template<class T = int>
std::string to_binary(T num)
{
  std::string str = "";

  while (num < 0)
  {
    if (num == -1)
    {
      std::size_t size = sizeof(num) * 8 - str.length();
      str.insert(0, size, '1');
      return str;
    }
    str.insert(0, std::to_string(num & 1));
    num = num >> 1;
  }

  while (num > 0)
  {
    str.insert(0, std::to_string(num & 1));
    num = num >> 1;
  }

  std::size_t size = sizeof(num) * 8 - str.length();
  str.insert(0, size, '0');


  return str == "" ? "0" : str;
}


/*template<class Score>
//std::vector<std::size_t> probabilisticSample(std::vector<Score> scores, int count)
{
  std::vector<std::size_t> sample;

  std::default_random_engine gen((unsigned)std::chrono::system_clock::now().time_since_epoch().count());

  int i = 0;

  while (i < count)
  {
    Score totalScore = 0;

    std::for_each(scores.begin(), scores.end(), [&totalScore](Score score) {totalScore += score; });

    if (totalScore == 0)
    {
      return sample;
    }

    std::uniform_real_distribution<double> dist(std::numeric_limits<double>::min(), totalScore);

    auto it = scores.begin();

    double randomVariable = dist(gen);

    //Score or double??
    double theOne = randomVariable - *it;

      while (theOne > 0)
      {
        //Could be one line but it's messy.
        ++it;
        theOne -= *it;
      }

    //Better than distance beacuse it wouldn't compile if you change the vector into a list which would be inefficient.
    std::size_t index = it - scores.begin();

    sample.push_back(index);

    *it = 0;
    i++;
  }

  return sample;
}

*/

template<class T>
struct ff {
  T operator() (Gene<T> g)
  {
    return g.getDNA();
  }

  static constexpr T thresh = std::numeric_limits<T>::max();


};

template<class T>
struct ffImp {
  int operator() (Gene<T> g)
  {
    int i = 0;

    int acc = 1;

    T dna = g.getDNA();

    while (dna > 0)
    {
      int val = dna & 0x01;

      i += acc * val;

      acc++;

      dna = dna >> 1;

    }

    return (g.getDNA() % 2) == 0 ? i : 1;
  };

  static constexpr int thresh = (((sizeof(T) * 8) * (sizeof(T) * 8 + 1)) / 2)-1;

};


int main()
{
  //CrossoverStrategy cs = ;

  //#define GENETIC_DIAGNOSTIC

  GeneticAlgorithm<ffImp<unsigned >, unsigned , CrossoverStrategy::Single, unsigned > ga(32, ffImp<unsigned >(), ffImp<unsigned>::thresh, 100, 0.75, 0.02);

  Gene<unsigned > gene = ga.run();

  std::cout << gene << ", " << gene.getDNA() << std::endl;

  //std::cout << (unsigned long long) (std::numeric_limits<unsigned long long>::max() - std::numeric_limits<long long>::min());




  


  
}