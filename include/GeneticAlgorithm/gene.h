#include <random>
#include <chrono>
#include <cstddef>
#include <array>
#include <string>
#include <iostream>
#include <cstdint>

typedef struct { long long seed; } seed_t;


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

  Gene<T> mutate(long long seed) const;

  template<class T>
  friend std::array<Gene<T>, 2> singleCrossover(Gene<T> p1, Gene<T> p2);

  template<class T>
  friend std::array<Gene<T>, 2> singleCrossover(Gene<T> p1, Gene<T> p2, long long seed);

  template<class T>
  friend std::array<Gene<T>, 2> dualCrossover(Gene<T> p1, Gene<T> p2);

  template<class T>
  friend std::array<Gene<T>, 2> dualCrossover(Gene<T> p1, Gene<T> p2, long long seed1, long long seed2);
  
  template<class T>
  friend std::array<Gene<T>, 2> uniformCrossover(Gene<T> p1, Gene<T> p2);

  template<class T>
  friend std::array<Gene<T>, 2> uniformCrossover(Gene<T> p1, Gene<T> p2, long long seed);
  

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
  Gene(bit_length, std::chrono::system_clock::now().time_since_epoch().count())
{}

template<class T>
Gene<T>::Gene(std::size_t bit_length, seed_t seed)
  :
  bit_length(bit_length),
  bit_representation(std::uniform_int_distribution<T>(0, (((long long)1) << bit_length) - 1)(std::default_random_engine(seed.seed)))
{}

template<>
Gene<char>::Gene(std::size_t bit_length, seed_t seed)
  :
  bit_length(bit_length),
  bit_representation(std::uniform_int_distribution<int_fast16_t>(0, (((long long)1) << bit_length) - 1)(std::default_random_engine(seed.seed)))
{}

template<>
Gene<unsigned char>::Gene(std::size_t bit_length, seed_t seed)
  :
  bit_length(bit_length),
  bit_representation(std::uniform_int_distribution<uint_fast16_t>(0, (((long long)1) << bit_length) - 1)(std::default_random_engine(seed.seed)))
{}

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
  return mutate(std::chrono::system_clock::now().time_since_epoch().count());
}

template<class T>
Gene<T> Gene<T>::mutate(long long seed) const
{
  auto dist = std::uniform_int_distribution<std::size_t>(0, bit_length - 1);

  std::size_t modIndex = dist(std::default_random_engine(seed));

  T modBit = (long long)1 << modIndex;

  return Gene<T>(this->bit_length, this->bit_representation ^ modBit);
}

template<class T>
std::array<Gene<T>, 2> singleCrossover(Gene<T> p1, Gene<T> p2)
{
  return singleCrossover(p1, p2, std::chrono::system_clock::now().time_since_epoch().count());
}

template<class T>
std::array<Gene<T>, 2> singleCrossover(Gene<T> p1, Gene<T> p2, long long seed)
{
  auto dist = std::uniform_int_distribution<std::size_t>(1, p1.bit_length - 1);

  std::size_t modIndex = dist(std::default_random_engine(seed));

  std::array<T, 2> masks = Gene<T>::generateMask<1>({ modIndex }, p1.bit_length);

  return crossover(p1, p2, masks[0], masks[1]);
}

template<class T>
std::array<Gene<T>, 2> dualCrossover(Gene<T> p1, Gene<T> p2)
{
  return dualCrossover(p1, p2, std::chrono::system_clock::now().time_since_epoch().count(), std::chrono::system_clock::now().time_since_epoch().count());
}

template<class T>
std::array<Gene<T>, 2> dualCrossover(Gene<T> p1, Gene<T> p2, long long seed1, long long seed2)
{
  auto dist0 = std::uniform_int_distribution<std::size_t>(1, p1.bit_length - 2);

  std::size_t firstIndex = dist0(std::default_random_engine(seed1));

  auto dist1 = std::uniform_int_distribution<std::size_t>(1, p1.bit_length - firstIndex - 1);

  std::size_t secondIndex = dist1(std::default_random_engine(seed2));

  std::array<T, 2> masks = Gene<T>::generateMask<2>({ firstIndex, secondIndex }, p1.bit_length);

  return crossover(p1, p2, masks[0], masks[1]);
}

template<class T>
std::array<Gene<T>, 2> uniformCrossover(Gene<T> p1, Gene<T> p2)
{
  return uniformCrossover(p1, p2, std::chrono::system_clock::now().time_since_epoch().count());
}

template<class T>
std::array<Gene<T>, 2> uniformCrossover(Gene<T> p1, Gene<T> p2, long long seed)
{
  T ones = (((long long)1) << p1.bit_length) - 1;

  auto dist = std::uniform_int_distribution<T>(0, ones);

  T mask = dist(std::default_random_engine(seed));

  T flip = mask ^ ones;

  return crossover(p1, p2, mask, flip);
}

template<>
std::array<Gene<char>, 2> uniformCrossover(Gene<char> p1, Gene<char> p2, long long seed)
{
  char ones = (((long long)1) << p1.bit_length) - 1;

  auto dist = std::uniform_int_distribution<int_fast16_t>(0, ones);

  char mask = dist(std::default_random_engine(seed));

  char flip = mask ^ ones;

  return crossover(p1, p2, mask, flip);
}

template<>
std::array<Gene<unsigned char>, 2> uniformCrossover(Gene<unsigned char> p1, Gene<unsigned char> p2, long long seed)
{
  unsigned char ones = (((long long)1) << p1.bit_length) - 1;

  auto dist = std::uniform_int_distribution<uint_fast16_t>(0, ones);

  unsigned char mask = dist(std::default_random_engine(seed));

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

  T mask = (maskValue << crossoverPoints[0]) - 1;

  int i = 0;

  for (auto& cp = crossoverPoints.begin() + 1; cp < crossoverPoints.end(); cp++)
  {
    mask = mask << (*cp);

    if (i % 2 == 1)
    {
      T ones = (maskValue << *cp) - 1;
      mask = mask & ones;
    }

    maskBitsCount += *cp;
    i++;
  }

  std::size_t lower = bit_length - maskBitsCount;

  mask = mask << lower;

  //even push on zeros, odd push on ones
  if (i % 2 == 1)
  {
    T ones = (maskValue << lower) - 1;
    mask = mask | ones;
  }

  T flip = (maskValue << bit_length) - 1;

  return { mask, mask ^ flip };
}
