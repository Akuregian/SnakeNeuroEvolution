#include "../Headers/Entity.hpp"

namespace NeuroEvolution {

	// New Entity
	Entity::Entity(const std::vector<unsigned int>& topology)
		: _Brain(std::make_shared<NeuroEvolution::NeuralNetwork>(topology))
	{
		Entity::InitializeSnake({}, {});
	};

	// Entity Created from Previous Weights/Bias
	Entity::Entity(std::vector<MAT_D>& w1, std::vector<VEC_D>& b1, unsigned int seed)
		: _Brain(std::make_shared<NeuroEvolution::NeuralNetwork>(w1, b1, NeuralSettings::TOPOLOGY))
	{

		if (seed != NULL)
		{
			seed_value = seed;
			SET_SEED(seed_value);
		}
		else
		{
			NEW_SEED;
			seed_value = Seed::GetInstance()->curr_seed;
			SET_SEED(seed_value);
		}
		Entity::InitializeSnake(w1, b1);
	};

	Entity::~Entity()
	{
		_Brain.reset();
	};

	void Entity::InitializeSnake(const std::vector<MAT_D>& w1, const std::vector<VEC_D>& b1)
	{
		if (seed_value == NULL)
		{
			NEW_SEED;
			seed_value = Seed::GetInstance()->curr_seed;
			SET_SEED(seed_value);
		}
		else
		{
			SET_SEED(seed_value);
		}

		InitializeValues();
		GenerateFood(TargetFood);
	}

	void Entity::InitializeValues()
	{
		this-> isAlive = true;
		this->lifeSpan = 100; 
		this->score = 0;
		this->steps = 0; 
		this->steps_since_last_apple = 0; 
		this->curr_dir = 4;
		this->brightness = 255;

		for(unsigned int i = 0; i < 3; i++)
		{
			this->colorlist_1[i] = std::rand() % 255;
			this->colorlist_2[i] = std::rand() % 255;
		}

		this->isReplayEntity = false;
		int r = std::uniform_int_distribution<int>(0, GameSettings::BoardY)(Seed::GetInstance()->m_Generator);
		int c = std::uniform_int_distribution<int>(0, GameSettings::BoardX)(Seed::GetInstance()->m_Generator);
		Segments.push_back(std::make_pair(r, c));
	}

	const VEC_D Entity::LookIn8Directions()
	{
		VEC_D VisionInput(24, 1);
		std::vector<double> vision_values;
		Point current_Head = Segments.back();

		for (int i = 0; i <= 7; i++) {

			// Store the Each Vision Value
			std::vector<double> Apple_Self_Wall;

			// Get The Current Head Position
			Point visionLine = current_Head;

			// Set the Variables
			float Distance_To_Self = INFINITY;
			float Distance_To_Food = INFINITY;
			float Distance_To_Wall = 0;

			// increment
			float Distance = 1.0f;
			// store the total distance
			float totalDistance = 0.0f;

			// Skip the headPosition
			visionLine.first += Vision[i].first;
			visionLine.second += Vision[i].second;

			totalDistance += Distance;
			bool foodFound = false;
			bool bodyFound = false;

			while (visionLine.first < GameSettings::BoardX && visionLine.second < GameSettings::BoardY && visionLine.first >= 0 && visionLine.second >= 0) {

				if (!bodyFound && std::find(Segments.begin(), Segments.end() - 1, visionLine) != Segments.end() - 1) {
					Distance_To_Self = totalDistance;
					bodyFound = true;
				}
				if (!foodFound && visionLine == TargetFood) {
					Distance_To_Food = totalDistance;
					foodFound = true;
				}

				visionLine.first += Vision[i].first;
				visionLine.second += Vision[i].second;
				totalDistance += Distance;
			}

			Distance_To_Wall = 1.0 / totalDistance;
			Distance_To_Self = Distance_To_Self != INFINITY ? 1 : 0;
			Distance_To_Food = Distance_To_Food != INFINITY ? 1 : 0;

			Apple_Self_Wall.push_back(Distance_To_Food);
			Apple_Self_Wall.push_back(Distance_To_Self);
			Apple_Self_Wall.push_back(Distance_To_Wall);

			vision_values.insert(vision_values.end(), Apple_Self_Wall.begin(), Apple_Self_Wall.end());
		}

		VisionInput = VisionInput.Map(vision_values.data(), vision_values.size());

		//	std::cout << "Vision: \n\n" << VisionInput << std::endl;

		return VisionInput;
	}

	bool Entity::Move(Point& nextMove)
	{
		// If Snake is Dead, Return
		if (!isAlive) 
		{
			return false;
		}

		// Move Snake
		Point NewHeadPosition = std::make_pair(Segments.back().first + nextMove.first, Segments.back().second + nextMove.second);

		// Check if its a food location
		if (isAppleLocation(NewHeadPosition)) 
		{
			score++;
			steps_since_last_apple = 0;
			Segments.push_back(NewHeadPosition);
			if (!GenerateFood(TargetFood)) { return false; }
		}
		else 
		{
			Segments.push_back(NewHeadPosition);
			Segments.pop_front();
		}

		// check if its a valid move
		if (!isValidMove(NewHeadPosition)) 
		{
			return false;
		}

		steps_since_last_apple++;
		// Check If the Snake hasnt eating within the Alloted amount of steps
		if (steps_since_last_apple > GeneticSettings::MAX_STEPS_WITHOUT_EATING) 
		{
			isAlive = false;
		}
	}

	bool Entity::GenerateFood(Point& point)
	{
		SET_SEED(seed_value); // @@@WHY DO I HAVE TO RESET THE SEED?@@@
		std::deque<Point> possiblitlies;
		for (int i = 0; i < GameSettings::BoardY; i++) {
			for (int j = 0; j < GameSettings::BoardX; j++) {
				Point check = std::make_pair(j, i);
				if (std::find(Segments.begin(), Segments.end(), check) == std::end(Segments)) {
					possiblitlies.push_back(std::make_pair(j, i));
				}
			}
		}

		// TODO: Not the Most Elegent Solution... Can be Reworked
		if (possiblitlies.size() <= 0) {
			if (!isReplayEntity)
			{
				ENGINE_LOGGER("Saved Weights");
				SaveWeights(); 
				std::cin.get(); // NEED TO CHANGE THIS
				return false;

			}
			std::cin.get(); // NEED TO CHANGE THIS
			return false;
		}
		int random = std::uniform_int_distribution<int>(0, possiblitlies.size() - 1)(Seed::GetInstance()->m_Generator);
		Point randPoint = possiblitlies[random];
		Point FoodLocation = std::make_pair(randPoint.first, randPoint.second);
		point = FoodLocation;
		return true;
	}

	const VEC_D Entity::CurrentDirection()
	{
		VEC_D DirectionInput(4, 1);

		// UP
		if (curr_dir == 0) {
			DirectionInput << 1, 0, 0, 0;
		}
		// DOWN
		else if (curr_dir == 1) {
			DirectionInput << 0, 1, 0, 0;
		}
		// LEFT
		else if (curr_dir == 2) {
			DirectionInput << 0, 0, 1, 0;
		}
		// RIGHT
		else if (curr_dir == 3) {
			DirectionInput << 0, 0, 0, 1;
		}
		else if (curr_dir == 4) {
			DirectionInput << 0, 0, 0, 0;
		}

		return DirectionInput;
	}

	const VEC_D Entity::CurrentTailDirection()
	{
		VEC_D TailDirectionInput(4, 1);
		if (Segments.size() > 1) {
			Point T1 = Segments[0];
			Point T2 = Segments[1];
			Point Diff;
			Diff.first = T2.first - T1.first;
			Diff.second = T2.second - T1.second;

			// if(Difference.y < 0) --> UP
			if (Diff.second < 0) {
				TailDirectionInput << 1, 0, 0, 0;
			}
			//	if(Difference.y > 0) --> DOWN
			else if (Diff.second > 0) {
				TailDirectionInput << 0, 1, 0, 0;
			}
			// if(Difference.x < 0) --> LEFT
			else if (Diff.first < 0) {
				TailDirectionInput << 0, 0, 1, 0;
			}
			//if(Difference.x > 0) --> RIGHT
			else if (Diff.first > 0) {
				TailDirectionInput << 0, 0, 0, 1;
			}
		}
		else {
			TailDirectionInput = CurrentDirection();
		}

		return TailDirectionInput;
	}

	void Entity::GenerateInputVector()
	{
		VEC_D input(32, 1);
		input.block(0, 0, 24, 1) = LookIn8Directions();
		input.block(24, 0, 4, 1) = CurrentDirection();
		input.block(28, 0, 4, 1) = CurrentTailDirection();
		input_vector = input;
	}

	bool Entity::Update()
	{
		if (isAlive)
		{
			steps++;
			GenerateInputVector();
			curr_dir = _Brain->ForwardPropagate(input_vector);
			Point nextMove = MOVES[curr_dir];
			if (!Move(nextMove))
			{
				isAlive = false;
				return false;
			}
		}
		return true;
	}

	bool Entity::isBodyColliding(Point& new_head_position)
	{
		if (std::find(Segments.begin(), Segments.end() - 1, new_head_position) != Segments.end() - 1)
		{
			return true;
		}
		return false;
	}

	bool Entity::isOutsideBoundaries(Point& new_head_position)
	{
		if (new_head_position.first >= GameSettings::BoardX || new_head_position.second >= GameSettings::BoardY || new_head_position.first < 0 || new_head_position.second < 0) 
		{
			return true;
		}
		return false;
	}

	bool Entity::isValidMove(Point& new_head_position)
	{
		if (isOutsideBoundaries(new_head_position)) 
		{
			return false;
		}
		else if (isBodyColliding(new_head_position)) 
		{
			return false;
		}
		return true;
	}

	bool Entity::isAppleLocation(Point& new_head_position)
	{
		if (new_head_position.first == TargetFood.first && new_head_position.second == TargetFood.second) 
		{
			return true;
		}
		return false;
	}

	void Entity::SaveWeights()
	{
		const static Eigen::IOFormat CSVFormat(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", "\n");

		std::string grid_size = '(' + std::to_string(GameSettings::BoardX) + 'x' + std::to_string(GameSettings::BoardY) + ')';
		std::ofstream file("../../../../SnakeNeuroEvolution/TopEntityWeights/TopSnake" + grid_size + ".csv", std::ofstream::out | std::ofstream::trunc);

		// Is the File Open?
		if (file.is_open()) {
			// Write All Weights to file
			for (int i = 0; i < _Brain->_Weights.size(); i++) {
				file << _Brain->_Weights[i].format(CSVFormat) << "\n\n";
			}

			// Add "Bias" So we can find this section in the file
			file << "Bias\n";
			for (int i = 0; i < _Brain->_Bias.size(); i++) {
				// Add All Bias Values
				file << _Brain->_Bias[i] << "\n\n";
			}

			// Add "Seed" So we can find this section in the file
			file << "Seed\n";
			// Seed Value
			file << seed_value << "\n\n";

			// Save Color_1
			file << "Color1\n";
			for (unsigned int i = 0; i < 3; i++)
			{
				file << colorlist_1[i] << "\n";
			}

			// Save Color_2
			file << "\nColor2\n";
			for (unsigned int i = 0; i < 3; i++)
			{
				file << colorlist_2[i] << "\n";
			}
		}
		file.close();
	}
};
