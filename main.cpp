/* TODO:
    * mesh creation has to be faster: allocating buffers one time and using glBufferSubData doens't increase performance significantly
    * make this faster on the application side
    * looping through every block for every chunk to create each mesh is really slow

    * holes in the terrain are caused by not assigning the meshes when Chunk operator= is called
    * the terrain offsets for the meshes are evaluated in the constructor that is called when a new chunk is allocated
    * but then when the chunks are rearranged the meshes are not rearranged as well
    * so the meshes for each chunk are still the ones of the original chunks allocated in the terrain constructor
    * that's because when rearranging the  m_Chunks vector a chunk at a certain index is assigned another chunk
    * so the former's fields are replaced by the latter's
    * except for the mesh, so that stays the one of the chunk that was at that particular index before the assignment
    * TO RESOLVE this calculate the offsets in the build function, which is called after the rearrangment and retrieves the chunk's OffsetIntoBuffer

    1. mesh creation: application side
        - searched for the first air block in a chunk, not significant performance implementation
        - OTHER possible SOLUTION: don't allocate a block pointer for every block in the chunk's constructor, it fills up the heap memory
        - instead 

        opengl side:
        - allocate one big buffer of a fixed side, how big should the buffers be to not worry about overflowing them with their new data?
        - then every time a mesh is recreated use glBufferSubData with the proper offset in the big buffer for that particular chunk

    2. has to be faster:
        - only render chunks whitin the camera's frustum (affects mesh creation)
        - keep in memory the chunks that have been loaded but are no longer rendered

    3. use a ray cast to break and add blocks (regenerate the mesh of the chunk that's being modified)
    4. add ligthing to the scene, initially use the jdh method, give east and west faces of a block different lighting simulating light coming from a certain direction
*/

/* TODO:
    * learn about the random seed for world generation
    * RANDOM WALK technique for generating mountains and caves?
*/

/* DONE:
    * modified GetModelMat function for returning the correct model based on the face transform
    * separated the model view and projection matrices in different vectors
    * fixed the problem for which the view and projection matrices where only updated when crossing chunks
    * the vertices' winding order depends on how, so changing the position of the vertices in the shader won't do anything
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

    float start = glfwGetTime(); // time in milliseconds
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
        {
            start = currentFrame;
            chunkNum++;
            if (chunkNum == terrain->m_Chunks.size())
                chunkNum = 0;   
        }

        player->GetCam()->UpdateTime(deltaTime);

        // process input and genrate meshes accordingly every frame
        //but only render every 1 / 60 seconds, 60 fps
        window->CheckKeyInput();

        terrain->GenerateWorld(player, chunkNum);

        renderer->Draw(terrain, player, chunkNum);

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