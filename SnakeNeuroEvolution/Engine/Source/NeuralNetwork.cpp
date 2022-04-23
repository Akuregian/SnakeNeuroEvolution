#include "../Headers/NeuralNetwork.hpp"

namespace NeuroEvolution {

	NeuralNetwork::NeuralNetwork()
	{
	}

	NeuralNetwork::~NeuralNetwork()
	{
	}

	NeuralNetwork::NeuralNetwork(std::vector<unsigned int> topology) 
		: NetworkFitness(0)
	{
		NeuralNetwork::CreateNeuralNetwork(topology);
	}

	// Initialize Neural Network with Previous Weights and Bias Values
	NeuralNetwork::NeuralNetwork(std::vector<MAT_D>& w1, std::vector<VEC_D>& b1, std::vector<unsigned int> topology)
		: _Topology(topology), NetworkFitness(0)
	{

		//	std::cout << "Initialized Neural Network With Previous Weights" << std::endl;
		for (uint64_t i = 0; i < _Topology.size(); i++) {
			// Initialize the layers
			if (i > 0) {
				// Neuron Layers Holds the Values Of Matrix Multiplication
				_Neurons.push_back(Eigen::MatrixXd::Zero(_Topology[i], 1));
			}
			else {
				// Creating the Input Layer
				_Neurons.push_back(Eigen::MatrixXd::Zero(_Topology[i], 1));
			}
		}

		_Weights = w1;
		_Bias = b1;
	}

	void NeuralNetwork::CreateNeuralNetwork(std::vector<unsigned int> topology)
	{
		this->_Topology = topology;

		if (_Topology.size() < 2) {
			assert(_Topology.size() > 1);
		}

		for (unsigned int i = 0; i < _Topology.size(); i++) {
			if (_Topology[i] == 0) {
				assert(_Topology[i] > 0);
			}
		}
		
		for (unsigned i = 0; i < _Topology.size(); i++) {
			// Initialize the layers
			if (i > 0) {
				// Neuron Layers Holds the Values Of Matrix Multiplication
				_Neurons.push_back(MAT_D::Zero(_Topology[i], 1));
				// Creates the weight Matrix N x M 
				_Weights.push_back(MAT_D::Random(_Topology[i], _Topology[i - 1]));
				// Initlialize Bias's
				_Bias.push_back(VEC_D::Random(_Topology[i]));
			}
			else {
				// Creating the Input Layer Neurons
				_Neurons.push_back(MAT_D::Zero(_Topology[i], 1));
			}
		}
	}


	void NeuralNetwork::PrintNetworkToConsole() {
		// ----------------Debugging-------------------

		std::cout << "Neural Layers\n" << std::endl;
		for (int i = 0; i < _Neurons.size(); i++) {
			std::cout << _Neurons[i].format(Eigen::IOFormat(2, 0, ", ", "\n", "[", "]")) << "\n\n" << std::endl;
		}
		std::cout << std::endl;

		std::cout << "_Weights\n" << std::endl;
		for (int i = 0; i < _Weights.size(); i++) {
			std::cout << _Weights[i].format(Eigen::IOFormat(2, 0, ", ", "\n", "[", "]")) << "\n\n" << std::endl;
		}
		std::cout << std::endl;

		std::cout << "_Bais\n" << std::endl;
		for (int i = 0; i < _Bias.size(); i++) {
			std::cout << _Bias[i].format(Eigen::IOFormat(2, 0, ", ", "\n", "[", "]")) << "\n\n" << std::endl;
		}
		std::cout << std::endl;
	}

	const int& NeuralNetwork::ForwardPropagate(VEC_D& input) {

		// Set The input too the input Neuron layer
		_Neurons.front().block(0, 0, _Neurons.front().rows(), _Neurons.front().cols()) = input;

		// Propagate the data forward, starting at the layer after the inputLayer ie: HiddenLayer 
		// M x N : M1 == N2 || M2 == N1 ---> If true, we can mulitply the matrices together
		for (unsigned int i = 1; i < _Topology.size(); i++) {
			if (i == _Topology.size() - 1) {
				// Multiply Input Matrix by Weights and store the Values in the NeuronLayer
				_Neurons[i] = (_Weights[i - 1] * _Neurons[i - 1]) + _Bias[i - 1];
				_Neurons[i] = _Neurons[i].unaryExpr(&NeuralNetwork::Sigmoid);
			}
			else {
				// Multiply Input Matrix by Weights and store the Values in the NeuronLayer
				_Neurons[i] = (_Weights[i - 1] * _Neurons[i - 1]) + _Bias[i - 1];
				_Neurons[i] = _Neurons[i].unaryExpr(&NeuralNetwork::RELU);
			}
		}

		// Finds the Largest Coefficient Of the Output Layer and returns its index Position
		Eigen::MatrixXd::Index Max_Coeff_Row, MaxCoeff_Col;
		_Neurons.back().maxCoeff(&Max_Coeff_Row, &MaxCoeff_Col);

		return Max_Coeff_Row;
	}

	// Activation Functions
	double NeuralNetwork::Sigmoid(const double& x) {
		return 1 / (1 + std::exp(-x));
	}

	double NeuralNetwork::RELU(const double& x) {
		return std::max(0.0, x);
	}
};
