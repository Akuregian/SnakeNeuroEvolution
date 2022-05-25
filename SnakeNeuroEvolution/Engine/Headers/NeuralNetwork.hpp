#pragma once
#include "Eigen/Core"
#include "Settings.hpp"
#include "Logger.hpp"
#include "Seed.hpp"

namespace NeuroEvolution {

	typedef Eigen::MatrixXd MAT_D;
	typedef Eigen::VectorXd VEC_D;

	struct NewChildWeights
	{
		NeuroEvolution::MAT_D One;
		NeuroEvolution::MAT_D Two;
	};

	struct NewChildBias
	{
		NeuroEvolution::VEC_D One;
		NeuroEvolution::VEC_D Two;
	};

	class NeuralNetwork {
	public:
		NeuralNetwork();
		~NeuralNetwork();
		NeuralNetwork(std::vector<unsigned int> topology);
		NeuralNetwork(std::vector<MAT_D>& w1, std::vector<VEC_D>& b1, std::vector<unsigned int> topology);
		void CreateNeuralNetwork(const std::vector<unsigned int> topology);
		const int& ForwardPropagate(VEC_D& input_vector);

		// Logging
		void PrintNetworkToConsole();

		void TweakWeights();

		// Activation Functions
		static double Sigmoid(const double& x);
		static double RELU(const double& x);

		double NetworkFitness;

		std::vector<unsigned int> _Topology;
		std::vector<MAT_D> _Neurons;
		std::vector<MAT_D> _Weights;
		std::vector<VEC_D> _Bias;

	};
}