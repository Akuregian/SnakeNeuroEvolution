#include "../Headers/Population.hpp"


namespace NeuroEvolution {

	Population::Population()
		: current_generation(0) {}

	Population::~Population() {}

	void Population::CreatePopulationOfEntites()
	{
		for (unsigned int i = 0; i < GeneticSettings::POP_SIZE; i++)
		{
			_EntityPopulation.push_back(std::make_shared<NeuroEvolution::Entity>(NeuralSettings::TOPOLOGY));
		}
	}

	bool Population::isAllEntitiesDead()
	{
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++)
		{
			if (_EntityPopulation[i]->isAlive)
			{
				return false;
			}
		}
		return true;
	}

	void Population::Update()
	{
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++)
		{
			_EntityPopulation[i]->Update();
		}
	}

	std::vector<std::shared_ptr<NeuroEvolution::Entity>>& Population::TrainEntitiesOnce()
	{
		Update();
		return _EntityPopulation;
	}

	void Population::TrainEntities()
	{
		while (!isAllEntitiesDead())
		{
			Update();
		}

		ENGINE_LOGGER("DoneTraining");

		Population::CreateNextGeneration();
	}

	void Population::CreateNextGeneration()
	{

		// Calculate Each Snakes Fitness Score and set it too Network Fitness
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++) 
		{
			double FitnessScore = GeneticAlgorithm::CalculateFitness(_EntityPopulation[i]->steps, _EntityPopulation[i]->score);
			_EntityPopulation[i]->_Brain->NetworkFitness = FitnessScore;
		}

		// Clear MatingPool
		_EntityMatingPool.clear();

		// Sort 'Elites' by fitness
		GeneticAlgorithm::ElitismSelection(_EntityPopulation);

		// Grabbing Roulette Wheel Sum
		double roulette_wheel_sum = 0;
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++)
		{
			roulette_wheel_sum += _EntityPopulation[i]->_Brain->NetworkFitness;
		}

		// Print Results and Set the top performing Snake
		Population::Results();

		// Store 'Elites' into MatingPool
		for (unsigned int i = 0; i < GeneticSettings::POP_SIZE; i++)
		{
			_EntityMatingPool.push_back(std::make_shared<Entity>(_EntityPopulation[i]->_Brain->_Weights, _EntityPopulation[i]->_Brain->_Bias, NULL));
			_EntityMatingPool[i]->_Brain->NetworkFitness = _EntityPopulation[i]->_Brain->NetworkFitness;
		}

		_EntityPopulation = _EntityMatingPool;

		while (_EntityPopulation.size() < GeneticSettings::MATING_POP_SIZE)
		{
			GeneticAlgorithm::CrossoverAndMutation(roulette_wheel_sum, _EntityMatingPool, _EntityPopulation);
		}

		for (int i = 0; i < GeneticSettings::POP_SIZE; i++)
		{
			_EntityPopulation[i]->_Brain->NetworkFitness = 0;
			_EntityPopulation[i]->score = 0;
		}


		current_generation++;
	}

	void Population::Results()
	{
		//--------------------- Print To Console ------------------------
		ENGINE_LOGGER("Generation: {0}", current_generation);
		ENGINE_LOGGER("--------------------------------------------------------");
		ENGINE_LOGGER("Score: {0}", _EntityPopulation.front()->score);
		ENGINE_LOGGER("Network Fitness: {0}", _EntityPopulation.front()->_Brain->NetworkFitness);
		ENGINE_LOGGER("--------------------------------------------------------\n\n");

		// Top Snake
 		ReplayEntity = std::make_shared<Entity>(_EntityPopulation.front()->_Brain->_Weights, _EntityPopulation.front()->_Brain->_Bias, _EntityPopulation.front()->seed_value);

	}

	void Population::LoadEntity()
	{
		// Matrix && Vector Varibales
		std::vector<Eigen::MatrixXd> Weights;
		std::vector<Eigen::VectorXd> Biases;
		std::string saved_seed;

		// in this object we store the data from the matrix
		std::ifstream weightsDataFile("../../../../SnakeNeuroEvolution/TopEntityWeights/TopSnake.csv");

		// File Data Variables
		std::vector<double> dataEntries;
		std::string dataRowString; // this variable is used to store the row of the matrix that contains commas
		std::string dataEntry; // this variable is used to store the matrix entry;
		int RowNumber = 0; // this variable is used to track the number of rows

		// read a row by row of matrixDataFile and store every line into the string variable matrixRowString
		while (getline(weightsDataFile, dataRowString)) {

			if (dataRowString.find("Bias") != std::string::npos) {
				break;
			}
			// stringstream to go through each string in the line obtained
			std::stringstream dataRowStringStream(dataRowString);

			// read pieces of the stream matrixRowStringStream until every comma, and store the resulting character into the matrixEntry. Stop when "\n" is reached
			while (std::getline(dataRowStringStream, dataEntry, ',')) {
				//convert the string to float and fill in the row vector, which stores all the matrix entries
				dataEntries.push_back(std::stof(dataEntry));
			}

			// Each Matrix Is Stored with a line seperating it from the other Matrix, If we hit that line: Create a Matrix
			if (dataRowString.empty()) {
				Eigen::MatrixXd read_matrix = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(dataEntries.data(), RowNumber, dataEntries.size() / RowNumber);
				Weights.push_back(read_matrix);
				dataEntries.clear();
				RowNumber = 0;
				continue;
			}

			RowNumber++; //update the column numbers
		}

		// read a row by row of matrixDataFile and store every line into the string variable matrixRowString
		while (getline(weightsDataFile, dataRowString)) {

			if (dataRowString.find("Seed") != std::string::npos) {
				break;
			}

			// stringstream to go through each string in the line obtained
			std::stringstream dataRowStringStream(dataRowString);

			// read pieces of the stream matrixRowStringStream until every comma, and store the resulting character into the matrixEntry. Stop when "\n" is reached
			while (std::getline(dataRowStringStream, dataEntry, ',')) {
				//convert the string to float and fill in the row vector, which stores all the matrix entries
				dataEntries.push_back(std::stof(dataEntry));
			}

			// Each Matrix Is Stored with a line seperating it from the other Matrix, If we hit that line: Create a Matrix
			if (dataRowString.empty()) {
				Eigen::VectorXd read_matrix = Eigen::Map<Eigen::VectorXd>(dataEntries.data(), dataEntries.size());
				Biases.push_back(read_matrix);
				dataEntries.clear();
				RowNumber = 0;
				continue;
			}
			RowNumber++; //update the column numbers
		}

		getline(weightsDataFile, dataRowString);
		saved_seed = dataRowString;

		uint32_t seedv = std::stoull(saved_seed);

		// Snake Variables
		ReplayEntity = std::make_shared<NeuroEvolution::Entity>(Weights, Biases, seedv);
		ReplayEntity->isReplayEntity = true;
	}
}