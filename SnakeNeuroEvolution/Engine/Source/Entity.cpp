#include "../Headers/Entity.hpp"

namespace NeuroEvolution {

	// New Entity
	Entity::Entity(const std::vector<unsigned int>& topology)
		: _Brain(std::make_shared<NeuroEvolution::NeuralNetwork>(topology))
	{
		ENGINE_INIT_WARN("Entity Created");
		Entity::InitializeSnake();
	};

	// Entity Created from Previous Weights/Bias
	Entity::Entity(std::vector<MAT_D>& w1, std::vector<VEC_D>& b1)
		: _Brain(std::make_shared<NeuroEvolution::NeuralNetwork>(w1, b1))
	{
		ENGINE_INIT_WARN("Entity created from Two parents");
	};

	Entity& Entity::operator=(const Entity& Other)
	{
		if (&Other != this)
		{
			score = Other.score;
			_Brain = Other._Brain;
			Segments = Other.Segments;
			seed_value = Other.seed_value;
		}

		return *this;
	}

	Entity::~Entity()
	{
		ENGINE_INIT_WARN("Entity Destroyed");
	};

	void Entity::InitializeSnake()
	{
		if (seed_value == NULL)
		{
			seed_value = Seed::GetInstance()->GetSeed();
		}
		else
		{
			SET_SEED(seed_value);
		}

		this->colorlist[0] = std::rand() % 255;
		this->colorlist[1] = std::rand() % 255;
		this->colorlist[2] = std::rand() % 255;

		int r = std::uniform_int_distribution<int>(0, GameSettings::BoardX)(Seed::GetInstance()->m_Generator);
		int c = std::uniform_int_distribution<int>(0, GameSettings::BoardY)(Seed::GetInstance()->m_Generator);

		Segments.push_back(std::make_pair(r, c));

		GenerateFood(TargetFood);
	}

	const VEC_D& Entity::LookIn8Directions()
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
			GenerateFood(TargetFood);
			// TODO: Not the Most Elegent Solution, Can be re-worked
			if (TargetFood.first == -1 && TargetFood.second == -1) 
			{
				return false;
			}
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

	void Entity::GenerateFood(Point& point)
	{
		std::deque<Point> possiblitlies;
		for (int i = 0; i < GameSettings::BoardY; i++) {
			for (int j = 0; j < GameSettings::BoardX; j++) {
				Point check = std::make_pair(i, j);
				if (std::find(Segments.begin(), Segments.end(), check) == std::end(Segments)) {
					possiblitlies.push_back(std::make_pair(i, j));
				}
			}
		}

		// TODO: Not the Most Elegent Solution... Can be Reworked
		if (possiblitlies.size() <= 0) {
			ENGINE_LOGGER_CRITICAL("SNAKE HAS COMPLETED OR WON, DO SOMETING ABOUT THIS");
			std::cin.get();
		}

		int random = std::uniform_int_distribution<int>(0, possiblitlies.size() - 1)(Seed::GetInstance()->m_Generator);
		Point randPoint = possiblitlies[random];
		Point FoodLocation = std::make_pair(randPoint.first, randPoint.second);

		//	std::cout << "FoodLocation: ( " << FoodLocation.first << ", " << FoodLocation.second << " )" << std::endl;
		point = FoodLocation;
	}

	const VEC_D& Entity::CurrentDirection()
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

	const VEC_D& Entity::CurrentTailDirection()
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

	void Entity::Update()
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
			}
		}
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
};
