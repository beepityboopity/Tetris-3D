This is an in-progress project to create a 3D Tetris game in C++ with the OpenGL library. It currently uses OpenGL 3.3 with GLFW for window creation, as well as GLM for mathematics and Imgui for ui. 

You can build the game by creating a project in an IDE (Visual Studio was used for development), setting resources/includes and resources/libs as includes and libraries respectively, and adding glfw3.lib and opengl32.lib as additional dependencies

To play the game after building, click "start" in the upper left-hand corner and then move the piece with WASD and rotate it with JKL. The goal is to fit the piece inside the black cube outline. Pieces will spawn in a random location and move towards the opposite side of the cube until they hit the "bottom".

As mentioned above, this game is a work in progess. Below is a to-do list of features and refactors that I have planned for the future.

- Add a cmake build script
- Refactor the game into a class
- Add a second game mode with non-random gravity
- Add an option for less wacky pieces
- Add a quick drop button
- Add increasing difficulty and a better scoring system
- Fix window resizing perspective issues
- Adjust the shader to something prettier
- Make the imgui panel immovable and add more options to it
- Add background music/sound effects
