/* TODO:
    1. terrain meshes creation:
        - every mesh is iterated through and frustum culling is done as part of the process in Mesh::Build()
        - instead evaluate before which chunks are in the frustum space, save theìr indexes, and generate only the meshes of these ones
        - there's no need to generate the meshes for every chunk in the terrain

    3. use a ray cast to break and add blocks (regenerate the mesh of the chunk that's being modified)
    4. add ligthing to the scene, initially use the jdh method, give east and west faces of a block different lighting simulating light coming from a certain direction
*/

/* TODO:
    * learn about the random seed for world generation
    * RANDOM WALK technique for generating mountains and caves?
*/

/* DONE:
    * locked the game to 60fps and generate one mesh per frame 
*/

#include "Renderer.h"
#include "TextureAtlas/TextureAtlas.h"
#include "Window/Window.h"
#include <iostream>

float currentFrame = 0.0f;
float lastFrame = 0.0f;
float deltaTime = 0.0f;
int frames = 0;

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    Player* player = new Player();
    Input inputHandler = Input(player);
    Window* window = new Window(&inputHandler);

    GLFWwindow* GLFWwin = window->GetGLFWWindow();

    if (glewInit() != GLEW_OK)
    {
        std::cout << "error initializing GLEW" << std::endl;
        return -1;
    }

    Renderer* renderer = new Renderer();
    TextureAtlas* atlas = new TextureAtlas();
    Terrain* terrain = new Terrain(player);

    float start = glfwGetTime(); // time in seconds
    float timeFor60fps = 1.0  / 60.0;
    int chunkNum = -1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(GLFWwin))
    {
        /* Render here */

        glClearColor((float)135 / 255, (float)206 / 255, (float)250 / 255, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - start;

        printf("delta time: %f\n", deltaTime);

        if (deltaTime < timeFor60fps)
            continue;
        else
            start = currentFrame;

        player->GetCam()->UpdateTime(deltaTime);

        // process input and genrate meshes accordingly every frame
        //but only render every 1 / 60 seconds, 60 fps
        window->CheckKeyInput();

        terrain->GenerateWorld(player);

        renderer->Draw(terrain);

        //sets the player's last chunk grid position to its current position
        player->SetLastChunkGridPosition();

        /* Swap front and back buffers */
        glfwSwapBuffers(GLFWwin);

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete terrain;
    delete renderer; // also deletes the shader in its destructor
    delete player;
    delete atlas;
    delete window;

    glfwTerminate();
}