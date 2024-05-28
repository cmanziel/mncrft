/* TODO:
    1. make world generation faster:
        I. don't allocate every chunk and then evaluate of which ones the mesh is going to be created
        evaluate which chunks are in front of the player only by using their position, then allocate the chunks whose positions are those ones
        of this chunks also generate the meshes
        Can't have the same SetChunkSurroundings as before though

        - having a model matrix for every face is memory expensive, maybe use a block's world position as a vertex attribute and initalize the model matrix for the vertex inside the shader

        - loop thorugh the meshes starting from the ones closer to the player so they're the first ones to be generated

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

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <iostream>
#include "Renderer.h"
#include "TextureAtlas/TextureAtlas.h"
#include "Window/Window.h"

float currentFrame = 0.0f;
float lastFrame = 0.0f;
float deltaTime = 0.0f;

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    Player* player = DBG_NEW Player();
    Input inputHandler = Input(player);
    Window* window = DBG_NEW Window(&inputHandler);

    GLFWwindow* GLFWwin = window->GetGLFWWindow();

    if (glewInit() != GLEW_OK)
    {
        std::cout << "error initializing GLEW" << std::endl;
        return -1;
    }

    Renderer* renderer = DBG_NEW Renderer();
    TextureAtlas* atlas = DBG_NEW TextureAtlas();
    Terrain* terrain = DBG_NEW Terrain(player);

    float start = glfwGetTime(); // time in seconds
    float timeForFrameRate = 1.0 / 60.0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(GLFWwin))
    {
        /* Render here */

        glClearColor((float)135 / 255, (float)206 / 255, (float)250 / 255, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - start;

        printf("frame rate: %f\n", 1.0 / (currentFrame - lastFrame));

        lastFrame = currentFrame;

        if (deltaTime < timeForFrameRate)
            continue;
        
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

    _CrtDumpMemoryLeaks();

    return 0;
}