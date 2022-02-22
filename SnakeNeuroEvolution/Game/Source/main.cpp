#include "../Headers/Wrapper.hpp"

int main(int argc, char* argv[])
{

	std::unique_ptr<Render::Wrapper> wrapper = std::make_unique<Render::Wrapper>();
	wrapper->Simulate();

	std::cin.get();
	return 0;
}