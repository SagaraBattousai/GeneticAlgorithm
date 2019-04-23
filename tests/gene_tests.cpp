#include "gene.h"
#include "gtest/gtest.h"
#include <cstdint>

  template<class T>
  class GeneTest : public ::testing::Test {
    
  protected:

    GeneTest();

    ~GeneTest() override {}

    bool isPower2(T value);

    //seed_t seed;

    Gene<T> gene;

  };

  template<class T>
  GeneTest<T>::GeneTest() : gene(Gene<T>())
  {
    //seed = { 15560541341198941 };
    //gene = Gene<T>();//(seed);
  }

  using GeneTypes = ::testing::Types<char, short, int, long, long long,
                                     unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long,
                                     int_fast8_t, int_fast16_t, int_fast32_t, int_fast64_t,
                                     uint_fast8_t, uint_fast16_t, uint_fast32_t, uint_fast64_t>;

  TYPED_TEST_SUITE(GeneTest, GeneTypes);

  template<class T>
  bool GeneTest<T>::isPower2(T value)
  {
    if (value < 0)
    {
      //damn signed values
      return (value & ((value + 1) * -1)) == 0;
    }

    return value == 0 ? false : (value & (value - 1)) == 0;
  }

  TYPED_TEST(GeneTest, GeneMutatesOnlyOneBit)
  {
    TypeParam DNA = gene.getDNA();

    //long long seed = std::chrono::system_clock::now().time_since_epoch().count();

    Gene<TypeParam> mutatedGene = gene.mutate();// this->seed.seed);

    TypeParam mutatedDNA = mutatedGene.getDNA();

    EXPECT_TRUE(isPower2(DNA ^ mutatedDNA)) << "DNA:        " << gene << std::endl << "DNA:        " << (long long)DNA << std::endl
                                            << "MutatedDNA: " << mutatedGene << std::endl << "MutatedDNA: " << (long long)mutatedDNA << std::endl
                                            << "XOR:        " << (DNA ^ mutatedDNA) << " isPower2: " << ((DNA ^ mutatedDNA) & ((DNA ^ mutatedDNA) - 1)) << std::endl;
                                         // << "With seed:  " << seed;

  }

  /*
  TYPED_TEST(GeneTest, SingleCrossover)

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
  
  */
