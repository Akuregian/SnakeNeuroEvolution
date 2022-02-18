#include "../Headers/GeneticAlgorithm.hpp"

namespace NeuroEvolution {

	GeneticAlgorithm::GeneticAlgorithm()
	{
		NeuroEvolution::Logger::Init();
		ENGINE_INIT_WARN("GeneticAlgorithm Initialized");
	}
	GeneticAlgorithm::~GeneticAlgorithm()
	{
		ENGINE_INIT_WARN("Genetic Algorithm Object Destroyed");
	}
	
	const double GeneticAlgorithm::CalculateFitness(const int& steps, const int& score)
	{
		// f(steps, score) = steps + (2^score + 500 * score^2.1) - (0.25 * steps^1.3 * score^1.2)
		double calculatedFitness = steps + (std::pow(2, score) + std::pow(score, 2.1) * 500) - (std::pow(0.25 * steps, 1.3) * (std::pow(score, 1.2)));
		return calculatedFitness;
	}

	void GeneticAlgorithm::ElitismSelection(std::vector<std::shared_ptr<Entity>>& curr_pop)
	{
		ENGINE_LOGGER_INFO("Sorting Population By Fitness Scores");
		std::sort(curr_pop.begin(), curr_pop.end(), [](const std::shared_ptr<Entity> lhs, const std::shared_ptr<Entity> rhs)
		{ 
			return lhs->_Brain->NetworkFitness > rhs->_Brain->NetworkFitness; 
		});
	}

	Parents GeneticAlgorithm::RouletteWheel(const double& roulette_wheel_sum, std::vector<std::shared_ptr<Entity>>& mating_pop )
	{
		unsigned parent_1 = NULL;
		unsigned parent_2 = NULL;

		std::uniform_real_distribution<double> RandomNumber(0, roulette_wheel_sum);
		std::random_device rd;
		std::mt19937 Generator(rd());

		for (unsigned i = 0; i < 2; i++) {
			double pick = RandomNumber(Generator);
			double current = 0;
			for (unsigned pop_index = 0; pop_index < GeneticSettings::POP_SIZE; pop_index++) {
				current += mating_pop[i]->_Brain->NetworkFitness;
					if (current >= pick) {
					if (parent_1 == NULL) {
						parent_1 = pop_index;
					}
					else {
						parent_2 = pop_index;
					}
					break;
				}
			}
		}

		Parents parents {parent_1, parent_2};
		return parents;
	}

	void GeneticAlgorithm::CrossoverAndMutation(const double& roulette_wheel_sum, std::vector<std::shared_ptr<Entity>>& mating_pop, std::vector<std::shared_ptr<Entity>>& entity_pop )
	{
		Parents selectedParents = GeneticAlgorithm::RouletteWheel(roulette_wheel_sum, mating_pop);

		// Grab Parents Weigths and Bias
		const std::vector<NeuroEvolution::MAT_D>& parent_1_weights = mating_pop[selectedParents.One]->_Brain->_Weights;
		const std::vector<NeuroEvolution::VEC_D>& parent_1_bias = mating_pop[selectedParents.One]->_Brain->_Bias;

		const std::vector<NeuroEvolution::MAT_D>& parent_2_weights = mating_pop[selectedParents.Two]->_Brain->_Weights;
		const std::vector<NeuroEvolution::VEC_D>& parent_2_bias = mating_pop[selectedParents.Two]->_Brain->_Bias;
		int layers = parent_1_weights.size();

		std::vector<NeuroEvolution::MAT_D> child_1_weights;
		std::vector<NeuroEvolution::VEC_D> child_1_bias;

		std::vector<NeuroEvolution::MAT_D> child_2_weights;
		std::vector<NeuroEvolution::VEC_D> child_2_bias;

		for (unsigned int i = 0; i < layers; i++) {

			// crossover Weights
			NewChildWeights Weights = SinglePointCrossover(parent_1_weights[i], parent_2_weights[i]);

			// Crossover Bias
			NewChildBias Bias = SinglePointCrossover(parent_1_bias[i], parent_2_bias[i]);

			GausssianMutation(Weights.One);
			GausssianMutation(Weights.Two);
			GausssianMutation(Bias.One);
			GausssianMutation(Bias.Two);

			child_1_weights.push_back(Weights.One);
			child_1_bias.push_back(Bias.One);

			child_2_weights.push_back(Weights.Two);
			child_2_bias.push_back(Bias.Two);
		}

		// Add New Children To the next population
		if (entity_pop.size() == GeneticSettings::POP_SIZE - 1)
		{
			ENGINE_LOGGER_INFO("Adding One New Children too the Population");
			entity_pop.push_back(std::make_shared<Entity>(child_1_weights, child_1_bias));
		}
		else 
		{
			ENGINE_LOGGER_INFO("Adding Two New Children too the Population");
			entity_pop.push_back(std::make_shared<Entity>(child_1_weights, child_1_bias));
			entity_pop.push_back(std::make_shared<Entity>(child_2_weights, child_2_bias));
		}

		assert(entity_pop.size() <= GeneticSettings::POP_SIZE);
	}
	
	// Crossover of weights && bias'
	NewChildWeights GeneticAlgorithm::SinglePointCrossover(const NeuroEvolution::MAT_D& w1, const NeuroEvolution::MAT_D& w2)
	{
		ENGINE_LOGGER_INFO("SinglePoint Crossover {Weights}");

		// AGAIN...........................
		std::random_device rd;
		std::mt19937 generator(rd());

		int rows = w1.rows();
		int cols = w1.cols();
		int randomRow = std::uniform_int_distribution<int>(0, rows - 1)(generator);
		int randomCol = std::uniform_int_distribution<int>(0, cols - 1)(generator);

		Eigen::MatrixXd child_1 = w1;
		Eigen::MatrixXd child_2 = w2;

		child_1.topRows(randomRow + 1).swap(child_2.topRows(randomRow + 1));
		child_1.block(randomRow, randomCol, 1, cols - randomCol).swap(child_2.block(randomRow, randomCol, 1, cols - randomCol));

		return { child_1, child_2 };
	}

	NewChildBias GeneticAlgorithm::SinglePointCrossover(const NeuroEvolution::VEC_D& b1, const NeuroEvolution::VEC_D& b2)
	{
		ENGINE_LOGGER_INFO("SinglePoint Crossover {Bias}");

		//-------------------------- FIX -----------------------------------------
		std::random_device rd;
		std::mt19937 generator(rd());
		//------------------------------------------------------------------------


		int rows = b1.rows();
		int randomRow = std::uniform_int_distribution<int>(0, rows - 1)(generator);

		Eigen::MatrixXd child_1 = b1;
		Eigen::MatrixXd child_2 = b2;

		child_1.topRows(randomRow + 1).swap(child_2.topRows(randomRow + 1));

		return { child_1, child_2 };
	}

	// Mutation of Weights or Bias'
	void GeneticAlgorithm::GausssianMutation(NeuroEvolution::MAT_D& child_weights)
	{	
		//-------------------------- FIX -----------------------------------------
		std::random_device rd;
		std::mt19937 generator(rd());
		std::normal_distribution<double> random_normal(0.0, 1.0);
		std::uniform_real_distribution<double> random_uniform(0.0, 1.0);
		//------------------------------------------------------------------------

		int rows = child_weights.rows();
		int cols = child_weights.cols();

		// Determine which Genes are Going to be Mutated by using the Mutation Probability
		NeuroEvolution::MAT_D random_chance = Eigen::MatrixXd(rows, cols).unaryExpr([&](double dummy_var) { return random_uniform(generator); });

		// Create a random Matrix using Normal Distribution (Gaussian)
		NeuroEvolution::MAT_D mutation = Eigen::MatrixXd(rows, cols).unaryExpr([&](double dummy_var) { return random_normal(generator); }) + child_weights;

		// Add The Randomly Created Matrix *= Scale to the Original Matrix (Update Weights || Bias Matrix)
		child_weights = (random_chance.array() < GeneticSettings::MUTATION_RATE).select(mutation, child_weights);
		
		ENGINE_LOGGER_INFO("Mutated Weights");
	}
	void GeneticAlgorithm::GausssianMutation(NeuroEvolution::VEC_D& child_bias)
	{
		//-------------------------- FIX -----------------------------------------
		std::random_device rd;
		std::mt19937 generator(rd());
		std::normal_distribution<double> random_normal(0.0, 1.0);
		std::uniform_real_distribution<double> random_uniform(0.0, 1.0);
		//------------------------------------------------------------------------

		int rows = child_bias.rows();

		// Determine which Genes are Going to be Mutated by using the Mutation Probability
		NeuroEvolution::VEC_D random_chance = Eigen::VectorXd(rows).unaryExpr([&](double dummy_var) { return random_uniform(generator); });

		// Create a random Matrix using Normal Distribution (Gaussian)
		NeuroEvolution::VEC_D mutation = Eigen::VectorXd(rows).unaryExpr([&](double dummy_var) { return random_normal(generator); }) + child_bias;

		// Add The Randomly Created Matrix *= Scale to the Original Matrix (Update Weights || Bias Matrix)
		child_bias = (random_chance.array() < GeneticSettings::MUTATION_RATE).select(mutation, child_bias);

		ENGINE_LOGGER_INFO("Mutated Bias");
	}
}