// This File will determine all the settings for the Neural Network, Genetic Algorithm && Snake

namespace GeneticSettings {

	// Genetic Algorithm 
	const int POP_SIZE = 10; // 500
	const int MATING_POP_SIZE = 5; // 250

	// Mutation Rate
	const double MUTATION_RATE = 0.05;
}

namespace NeuralSettings {

	// Neural Network
	const std::vector<unsigned int> TOPOLOGY = { 3, 2, 1 };
}