#pragma once

#include "gene.h"
#include <cstddef>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "sampling.h"
#include <array>
#include <random>
#include <chrono>

enum class CrossoverStrategy { Single, Dual, Uniform };

template <class T>
std::array<Gene<T>, 2> crossoverUsing(CrossoverStrategy strategy, Gene<T> p1, Gene<T> p2)
{
  switch (strategy)
  {
    case CrossoverStrategy::Single: return singleCrossover(p1, p2);

    case CrossoverStrategy::Dual: return dualCrossover(p1, p2);

    case CrossoverStrategy::Uniform: return uniformCrossover(p1, p2);

    default: return singleCrossover(p1, p2);
  }
}

template <class FitnessFunction, class Fitness, CrossoverStrategy strategy, class T>
class GeneticAlgorithm
{
public:

  //FitnessFunction func, FitnessFunction thresh, population size, percentage to replace with crossover, percentage to mutate
  GeneticAlgorithm<FitnessFunction, Fitness, strategy, T>(std::size_t bit_length, FitnessFunction func, Fitness thresh, std::size_t population_size, double crossoverRate, double mutationRate);

  //seed?
  //GeneticAlgorithm<FitnessFunction, Fitness, T>(std::size_t bit_length, FitnessFunction func, Fitness thresh, std::size_t population_size, double crossover, double mutate);

  Gene<T> run();

private:

  FitnessFunction func;

  Fitness thresh;

  std::size_t population_size;

  double crossoverRate;

  double mutationRate;

  //map instead?
  std::vector<Gene<T>> population;

  std::vector<Fitness> fitness;

  static std::vector<Gene<T>> initPopulation(const std::size_t& population_size, const std::size_t& bit_length);

  void evaluateFitnessFunction();

  std::size_t select(std::vector<Gene<T>>& newPopulation);

  void crossover(std::vector<Gene<T>>& newPopulation, std::size_t crossoverSize);

  void mutate(std::vector<Gene<T>>& newPopulation);

};

//p = population_size, r = crossover, m = mutate
//FitnessFunction func, FitnessFunction thresh, population size, percentage to replace with crossover, percentage to mutate
template <class FitnessFunction, class Fitness, CrossoverStrategy strategy, class T>
GeneticAlgorithm<FitnessFunction, Fitness, strategy, T>::GeneticAlgorithm(std::size_t bit_length, FitnessFunction func, Fitness thresh, std::size_t population_size, double crossoverRate, double mutationRate)
  :
  func(func),
  thresh(thresh),
  population_size(population_size),
  crossoverRate(crossoverRate),
  mutationRate(mutationRate),
  population(initPopulation(population_size, bit_length)),
  fitness(population_size)
{}

template <class FitnessFunction, class Fitness, CrossoverStrategy strategy, class T>
Gene<T> GeneticAlgorithm<FitnessFunction, Fitness, strategy, T>::run()
{
  //Evaluate: For each h in P, compute Fitness(h)'
  evaluateFitnessFunction();

  //While[max Fitness(h)] < Fitnessdhreshold do h
  auto fittestIterator = std::max_element(fitness.begin(), fitness.end());
  
   Fitness fittest = *fittestIterator;

   //debug
   #ifdef GENETIC_DIAGNOSTICS
     std::size_t debug = 0;
   #endif 

  while (fittest < thresh)
  {
    //setSize?
    std::vector<Gene<T>> newPopulation;

    std::size_t crossoverSize = select(newPopulation);

    crossover(newPopulation, crossoverSize);

    mutate(newPopulation);

    //4. Update: P < -Ps.
    this->population = newPopulation;

    //5. Evaluate: for each h in P, compute Fitness(h)
    evaluateFitnessFunction();
    fittestIterator = std::max_element(fitness.begin(), fitness.end());
    fittest = *fittestIterator;

    #ifdef GENETIC_DIAGNOSTICS
      std::cout << debug << ", " << population[fittestIterator - fitness.begin()] << "\r";
      debug++;
    #endif
  }

  #ifdef GENETIC_DIAGNOSTICS
    std::cout << debug << ", " << population[fittestIterator - fitness.begin()] << std::endl;
  #endif

  std::size_t index = fittestIterator - fitness.begin();

  return population[index];
}

template <class FitnessFunction, class Fitness, CrossoverStrategy strategy, class T>
std::vector<Gene<T>> GeneticAlgorithm<FitnessFunction, Fitness, strategy, T>::initPopulation(const std::size_t& population_size, const std::size_t& bit_length)
{
  std::vector<Gene<T>> population;

  for (int i = 0; i < population_size; i++)
  {
    population.push_back(Gene<T>(bit_length));
  }

  return population;
}

template <class FitnessFunction, class Fitness, CrossoverStrategy strategy, class T>
void GeneticAlgorithm<FitnessFunction, Fitness, strategy, T>::evaluateFitnessFunction()
{
  for (std::size_t i = 0; i < population.size(); i++)
  {
    fitness[i] = func(population[i]);
  }
  //std::vector<Gene<T>> population;

  //std::vector<Fitness> FitnessFunction;

}

template <class FitnessFunction, class Fitness, CrossoverStrategy strategy, class T>
std::size_t GeneticAlgorithm<FitnessFunction, Fitness, strategy, T>::select(std::vector<Gene<T>>& newPopulation)
{
  //1. Select: Probabilistically select(1 - r)p members of P to add to Ps.The probability Pr(hi) of
  //   selecting hypothesis hi from P is given by
  std::size_t crossoverSize = (std::size_t) floor((crossoverRate * population_size) / 2) * 2;
  std::size_t survivorSize = (std::size_t) population_size - crossoverSize;

  std::vector<std::size_t> sample = probabilisticSample(fitness, survivorSize);

  for (auto& index : sample)
  {
    newPopulation.push_back(population[index]);
  }

  return crossoverSize;

}

template <class FitnessFunction, class Fitness, CrossoverStrategy strategy, class T>
void GeneticAlgorithm<FitnessFunction, Fitness, strategy, T>::crossover(std::vector<Gene<T>>& newPopulation, std::size_t crossoverSize)
{
  // 2.Crossover: Probabilistically select(r * p) / 2 pairs of hypotheses from P, according to Pr(h_i) given
  //   above.For each pair, (hl, h2), produce two offspring by applying the Crossover operator.
  //   Add all offspring to P.

  std::vector<std::size_t> sample = probabilisticSample(fitness, crossoverSize);

  for (auto it = sample.begin(); it != sample.end(); it += 2)
  {
    Gene<T> p1 = population[*it];
    Gene<T> p2 = population[*(it + 1)];
    std::array<Gene<T>, 2> child = crossoverUsing(strategy, p1, p2);
    newPopulation.push_back(child[0]);
    newPopulation.push_back(child[1]);
  }

}

template <class FitnessFunction, class Fitness, CrossoverStrategy strategy, class T>
void GeneticAlgorithm<FitnessFunction, Fitness, strategy, T>::mutate(std::vector<Gene<T>>& newPopulation)
{
  // 3.Mutate: Choose m percent of the members of Ps, with uniform probability.For each, invert
  //   one randomly selected bit in its representation.

  double mutationAmount = mutationRate * population_size;

  std::size_t mutationSize = 0;

  if (mutationAmount < 1 && mutationAmount > 0)
  {
    mutationSize = 1;
  }
  else
  {
    mutationSize = (std::size_t) floor(mutationAmount);
  }



  std::default_random_engine gen((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<std::size_t> dist(0, population_size);

  //int i = 0;
  std::vector<std::size_t> mutationIndicies = uniformSample((int) 0, (int)population_size - 1, mutationSize);

  for (auto& i : mutationIndicies)
  {
    newPopulation[i] = newPopulation[i].mutate();
  }


}