#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// gl_Position is a pre-defined output
const char* vertexShaderSource = "#version 330 core                                   \n"
                                 "layout (location = 0) in vec3 aPos;                 \n"
                                 "void main()                                         \n"
                                 "{                                                   \n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
                                 "}                                                   \0";

// The fragment shader only needs one output: the final color of the fragment
const char* fragmentShaderSource = "#version 330 core                            \n"
                                   "out vec4 FragColor;                          \n"
                                   "void main()                                  \n"
                                   "{                                            \n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
                                   "}                                            \0";

int main()
{
   // Initialize GLFW before calling any GLFW functions
   // ****************************************************************************************************
   glfwInit();
   // Tell GLFW we want to use OpenGL version 3.3
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   // Tell GLFW we want to use the OpenGL Core Profile
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

   // Create a window
   // ****************************************************************************************************
   GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
   if (window == NULL)
   {
      std::cout << "Failed to create GLFW window" << std::endl;
      // Destroy all remaining windows, free any allocated resources and set GLFW to an uninitialized state
      glfwTerminate();
      return -1;
   }

   // Make the context of the specified window current on the calling thread
   // A context can only be made current on a single thread at a time, and each thread can only have a single current context at a time
   glfwMakeContextCurrent(window);

   // Specify the function that is called when the window is resized (the resize callback)
   // When the window is first displayed, this function is also called
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   // Initialize GLAD before calling any OpenGL functions
   // ****************************************************************************************************
   // GLAD manages function pointers for OpenGL
   // We pass it the function used to load the addresses of the OpenGL function pointers, which is OS-specific
   // glfwGetProcAddress defines the correct function based on the OS we are compiling for
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   // Vertex shader
   // ****************************************************************************************************
   int vertexShader = glCreateShader(GL_VERTEX_SHADER);        // glCreateShader returns 0 if an error occurs during the creation the shader object
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Replace the source code of a given shader
   glCompileShader(vertexShader);

   int success;
   char infoLog[512];

   // glGetShaderiv allows us to query a shader for information (GL_SHADER_TYPE, GL_DELETE_STATUS, GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH, GL_SHADER_SOURCE_LENGTH)
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      // glGetShaderInfoLog allows us to print compilation errors
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
   }

   // Fragment shader
   // ****************************************************************************************************
   int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
   }

   // Shader program
   // ****************************************************************************************************
   int shaderProgram = glCreateProgram();         // A shader program is the result of linking multiple shaders together. glCreateProgram returns 0 if an error occurs during the creation the shader program
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);                  // When two shaders are linked, the outputs of the first one are connected with the inputs of the second one

   // glGetProgramiv allows us to query a program for information
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if (!success)
   {
      // glGetProgramInfoLog allows us to print link errors
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
   }

   // Shaders can be deleted, since they have already been attached and linked to the shader program
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   // Initialize the relevant buffers with the data that we wish to render
   // ****************************************************************************************************
   float vertices[] =       { 0.5f,  0.5f, 0.0f,   // Top right
                              0.5f, -0.5f, 0.0f,   // Bottom right
                             -0.5f, -0.5f, 0.0f,   // Bottom left
                             -0.5f,  0.5f, 0.0f }; // Top left

   unsigned int indices[] = { 0, 1, 3,             // First triangle
                              1, 2, 3 };           // Second triangle

   unsigned int VAO, VBO, EBO;

   // Create a VAO, a VBO and an EBO
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   // Bind the VAO
   // From this point onward, the VAO will store the vertex attribute configuration
   glBindVertexArray(VAO);

   // Bind the VBO and store the vertex data inside of it
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Bind the EBO and store the indices inside of it
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Connect the vertex data that is inside VBO with attribute 0 of the vertex shader
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   // The call to glVertexAttribPointer registered VBO as the vertex attribute's bound VBO, so we can safely unbind it now.
   // We do not unbind the EBO while the VAO is bound, since the VAO stores any changes to the GL_ELEMENT_ARRAY_BUFFER target.
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   // We can also unbind VAO so that other VAO calls won't accidentally modify it
   glBindVertexArray(0); 

   // Uncomment this call to draw in wireframe polygons.
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   // The opposite is:
   //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // Render loop
   // ****************************************************************************************************
   while(!glfwWindowShouldClose(window))
   {
      // Process keyboard input
      processInput(window);

      // Set the color that OpenGL uses to reset the colorbuffer
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      // Clear the entire buffer of the current framebuffer
      // Some of the available options are:
      // GL_COLOR_BUFFER_BIT   (color buffer)
      // GL_DEPTH_BUFFER_BIT   (depth buffer)
      // GL_STENCIL_BUFFER_BIT (stencil buffer)
      glClear(GL_COLOR_BUFFER_BIT);

      // glUseProgram specifies the shader program that is to be used in all subsequent drawing commands
      glUseProgram(shaderProgram);

      // There is no need to bind the VAO in every iteration of the render loop, since there is only 1, but we will do it anyway in this example
      glBindVertexArray(VAO);

      // Draw
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      // There is no need to unbind the VAO in every iteration of the render loop in this example
      // glBindVertexArray(0);

      // Swap the front/back buffers
      // The front buffe r contains the final image that is displayed on the screen,
      // while all the rendering commands draw to the back buffer
      // When all the rendering commands are finished, the two buffers are swapped
      glfwSwapBuffers(window);

      // Process the events that have been received
      // This will cause the window and input callbacks associated with those events to be called
      glfwPollEvents();
   }

   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);

   // Destroy all remaining windows, free any allocated resources and set GLFW to an uninitialized state
   glfwTerminate();

   return 0;
}

// When the window is resized, the rendering window must also be resized
// For this reason, we call glViewport in the resize callback with the new dimensions of the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   // glViewPort specifies the size of the rendering window
   // The first two parameters (x, y) set the location of the lower left corner of the rendering window
   // The third and fourth parameters set the width and the height of the rendering window
   // The specified coordinates tell OpenGL how it should map its Normalized Device Coordinates (NDC),
   // which range from -1 to 1, to window coordinates (whose range is defined here)
   // We could make the rendering window smaller than GLFW's window, and use the extra space to display a menu
   // In this case, glViewport maps 2D coordinates as illustrated below:
   // Horizontally: (-1, 1) -> (0, 800)
   // Vertically:   (-1, 1) -> (0, 600)
   glViewport(0, 0, width, height);
}

// This function is used to exit the rendering the loop when the escape key is pressed
// It is called in every iteration of the render loop
void processInput(GLFWwindow* window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, true);
   }
}

// Additional information:
// The process of transforming 3D coordinates to 2D pixels is managed by OpenGL's graphics pipeline
// The graphics pipeline can be divided into two large parts:
// 1) 3D coordinates -> 2D coordinates
// 2) 2D coordinates -> Colored pixels
// A 2D coordinate is a precise representation of where a point is in 2D space
// A 2D pixel is an approximation of a 2D point that is limited by the resolution of the window
// The steps that compose the graphics pipeline can be executed in parallel by running small programs on the GPU
// These small programs are called shaders
// Stages of the graphics pipeline:
// Vertex Shader -> Shape Assembly -> Geometry Shader -> Rasterization -> Fragment Shader -> Tests and Blending

// A vertex is a collection of data for a 3D coordinate (e.g. position, color, normal...)
// A vertex's data is represented using vertex attributes

// In order for OpenGL to know what to make of a set of vertices, you must hint what kind of render type you want to form with the data
// Do we want the data to be rendered as a collection of points, a collection of triangles or a line?
// The hints that tell OpenGL what to make of a set of vertices are called primitives.
// Examples: GL_POINTS, GL_TRIANGLES and GL_LINE_STRIP

// A fragment is all the data required by OpenGL to render a single pixel.

// Steps:
// 1) Vertex Shader: Takes as input a single vertex and transforms its position (3D -> 3D). Also allows us to do some basic processing on the vertex's attributes.
// 2) Shape Assembly: Takes as input all the vertices that form a primitive and assembles them in the primitive shape we specified.
// 3) Geometry Shader: Takes as input a collection of vertices that form a primitive and has the ability to generate other shapes by emitting new vertices to form other primitives.
// 4) Rasterization: Takes as input a primitive and maps it to its corresponding pixels on the screen, resulting in fragments for the fragment shader to use. Note that before the fragment shader runs, clipping is performed. Clipping discards all the fragments that are outside of view.
// 5) Fragment shader: Takes as input a fragment. Its main purpose is to calculate the final color of a pixel. This is where all the advanced OpenGL effects occur. The fragment shader usually contains data about the 3D scene that it can use to perform lighting calculations.
// 6) Alpha Test and Blending: Checks the depth and stencil values of a fragment to determine if it is in front or behind other objects. Additionally, it checks the alpha value and blends objects accordingly.

// We can play with the vertex, fragment and geometry shaders.

// OpenGL transforms the 3D coordinates that are in the range [-1.0, 1.0] on all 3 axes (x, y and z) to 2D pixels on the screen
// This range is called the Normalized Device Coordinates (NDC) range
// After vertex coordinates have been processed by the vertex shader, they should be in the NDC range
// Any coordinates outside of this range are clipped
// Normalized Deviced Coordinates are transformed to screen-space coordinates by the viewport transform (glViewport takes care of this)
// The resulting screen-space coordinates are transformed into fragments and given to the fragment shader

// So, how do we send vertex data to the vertex shader? Steps:
// 1) Create memory on the GPU where we can store the vertex data
// 2) Configure how OpenGL should interpret the memory
// 3) Specify how to send the data to the graphics card
// Let's break each step down:

// 1) Create memory on the GPU where we can store the vertex data
//       - We manage this memory through Vertex Buffer Objects (VBO). A VBO can store a large number of vertices in the GPU's memory.
//       - Using VBOs is advantageous because they allow us to send large batches of data to the graphics card all at once. Sending data from the CPU to the graphics card is slow, so we must try to send as much data as possible at once.

// float vertices[] = {...};           // Array containing the coordinates of 3 vertices
// unsigned int VBO;                   // Will contain unique ID (GLuint) corresponding to a buffer object
// glGenBuffers(1, &VBO);              // Generate one buffer object and store its unique ID in VBO
// glBindBuffer(GL_ARRAY_BUFFER, VBO); // To change the state of a buffer object, it must first be bound to a buffer binding target
                                       // Only a single buffer object can be bound to a given buffer binding target at a time, but different buffer objects can be bound to different buffer binding targets simultaneously
                                       // Once a buffer object has been bound, any calls made on its buffer binding target affect its state
                                       // Calling glBindBuffer with 0 unbinds the current buffer object
                                       // Common buffer binding targets are: GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER and GL_TEXTURE_BUFFER
// glBufferData(GL_ARRAY_BUFFER,       // Copy the vertex data into VBO's memory by calling glBufferData with the buffer binding target that VBO is bound to
//              sizeof(vertices),
//              vertices,
//              GL_STATIC_DRAW);       // This fourth parameter specifies how the graphics card should manage the given data. It can be set to 3 different values:
                                       // GL_STATIC_DRAW:  The data is not likely to change, or it will rarely change.
                                       // GL_DYNAMIC_DRAW: The data is likely to change often.
                                       // GL_STREAM_DRAW:  The data will change every time it is drawn.

// 2) Configure how OpenGL should interpret the memory
//       - How should OpenGL interpret the vertex data in memory?
//       - How should it connect the vertex data to the vertex shader's attributes?

// The vertex shader allows us to specify any input we want in the form of vertex attributes
// We have to tell OpenGL how to connect our input data with the vertex attributes
// In our case, the data in VBO is organized as follows:
//         |  Vertex1  |  Vertex2  |  Vertex3  |
//         | X   Y   Z | X   Y   Z | X   Y   Z |
//         |   |   |   |   |   |   |   |   |   |
// Byte:   0   4   8   12  16  20  24  28  32  36
// Offset: 0
// Pos:    ------------>
//          Stride: 12

// Each coordinate is a 32-bit (4 byte) floating point value
// Each position is composed of 3 coordinates (12 bytes)
// There is no space between each set of 3 values, that is, the values are tightly packed
// The first value is at the beginning of the buffer

// Knowing this, we can tell OpenGL how to interpret the vertex data for each attribute
// glVertexAttribPointer tells OpenGL how to connect the vertex data with a specific attribute when a drawing call is made.
// Each vertex attribute takes its data from the VBO that is bound to GL_ARRAY_BUFFER when the call to glVertexAttribPointer is made.

// glVertexAttribPointer(0,                 // Index:      Specifies which vertex attribute we want to configure.
                                            //             In the vertex shader we declared the following:
                                            //             layout (location = 0) in vec3 aPos;
                                            //             So index 0 corresponds to the position vertex attribute.
//                       3,                 // Size:       Specifies the number of elements that make up the vertex attribute.
                                            //             Since the position is a vec3, we select 3.
//                       GL_FLOAT,          // Type:       Specifies the type of the elements that make up the vertex attribute.
//                       GL_FALSE,          // Normalized: Specifies if the data should be normalized.
//                       3 * sizeof(float), // Stride:     Specifies the space between consecutive vertex attribute sets.
                                            //             Since the next set of position data is located exactly 3 times the size of a float away,
                                            //             we specify that value as the stride.
//                       (void*) 0);        // Offset:     Specifies the offset of where the position data begins in the buffer.

// glEnableVertexAttribArray(0);            // Enables a generic vertex attribute. A vertex attribute can be disabled by calling glDisableVertexAttribArray.

// Another example of the vertex data is connected with the vertex attributes:

   //                Position    Color             TexCoords
   //                <-------->  <-------------->  <-------->
// GLfloat data[] = {1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.5f,
//                   0.0f, 1.0f, 0.2f, 0.8f, 0.0f, 0.0f, 1.0f};
   // Position
// glEnableVertexAttribArray(0);
// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) 0);
//
   // Color
// glEnableVertexAttribArray(1);
// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) (2 * sizeof(GL_FLOAT)));
//
   // TexCoords
// glEnableVertexAttribArray(2);
// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) (5 * sizeof(GL_FLOAT)));

// So far we have the following:
// - We initialized the vertex data in a buffer using a vertex buffer object.
// - We set up vertex and fragment shaders and told OpenGL how to link the vertex data to the vertex attributes of the vertex shader.

// So drawing in OpenGL looks like this:

// A) Copy our vertices array into a buffer for OpenGL to use.
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
// B) Set the vertex attributes pointers.
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
//    glEnableVertexAttribArray(0);
//
// C) Use our shader program when we want to render an object.
//
//    glUseProgram(shaderProgram);
//
// D) Draw the object.
//
//    ...

// If you have many objects, you need to do this for each one:
// - Bind the appropriate buffer object
// - Configure all the vertex attributes

// This is a long process. Is there a way to make it faster? Yes! Use Vertex Array Objects (VAO).
// A VAO can be bound just like a VBO, and once it is bound, it stores any vertex attribute calls.
// So we no longer need to configure all of the vertex attribute pointers multiple times.
// Instead, when we want to draw an object, we can simply load the VAO that was bound when we made the vertex attributes calls for said object.
// VAOs make it easy to switch between different vertex data and attribute configurations.

// A VAO stores the following:
// - Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
// - Vertex attribute configurations (these configurations are established using glVertexAttribPointer).
// - VBOs that are associated with vertex attributes (these associations are established when glVertexAttribPointer is called).

// When a VAO has been bound, any subsequent VBO, EBO, glVertexAttribPointer and glEnableVertexAttribArray calls are stored inside of it.
// So a VAO stores a vertex attribute configuration and which VBO to use.

// Creating a VAO is similar to creating a VBO:
// unsigned int VAO;
// glGenVertexArrays(1, &VAO);

// Using a VAO looks like this:
// A) Bind the VAO
//
// glBindVertexArray(VAO);
//
// B) Copy the vertices array into a buffer for OpenGL to use.
//
// glBindBuffer(GL_ARRAY_BUFFER, VBO);
// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
// C) Set the vertex attribute pointers.
//
// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
// glEnableVertexAttribArray(0); 
//
// D) Inside the render loop, bind the VAO and draw the object.
//
// glUseProgram(shaderProgram);
// glBindVertexArray(VAO);
// ...

// 3) Specify how to send the data to the graphics card

// To draw, OpenGL provides glDrawArrays, which draws primitives using:
// - The currently active shader program.
// - The previously defined vertex attribute configuration.
// - The vertex data in the currently bound VBO, or the vertex data that can be accessed indirectly through the currently bound VAO

// glUseProgram(shaderProgram);
// glBindVertexArray(VAO);
// glDrawArrays(GL_TRIANGLES, // Mode:    Specifies the kind of primitive to render. The options are:
                              //          GL_POINT, GL_LINE, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLE, GL_TRIANGLE_STRIP and GL_TRIANGLE_FAN.
//              0,            // First:   Specifies the starting index in the enabled arrays.
//              3);           // Count:   Specifies the number of vertices to draw.
                              //          The vertices are obtained directly from the currently bound VBO,
                              //          or indirectly through the currently bound VAO.

// Element Buffer Objects (EBO)
// Take a look at how we specify this rectangle:

// float vertices[] = { // First triangle
//                      0.5f,  0.5f, 0.0f,   // Top right
//                      0.5f, -0.5f, 0.0f,   // Bottom right
//                     -0.5f,  0.5f, 0.0f,   // Top left 
//                      // Second triangle
//                      0.5f, -0.5f, 0.0f,   // Bottom right
//                     -0.5f, -0.5f, 0.0f,   // Bottom left
//                     -0.5f,  0.5f, 0.0f }; // Top left

// Bottom right and top left are specified twice. Is there a way to avoid this overhead? Yes! With EBOs.
// An EBO is a buffer, just like a VBO, that stores indices that OpenGL uses to decide what vertices to draw.
// This is called indexed drawing.

// Using EBOs we can draw our rectangle like this (now we only need 4 vertices!):
//
// float vertices[] = { 0.5f,  0.5f, 0.0f,   // Top right
//                      0.5f, -0.5f, 0.0f,   // Bottom right
//                     -0.5f, -0.5f, 0.0f,   // Bottom left
//                     -0.5f,  0.5f, 0.0f }; // Top left
//
// unsigned int indices[] = { 0, 1, 3,       // First triangle
//                            1, 2, 3 };     // Second triangle
//
// unsigned int EBO;
// glGenBuffers(1, &EBO);
//
   // We use GL_ELEMENT_ARRAY_BUFFER as the buffer binding target
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
   // We use glDrawElements instead of glDrawArrays to indicate that we want to render triangles from an index buffer
// glDrawElements(GL_TRIANGLES,    // Mode:    Specifies the mode we want to draw in.
//                6,               // Count:   Specifies the number of elements we want to draw.
//                GL_UNSIGNED_INT, // Type:    Specifies the type of the indices.
//                0);              // Indices: Specifies an offset in a buffer or a pointer to the location where the indices are stored (if an EBO is not used).

// glDrawElements takes its indices from the EBO that is currently bound to the GL_ELEMENT_ARRAY_BUFFER target.
// So we have to bind the corresponding EBO each time we want to render an object with indices.
// This is cumbersome. Is there a way to avoid it? Yes! Use VAOs.
// When a EBO is bound after a VAO has been bound, the VAO stores the EBO.
// In other words, a VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
// This also means it stores the calls that unbind an EBO from the target GL_ELEMENT_ARRAY_BUFFER.
// So make sure you don't unbind the EBO before unbinding the VAO.

// The initialization code now looks like this:

// Initialization:
//
// A) Bind the VAO
//
// glBindVertexArray(VAO);
//
// B) Copy the vertices array into a vertex buffer for OpenGL to use
//
// glBindBuffer(GL_ARRAY_BUFFER, VBO);
// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
// B) Copy the indices array into an element buffer for OpenGL to use
//
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
// D) Set the vertex attribute pointers
//
// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// glEnableVertexAttribArray(0);
//
// ...
//
// Render loop:
//
// glUseProgram(shaderProgram);
// glBindVertexArray(VAO);
// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)
// glBindVertexArray(0);

