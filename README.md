<pre>
Engine Class
	 |		Genetic Algorithm (Base Class)
	 |				   |
	 |-----------------Population (Derived Class)
							|					
					Entity Class <----- NeuralNetwork Class
					1. Population Class Stores a vector of Entity
					2. Each Entity contains its on Neural Network
</pre>

**If Cloning Repository: git --recursive-submodules PATH_TO_GIT_REPO**

The User Must Customize the Entity Class to fit the problem.

1. Input Vector Must be Created and Must fit the Neural Network Structure.
	ie. The vector must be the same size as the first layer
2. Fitness Function must be change to fit your problem
3. Entity Class must be customized to fir your problem

Things To Note:

1. Fitness Score is attached to the Neural Network class and 'Elitism Selection' uses that score to sort the population.
2. 'Calculate Fitness Function' can be changed and tuned too fit the specific problem.
