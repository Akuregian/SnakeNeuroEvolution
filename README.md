
**If Cloning Repository: git --recursive-submodules PATH_TO_GIT_REPO**

[Watch the full video](https://www.youtube.com/watch?v=uu1r35r0DSs&t=240s)



https://user-images.githubusercontent.com/44350860/188291700-c7dcc083-3082-4156-9d06-eeaed0b7dd53.mp4



# Snake Neuro Evolution  
## Structure:  
<pre>

Wrapper(GUI-Interface)  
	     |  
	     |  
	Engine Class  
		|		Genetic Algorithm (Base Class) 
		|				   |
		|----------------- Population (Derived Class)
					   	   |					
			   		     Entity Class 
					     	   |
					  NeuralNetwork Class
</pre>


This program implments a Neural Network, coupled with a genetic algorithm to evolve Snake Entities to learn and beat the game.
A Neural Network class is created and placed inside a Snake. A Population of these snakes are instantiated and passed to the Genetic Algorithm Class 
which trains the snakes until they have all died. At which point the results are sent to the Engine, which is picked up by the wrapper class and 
displays the results.


