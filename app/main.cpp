#include <iostream>
//#include "gene.h"
#include <string>
#include <array>
#include <random>
#include <cstddef>


double expected(int bit_length)
{
  long long max = (((long long)1) << bit_length) - 1;

  return max / 2;
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

/*
template<class T, int numCrossovers>
std::array<T, 2> generateMask(std::array<std::size_t, numCrossovers> crossoverPoints, std::size_t bit_length)
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
*/


int main()
{
 
  /*double avg = 0;

  int constexpr bit_length = 4;
  Gene<long long> g = Gene<long long>(bit_length);

  for (int i = 0; i < 100000; i++)
  {
    
    avg += g.bit_representation;
    g = Gene<long long>(bit_length);
  }

  avg /= 100000;
  
  std::string bin = to_binary(g.bit_representation);

  double exp = expected(bit_length);

  std::cout <<  exp - avg << " " << bin << " " << bin.length() << " " << g.bit_representation << std::endl;
  */
  
 /* Gene<int> p1 = Gene<int>(16, 0b1111000011110000);
  
  Gene<int> p2 = Gene<int>(16, 0b1001100110011001);

  Gene<int> *c1 = uniformCrossover(p1, p2).data();
  
  std::cout << p1 << " : "  << p2 << " : "  << c1[0] << " : " << c1[1]  << std::endl;
  

  int ix = -5;

  int iy = -1025;

  unsigned ux = 5;

  unsigned uy = 1025;

  unsigned unx = -5;

  unsigned uny = -1025;

  std::cout << to_binary<int>(ix) << " : " << to_binary<char>((char)ix) << " : " << to_binary<unsigned char>((unsigned char)ix) << std::endl;

  std::cout << to_binary<int>(iy) << " : " << to_binary<char>((char)iy) << " : " << to_binary<unsigned char>((unsigned char)iy) << std::endl;

  std::cout << to_binary<unsigned>(ux) << " : " << to_binary<char>((char)ux) << " : " << to_binary<unsigned char>((unsigned char)ux) << std::endl;

  std::cout << to_binary<unsigned>(uy) << " : " << to_binary<char>((char)uy) << " : " << to_binary<unsigned char>((unsigned char)uy) << std::endl;

  std::cout << to_binary<unsigned>(unx) << " : " << to_binary<char>((char)unx) << " : " << to_binary<unsigned char>((unsigned char)unx) << std::endl;

  std::cout << to_binary<unsigned>(uny) << " : " << to_binary<char>((char)uny) << " : " << to_binary<unsigned char>((unsigned char)uny) << std::endl;

  //std::array<int, 2> masks = generateMask<int, 1>({ 4 }, 8);

  //std::cout << to_binary(masks[0]) << " : " << to_binary(masks[1]) << std::endl;

  //std::cout << "HI" << std::endl;

  */
 

  return 0;
}