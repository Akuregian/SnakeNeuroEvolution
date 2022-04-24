// This File will determine all the settings for the Neural Network, Genetic Algorithm && Snake

namespace NeuralSettings {

	// Neural Network Topology
	const std::vector<unsigned int> TOPOLOGY = { 32, 20, 12, 4 };
}

namespace WindowSettings {

	// Window Settings
	const unsigned int Width = 1200;
	const unsigned int Height = 800;
}

namespace GameSettings {

	// How Fast eac Frame Ticks
	const unsigned int TickSpeed = 500; // 10
	const unsigned int BoardX = 20; // 40
	const unsigned int BoardY = 20; // 20
}

namespace GeneticSettings {

	// Genetic Algorithm 
	const int POP_SIZE = 25; // 250
	const int MATING_POP_SIZE = 50; // 500

	// Mutation Rate
	const double MUTATION_RATE = 0.05;
	const unsigned int MAX_STEPS_WITHOUT_EATING = GameSettings::BoardX * GameSettings::BoardY;
	const unsigned int MAX_GENERATIONS = 5000;
}
