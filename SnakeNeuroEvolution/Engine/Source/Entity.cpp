#include "../Headers/Entity.hpp"

namespace NeuroEvolution {

	Entity::Entity(const std::vector<unsigned int>& topology)
		: _Brain(std::make_shared<NeuroEvolution::NeuralNetwork>(topology))
	{
		ENGINE_INIT_WARN("Entity Created");
	};

	Entity::Entity(std::vector<MAT_D>& w1, std::vector<VEC_D>& b1)
		: _Brain(std::make_shared<NeuroEvolution::NeuralNetwork>(w1, b1))
	{
		ENGINE_INIT_WARN("Entity created from Two parents");
	};

	Entity::~Entity()
	{
		ENGINE_INIT_WARN("Entity Destroyed");
	};

	void Entity::UpdateVisionVector()
	{
		NeuroEvolution::VEC_D vision_vector(3, 1);
		vision_vector << 2, 3, 1;
		input_vector = vision_vector;
	}

};
