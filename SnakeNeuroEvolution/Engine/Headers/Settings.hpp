// This File will determine all the settings for the Neural Network, Genetic Algorithm && Snake

namespace GeneticSettings {

	// Genetic Algorithm 
	const int POP_SIZE = 10; // 500
	const int MATING_POP_SIZE = 5; // 250

	// Mutation Rate
	const double MUTATION_RATE = 0.05;
}

namespace NeuralSettings {

	// Neural Network Topology
	const std::vector<unsigned int> TOPOLOGY = { 3, 2, 1 };
}

namespace Window {

	// Window Settings
	const unsigned int Width = 1200;
	const unsigned int Height = 800;
}

namespace Game {

	// How Fast eac Frame Ticks
	const unsigned int TickSpeed = 50;
}