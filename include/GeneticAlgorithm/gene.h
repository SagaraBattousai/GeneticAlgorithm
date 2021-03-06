#pragma once

#include <random>
#include <chrono>
#include <cstddef>
#include <array>
#include <string>
#include <iostream>
#include <cstdint>

typedef struct { unsigned seed; } seed_t;


template <class T>
class Gene
{

public:

  Gene<T>();

  Gene<T>(seed_t seed);

  Gene<T>(std::size_t bit_length);

  Gene<T>(std::size_t bit_length, seed_t seed);

  T getDNA() const;

  Gene<T> mutate() const;

  Gene<T> mutate(unsigned seed) const;

  template<class T>
  friend std::array<Gene<T>, 2> singleCrossover(Gene<T> p1, Gene<T> p2);

  template<class T>
  friend std::array<Gene<T>, 2> singleCrossover(Gene<T> p1, Gene<T> p2, unsigned seed);

  template<class T>
  friend std::array<Gene<T>, 2> dualCrossover(Gene<T> p1, Gene<T> p2);

  template<class T>
  friend std::array<Gene<T>, 2> dualCrossover(Gene<T> p1, Gene<T> p2, unsigned seed1, unsigned seed2);
  
  template<class T>
  friend std::array<Gene<T>, 2> uniformCrossover(Gene<T> p1, Gene<T> p2);

  template<class T>
  friend std::array<Gene<T>, 2> uniformCrossover(Gene<T> p1, Gene<T> p2, unsigned seed);
  

  template<class T>
  friend std::ostream& operator<<(std::ostream& os, Gene<T> gene);

protected:
  Gene<T>(std::size_t bit_length, T bit_representation);

private:

  std::size_t bit_length;

  T bit_representation;


  template<class T>
  friend std::array<Gene<T>, 2> crossover(Gene<T> p1, Gene<T> p2, T mask, T flip);

  template<int numCrossovers>
  static std::array<T, 2> generateMask(std::array<std::size_t, numCrossovers> crossoverPoints, std::size_t bit_length);

  std::string to_binary();
 
};


template<class T>
Gene<T>::Gene() : Gene(sizeof(T) * 8)
{}

template<class T>
Gene<T>::Gene(seed_t seed) : Gene(sizeof(T) * 8, seed.seed)
{}

template<class T>
Gene<T>::Gene(std::size_t bit_length)
  : 
  Gene(bit_length, seed_t{ (unsigned)std::chrono::system_clock::now().time_since_epoch().count() })
{}

template<class T>
Gene<T>::Gene(std::size_t bit_length, seed_t seed) : bit_length(bit_length)
{
  auto dre = std::default_random_engine(seed.seed);
  T max_value = (T) std::numeric_limits<unsigned long long>::max();
  bit_representation = std::uniform_int_distribution<T>(0, max_value)(dre);
}

template<>
Gene<char>::Gene(std::size_t bit_length, seed_t seed) : bit_length(bit_length)
{
  auto dre = std::default_random_engine(seed.seed);
  bit_representation = (char) std::uniform_int_distribution<int_fast16_t>(0, UINT_FAST8_MAX)(dre);
}

template<>
Gene<unsigned char>::Gene(std::size_t bit_length, seed_t seed) : bit_length(bit_length)
{
  auto dre = std::default_random_engine(seed.seed);
  bit_representation = (unsigned char) std::uniform_int_distribution<uint_fast16_t>(0, UINT_FAST8_MAX)(dre);
}

template<class T>
Gene<T>::Gene(std::size_t bit_length, T bit_representation)
  :
  bit_length(bit_length),
  bit_representation(bit_representation)
{}

template<class T>
T Gene<T>::getDNA() const
{
  return bit_representation;
}

template<class T>
Gene<T> Gene<T>::mutate() const
{
  return mutate((unsigned) std::chrono::system_clock::now().time_since_epoch().count());
}

template<class T>
Gene<T> Gene<T>::mutate(unsigned seed) const
{
  auto dist = std::uniform_int_distribution<std::size_t>(0, bit_length - 1);

  auto dre = std::default_random_engine(seed);

  std::size_t modIndex = dist(dre);

  T modBit = (long long)1 << modIndex;

  return Gene<T>(this->bit_length, this->bit_representation ^ modBit);
}

template<class T>
std::array<Gene<T>, 2> singleCrossover(Gene<T> p1, Gene<T> p2)
{
  return singleCrossover(p1, p2, (unsigned) std::chrono::system_clock::now().time_since_epoch().count());
}

template<class T>
std::array<Gene<T>, 2> singleCrossover(Gene<T> p1, Gene<T> p2, unsigned seed)
{
  auto dist = std::uniform_int_distribution<std::size_t>(1, p1.bit_length - 1);

  std::default_random_engine gen(seed);

  std::size_t modIndex = dist(gen);

  std::array<T, 2> masks = Gene<T>::generateMask<1>({ modIndex }, p1.bit_length);

  return crossover(p1, p2, masks[0], masks[1]);
}

template<class T>
std::array<Gene<T>, 2> dualCrossover(Gene<T> p1, Gene<T> p2)
{
  return dualCrossover(p1, p2, (unsigned) std::chrono::system_clock::now().time_since_epoch().count(), (unsigned) std::chrono::system_clock::now().time_since_epoch().count());
}

template<class T>
std::array<Gene<T>, 2> dualCrossover(Gene<T> p1, Gene<T> p2, unsigned seed1, unsigned seed2)
{
  auto dist0 = std::uniform_int_distribution<std::size_t>(1, p1.bit_length - 2);

  std::default_random_engine gen1(seed1);

  std::size_t firstIndex = dist0(gen1);

  auto dist1 = std::uniform_int_distribution<std::size_t>(1, p1.bit_length - firstIndex - 1);

  std::default_random_engine gen2(seed2);

  std::size_t secondIndex = dist1(gen2);

  std::array<T, 2> masks = Gene<T>::generateMask<2>({ firstIndex, secondIndex }, p1.bit_length);

  return crossover(p1, p2, masks[0], masks[1]);
}

template<class T>
std::array<Gene<T>, 2> uniformCrossover(Gene<T> p1, Gene<T> p2)
{
  return uniformCrossover(p1, p2, (unsigned) std::chrono::system_clock::now().time_since_epoch().count());
}

template<class T>
std::array<Gene<T>, 2> uniformCrossover(Gene<T> p1, Gene<T> p2, unsigned seed)
{
  T ones = (T)std::numeric_limits<unsigned long long>::max(); //(((long long)1) << p1.bit_length) - 1;

  std::default_random_engine gen(seed);

  auto dist = std::uniform_int_distribution<T>(0, ones);

  T mask = dist(gen);

  T flip = mask ^ ones;

  return crossover(p1, p2, mask, flip);
}

template<>
std::array<Gene<char>, 2> uniformCrossover(Gene<char> p1, Gene<char> p2, unsigned seed)
{
  char ones = std::numeric_limits<unsigned char>::max(); //(((long long)1) << p1.bit_length) - 1;

  auto dist = std::uniform_int_distribution<int_fast16_t>(0, ones);

  auto dre = std::default_random_engine(seed);

  char mask = (char) dist(dre);

  char flip = mask ^ ones;

  return crossover(p1, p2, mask, flip);
}

template<>
std::array<Gene<unsigned char>, 2> uniformCrossover(Gene<unsigned char> p1, Gene<unsigned char> p2, unsigned seed)
{
  unsigned char ones = std::numeric_limits<unsigned char>::max();// (((long long)1) << p1.bit_length) - 1;

  auto dist = std::uniform_int_distribution<uint_fast16_t>(0, ones);

  auto dre = std::default_random_engine(seed);

  unsigned char mask = (unsigned char) dist(dre);

  unsigned char flip = mask ^ ones;

  return crossover(p1, p2, mask, flip);
}

template<class T>
std::array<Gene<T>, 2> crossover(Gene<T> p1, Gene<T> p2, T mask, T flip)
{
  T c1_bit_representation = p1.bit_representation & mask;

  c1_bit_representation |= (p2.bit_representation & flip);

  T c2_bit_representation = p2.bit_representation & mask;

  c2_bit_representation |= (p1.bit_representation & flip);

  Gene<T> c1 = Gene<T>(p1.bit_length, c1_bit_representation);

  Gene<T> c2 = Gene<T>(p2.bit_length, c2_bit_representation);


  return { c1, c2 };
}

template<class T>
std::ostream& operator<<(std::ostream& os, Gene<T> gene)
{
  os << gene.to_binary();

  return os;
}

template<class T>
std::string Gene<T>::to_binary()
{
  T num = bit_representation;

  std::string str = "";

  while (num < 0)
  {
    if (num == -1)
    {
      std::size_t size = bit_length - str.length();
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

  std::size_t size = bit_length - str.length();
  str.insert(0, size, '0');

  return str;
}

template<class T>
template<int numCrossovers>
std::array<T, 2> Gene<T>::generateMask(std::array<std::size_t, numCrossovers> crossoverPoints, std::size_t bit_length)
{
  long long maskValue = 1;

  std::size_t maskBitsCount = crossoverPoints[0];

  T mask = (T) (maskValue << crossoverPoints[0]) - 1;

  int i = 0;

  for (auto cp = crossoverPoints.begin() + 1; cp < crossoverPoints.end(); cp++)
  {
    mask = mask << (*cp);

    if (i % 2 == 1)
    {
      T ones = (T) (maskValue << *cp) - 1;
      mask = mask & ones;
    }

    maskBitsCount += *cp;
    i++;
  }

  std::size_t lower = bit_length - maskBitsCount;

  mask = (T) (mask << lower);

  //even push on zeros, odd push on ones
  if (i % 2 == 1)
  {
    T ones = (T) (maskValue << lower) - 1;
    mask = (T) (mask | ones);
  }

  T flip = (T) std::numeric_limits<unsigned long long>::max(); //(T)(maskValue << bit_length) - 1;

  return { mask, (T) (mask ^ flip) };
}
