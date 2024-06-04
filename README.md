# mncrft

This project aims to recreate a Minecraft-like terrain generation.
I tried to create as much as i could from scratch in order to learn the basics of OpenGL, with the help of the libraries GLFW, GLM and GLEW.

This is are the main classes in the application:
- Terrain: the terrain is composed of chunks, which are generated around the player according to his position in space. According to its position in space a position in the terrain's chunk grid is calculated, and around it are created the chunks spanning across an area of (CHUNK_RADIUS + 1)^2

- Chunk: a single chunk is a collection of Blocks, every chunk allocates in the memory its own blocks, giving them a local position and a world position based on its own. From every chunk is created a Mesh, which is the actual collection of vertices that will be sent to the shaders to be drawn on the screen.
Every column of blocks in a chunk is {CHUNK_HEIGHT - air_blocks} blocks tall, with air_blocks being calculated by multiplying a constant for a value of noise 
The blocks are store in the m_Blocks array, they are stored row by row going from z = 0 to z = CHUNK_SIZE on the x axis, going also from 0 to CHUNK_SIZE. This going from y = 0 to y = CHUNK_HEIGHT

- Block: a Block is the smallest component of the terrain. Each block has a different ID according to its local y coordinate in the chunk. The ID determines which texture will be applied to the block's faces and if the block is to be considered solid or not when the mesh of its chunk is being created.
Block IDs are *air*, *dirt*, *grass*, *cobblestone*.

- Mesh: the mesh is the collection of vertices of a chunk that will eventually be drawn to the screen. It is created by evaluating if the blocks' faces would be visible by the player, if not there's no need to draw them on the screen.
The mesh vertices will then be used as the buffer in the function glBufferData or glBufferSubData

- Shader and Buffer: these are the classes that wrap around the opengl functions. They are responsible of creating a shader program, compiling it and deleting it, allocating the gl buffers and update them with data from the meshes

## Terrain generation process:
- In the Terrain class constructor every Chunk class instance is allocated and stored row-by-row along the z axis in a bidimensional array.
- The chunks grid consists of a number of CHUNK_RADIUS chunks created along the positive and negative z and x axis from the player
- then for every chunk the function GenerateMeshes is called: the function checks if the current chunk being processed is in front of the camera to determine if the mesh should be generated or not
- after the call of GenerateMeshes the value of m_CurrentChunk is incremented to process, in the next frame, the next chunk in the grid
- when the player crosses the edge of the chunk it's in, the terrain around it is regenerated: chunks that are distant more then CHUNK_RADIUS from its position in the grid are deleted and new ones are generated in the direction it's moving
- when this occurs, m_CurrentChunk is reset to 0 because all the meshes should be generated again according to the new chunks' disposition that affects some of the chunks' surroundings

## Mesh generation process:
- every chunk has a m_LowestSolidHeight field that holds the lowest y coordinate value of the block whose ID is not *air*. The m_Blocks array is iterated through starting from the lowest solid block's index
- the condition checked first is if the block's world position lies inside the camera's frustum. 
- for every face of the blocks is checked if there is a neighbouring block which is solid. If it is not, then the vertices of the face should be added to the mesh because they will be visible
- if a face is at the edge of a chunk, then the program checks if the neighbouring chunk exists and if it has a solid block adjacent to the face
- By doing this only the vertices that make up the external profile of the chunk will be rendered
- three vectors store the data that's needed to correctly render a block face: m_Faces,m_TexCoords and m_ModelMats

*m_Faces*: each face of a block has its array of data in the header file Renderer.h. For every face added to the mesh one of the values of the enum variable "sides" is pushed back to the m_Faces vector. So when the process of mesh generation is done m_Faces is a collection of indexes that determine which of the six arrays that represent a face will be used in the shader.

*m_TexCoords*: for every vertex added to the mesh the corresponding 2D textures coordinates are pushed back to the m_TexCoords vector.
The texture coordinates referer to the texture atlas image and depend on the block's ID

*m_ModelMats*: for every face added to the mesh, a mat4 4x4 matrix is pushed back to the m_ModelMats vector. These are the matrices that will compute the correct 3D position of a vertex.
When a face is deemed to be part of the chunk's mesh a model matrix is calculated from its block's world position and in the vertex shader every position of the vertices of that face will be trasnformed by the matrix

## OpenGL side:
- In the Terrain class constructor are generated, through the instances of the Buffer class, the three main buffer objects that hold the data for all the aforementioned Mesh class' fields.
These buffers are given the size as if all the faces of a chunk where added to the mesh so that there wouldn't be any buffer overflow.
- The Chunk class has a m_OffsetIntoBuffer field which holds the value of the starting index from which the relative mesh data will be written
- Data will be sent to these buffer objects every time a mesh is generated through the glBufferSubData function with the chunk's m_OffsetIntoBuffer field value as the offset parameter

## Shaders
Vertex Shader:
- takes in six different vec3 attributes from pos_top to pos_back. Each of them represent the position of one of the vertices in the relative array.
- six different vec2 attributes. Each of them represent the texture coordinates for the specific vertex being processed
- four vec4 attributes. Each of them represent a column of the model matrix that will transform the vertex. This is an instanced array whose value changes every six vertices, which is the number of vertices in a face.
- an int attribute called "side": this is also an instaced attribute that changes every six vertices. It holds the current value from the buffer object whose data store comes from the Mesh's class m_Faces field. So it determines of which of of the block's faces the current vertex being processed is. According to that, one of the vec3 positions and vec2 texture coordinates is used
- two mat4 uniform variables: the view and projection matrices calculated in the Camera class whose values depened on the camera position and orientation in 3D space

## Some examples of how the development progressed:
(./images/chunk_no_shell.png)

(./images/chunk_shell.png)