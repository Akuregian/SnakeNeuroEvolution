// This File will determine all the settings for the Neural Network, Genetic Algorithm && Snake

namespace NeuralSettings {

	// Neural Network Topology
	const std::vector<unsigned int> TOPOLOGY = { 3, 2, 1 };
}

namespace WindowSettings {

	// Window Settings
	const unsigned int Width = 1200;
	const unsigned int Height = 800;
}

namespace GameSettings {

	// How Fast eac Frame Ticks
	const unsigned int TickSpeed = 50;
	const unsigned int BoardX = 10;
	const unsigned int BoardY = 10;
}

namespace GeneticSettings {

	// Genetic Algorithm 
	const int POP_SIZE = 1; // 500
	const int MATING_POP_SIZE = 2; // 250

	// Mutation Rate
	const double MUTATION_RATE = 0.05;
	const unsigned int MAX_STEPS_WITHOUT_EATING = GameSettings::BoardX * GameSettings::BoardY;
	const unsigned int MAX_GENERATIONS = 5000;
}
