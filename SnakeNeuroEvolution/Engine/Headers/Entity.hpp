#pragma once
#include "NeuralNetwork.hpp"

// This class can be tailored too your specific problem..
// **Need to change the Fitness function in 'Genetic Algorithm class'

namespace NeuroEvolution {

	typedef std::pair<int, int> Point;

	class Entity {
	public:
		Entity(const std::vector<unsigned int>& topology);
		Entity(std::vector<MAT_D>& w1, std::vector<VEC_D>& b1, unsigned int seed);
		~Entity();

		// Initialize the Snake
		void InitializeSnake(const std::vector<MAT_D>& w1, const std::vector<VEC_D>& b1);

		// Vision
		const VEC_D LookIn8Directions();

		// Head Direction (1 Hot Encoded)
		const VEC_D CurrentDirection();

		//Tail Direction (1 Hot Encoded)
		const VEC_D CurrentTailDirection();

		// Generate Input: LookIn8Directions-> CurrentDirection-> CurrentTailDirection
		void GenerateInputVector();

		// Move: Pop Tail to create the illusion of Moving Around
		bool Move(Point& nextMove);

		// Generate Random Food Location
		void GenerateFood(Point& point);

		// Update: Increment Steps, Look(), FeedForward i.e Think(), and Select the Ouputs Direction
		void Update();
		bool isBodyColliding(Point& new_head_position);
		bool isOutsideBoundaries(Point& new_head_position);
		bool isValidMove(Point& new_head_position);
		bool isAppleLocation(Point& new_head_position);

		// Input Vector
		VEC_D input_vector;

		std::shared_ptr<NeuroEvolution::NeuralNetwork> _Brain;

		bool isAlive;
		int lifeSpan; // How Long Snake can live in the population, After Each Generation
		int score; // The Snakes Score For Each game
		int steps; // How many Steps the snake took
		int steps_since_last_apple; // How Many steps the snake can take without eating
		int curr_dir;
		int colorlist[3];
		int brightness;

		// Snake Body
		std::deque<Point> Segments;

		// For Seeding
		uint32_t seed_value = NULL;

		// TargetFood
		Point TargetFood;

	protected:

		// Snake Moves
		const Point UP = std::make_pair(0, -1);
		const Point DOWN = std::make_pair(0, 1);
		const Point LEFT = std::make_pair(-1, 0);
		const Point RIGHT = std::make_pair(1, 0);
		const Point DETERMINE = std::make_pair(0, 0); // Determines the Next Move And Chooses Direction
		const std::vector<Point> MOVES = { UP, DOWN, LEFT, RIGHT, DETERMINE };

		// Next Move
		Point nextMove;

		// Vision
		const Point Vision[8] =
		{
		  {-1,  0 },  // Left
		  {-1, -1 },  // Top Left Diagonal
		  { 0, -1 },  // UP
		  { 1, -1 },  // Top Right Diagonal
		  { 1,  0 },  // Right
		  { 1,  1 },  // Bottom Right Diagonal
		  { 0,  1 },  // Bottom
		  { -1, 1 }   // Bottom Left Diagonal
		};
	};
}
