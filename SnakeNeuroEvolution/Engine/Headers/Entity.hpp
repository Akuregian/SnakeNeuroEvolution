#pragma once
#include "NeuralNetwork.hpp"

// This class can be tailored too your specific problem..
// **Need to change the Fitness function in 'Genetic Algorithm class'

namespace NeuroEvolution {

	class Entity {
	public:
		Entity(const std::vector<unsigned int>& topology);
		Entity(std::vector<MAT_D>& w1, std::vector<VEC_D>& b1);
		~Entity();
		
		// Vision Vector is Fed into the Neural Network
		void UpdateVisionVector();

		// Input Vector
		NeuroEvolution::VEC_D input_vector;
		std::shared_ptr<NeuroEvolution::NeuralNetwork> _Brain;

		int direction = 0;
		int steps = 0;
		int score = 0;
	};

}
