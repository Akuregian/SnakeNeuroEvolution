#pragma once
#include "Entity.hpp"

namespace NeuroEvolution {

	struct Parents
	{
		unsigned int One;
		unsigned int Two;
	};

	class GeneticAlgorithm {
	public:
		GeneticAlgorithm();
		~GeneticAlgorithm();
		static const double CalculateFitness(const int& steps, const int& score);
		static void ElitismSelection(std::vector<std::shared_ptr<Entity>>& curr_pop);
		static Parents RouletteWheel(const double& roulette_wheel_sum, std::vector<std::shared_ptr<Entity>>& mating_pop);
		static void CrossoverAndMutation(const double& roulette_wheel_sum, std::vector<std::shared_ptr<Entity>>& mating_pop, std::vector<std::shared_ptr<Entity>>& entity_pop);

		// Crossover Of Weights or Bias's
		static NewChildWeights SinglePointCrossover(const NeuroEvolution::MAT_D& weights, const NeuroEvolution::MAT_D& bias);
		static NewChildBias SinglePointCrossover(const NeuroEvolution::VEC_D& weights, const NeuroEvolution::VEC_D& bias);

		// Mutation of Weights or Bias's
		static void GausssianMutation(NeuroEvolution::MAT_D& child_weights);
		static void GausssianMutation(NeuroEvolution::VEC_D& child_bias);
	};

}