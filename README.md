
# Snake Neuro Evolution  
**If Cloning Repository: git --recursive-submodules PATH_TO_GIT_REPO**

## How To Begin Training
```c++
#include "../Headers/Wrapper.hpp"

int main(int argc, char* argv[])
{
	std::unique_ptr<Render::Wrapper> wrapper = std::make_unique<Render::Wrapper>();
	wrapper->Simulate();

	std::cin.get();
	return 0;
}
```
[Watch the full video](https://www.youtube.com/watch?v=uu1r35r0DSs&t=240s)

https://user-images.githubusercontent.com/44350860/188291700-c7dcc083-3082-4156-9d06-eeaed0b7dd53.mp4

## Structure:  
<pre>

Wrapper(GUI-Interface)  
	     |  
	     |  
	Engine Class  
		|		Genetic Algorithm (Base Class) - |  
		|				   |             | (Population class Derives from Genetic Algorithm class)  
		|----------------- Population (Derived Class) ---|  
					   	   |					
			   		     Entity Class  (Instantaites Neural Network class inside Entity Class)  
					     	   |  
					  NeuralNetwork Class  
</pre>

# What This Program Does
Implements a Neural Network, coupled with a genetic algorithm to evolve a population of Snakes that learn to beat the game overtime.  

# How It Works
A population of Snakes, each with their own Neural Network, are tasked to figure out how to beat the game. 
Each snake has a specific set of genes, which at first, are random floats between -1 and 1.  
After all the snakes have played the game and died, the top performing snakes are allowed to reproduce and pass their genes onto the next generation.  
This is known as Natural Selection and is how the snakes learn to complete the game. 

# Things that need to be Done/Fixed  
- When snake weights are saved, we pause at a cin.get()  
- Everything happens within Simulate, make it easier for the user to access the settings to simulate training  
- when displaying all snakes while training, maybe only display the top 10 or 15, so the user can actually see whats happening  
- give the user the ability to select between 6x6, 8x8, 10x10, 12x12, 14x14 Grids, and load the correct Trained snake, when load_snake is selected  
- when snakes are training, display "Training Snakes Population" with a display showing the progress. This could be a Percentage of the (total_pop/total_dead)  
- Display a small thin box around the playing grid?

![NeuroEvolutionEngine (1)](https://user-images.githubusercontent.com/44350860/199628336-91f8ccf1-e268-49e4-b14d-692f4bab02e5.jpg)
![NeuroEvolutionEngine (2)](https://user-images.githubusercontent.com/44350860/199628339-f833780e-e780-4244-9383-80b7e71ad409.jpg)
