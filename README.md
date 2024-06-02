# mc-gl

This project aims to recreate a Minecraft-like terrain generation.
I tried to create as much as i could from scratch in order to learn the basics of OpenGL, with the help of the libraries GLFW, GLM and GLEW.

This is what the process is made of:
- Terrain: the terrain is composed of chunks, which are generated around the player according to his position in space. According to its position in space a position in the terrain's chunk grid is calculated, and around it are created the chunks spanning across an area of (CHUNK_RADIUS + 1)^2

- Chunk: a single chunk is a collection of Blocks, every chunk allocates in the memory its own blocks, giving them a local position and a world position based on its own. From every chunk is created a Mesh, which is the actual collection of vertices that will be sent to the shaders to be drawn on the screen.
Every column of blocks in a chunk is {CHUNK_HEIGHT - air_blocks} blocks tall, with air_blocks being calculated by multiplying a constant for a value of noise 
The blocks are store in the m_Blocks array, they are stored row by row going from z = 0 to z = CHUNK_SIZE on the x axis, going also from 0 to CHUNK_SIZE. This going from y = 0 to y = CHUNK_HEIGHT

- Block: a Block is the smallest component of the terrain. Each block has a different ID according to its local y coordinate in the chunk. The ID determines which texture will be applied to the block's faces and if the block is to be considered solid or not when the mesh of its chunk is being created.
Block IDs are *air*, *dirt*, *grass*, *cobblestone*.

- Mesh: the mesh is the collection of vertices of a chunk that will eventually be drawn to the screen. It is created by evaluating if the blocks' faces would be visible by the player, if not there's no need to draw them on the screen.
The mesh vertices will then be used as the buffer in the function glBufferData or glBufferSubData

- Shader and Buffer: these are the classes that wrap around the opengl functions. They are responsible of creating a shader program, compiling it and deleting it, allocating the gl buffers and update them with data from the meshes

## Mesh creation process:
- every chunk has a m_LowestSolidHeight field which holds the lowest y coordinate value of the block whose ID is not *air*. The m_Blocks array is iterated through starting from the lowest solid block's index