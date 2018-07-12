#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// gl_Position is a pre-defined output
const char* vertexShaderSource = "#version 330 core                                             \n"
                                 "                                                              \n"
                                 "layout (location = 0) in vec3 aPos;                           \n"
                                 "layout (location = 1) in vec2 aTexCoord;                      \n"
                                 "                                                              \n"
                                 "out vec2 TexCoord;                                            \n"
                                 "                                                              \n"
                                 "uniform mat4 model;                                           \n"
                                 "uniform mat4 view;                                            \n"
                                 "uniform mat4 projection;                                      \n"
                                 "                                                              \n"
                                 "void main()                                                   \n"
                                 "{                                                             \n"
                                 "   gl_Position = projection * view * model * vec4(aPos, 1.0); \n"
                                 "   TexCoord    = aTexCoord;                                   \n"
                                 "}                                                             \0";

// The fragment shader only needs one output: the final color of the fragment
const char* fragmentShaderSource = "#version 330 core                               \n"
                                   "                                                \n"
                                   "in  vec2 TexCoord;                              \n"
                                   "                                                \n"
                                   "out vec4 FragColor;                             \n"
                                   "                                                \n"
                                   "uniform sampler2D texture1;                     \n"
                                   "uniform sampler2D texture2;                     \n"
                                   "                                                \n"
                                   "void main()                                     \n"
                                   "{                                               \n"
                                   "   FragColor = mix(texture(texture1, TexCoord), \n"
                                   "                   texture(texture2, TexCoord), \n"
                                   "                   0.2);                        \n"
                                   "}                                               \0";

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

   // Enable depth testing globally
   glEnable(GL_DEPTH_TEST);

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
   //                     Positions           Texture coords
   //                    <--------------->    <-------->
   float vertices[] = { -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f };

   unsigned int VAO, VBO;

   // Create a VAO and a VBO
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   // Bind the VAO
   // From this point onward, the VAO will store the vertex attribute configuration
   glBindVertexArray(VAO);

   // Bind the VBO and store the vertex data inside of it
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // Vertex attributes
   // ****************************************************************************************************
   // Connect the vertex positions that are inside VBO with attribute 0 of the vertex shader
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   // Connect the texture coordinates that are inside VBO with attribute 2 of the vertex shader
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(GL_FLOAT)));
   glEnableVertexAttribArray(1);

   // The call to glVertexAttribPointer registered VBO as the vertex attribute's bound VBO, so we can safely unbind it now.
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   // We can also unbind VAO so that other VAO calls won't accidentally modify it
   glBindVertexArray(0);

   // Texturing
   // ****************************************************************************************************
   unsigned int texture1, texture2;

   // Texture 1
   // ---------

   // Create a texture
   glGenTextures(1, &texture1);

   // Bind the texture object to the GL_TEXTURE_2D target
   // Any subsequent texture commands will configure the currently bound texture object
   glBindTexture(GL_TEXTURE_2D, texture1);

   // Set the texture wrapping parameters
   // GL_REPEAT is the default wrapping method
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // Set the texture filtering parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // OpenGL expects the 0.0 Y-axis coordinate to be on the bottom side of the image, but images usually
   // have the 0.0 Y-axis coordinate at the top of the image. Because of this, images are inverted in the
   // the vertical direction when we load them. We can tell the stb_image library to flip the Y-axis
   // during image loading by making the call below before loading any images
   stbi_set_flip_vertically_on_load(true);

   // Load the texture image
   int width, height, nrChannels;
   unsigned char *data = stbi_load("tex/container.jpg", &width, &height, &nrChannels, 0);

   if (data)
   {
      // Generate the texture and the mipmaps
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   }
   else
   {
      std::cout << "Failed to load texture" << std::endl;
   }

   // We can unbind the texture so that other texture calls won't accidentally modify it
   glBindTexture(GL_TEXTURE_2D, 0);

   // Since the texture has been generated, we can delete the texture image
   stbi_image_free(data);

   // Texture 2
   // ---------

   // Create a texture
   glGenTextures(1, &texture2);

   // Bind the texture object to the GL_TEXTURE_2D target
   // Any subsequent texture commands will configure the currently bound texture object
   glBindTexture(GL_TEXTURE_2D, texture2);

   // Set the texture wrapping parameters
   // GL_REPEAT is the default wrapping method
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // Set the texture filtering parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Load the texture image
   data = stbi_load("tex/awesomeface.png", &width, &height, &nrChannels, 0);

   if (data)
   {
      // Generate the texture and the mipmaps
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   }
   else
   {
      std::cout << "Failed to load texture" << std::endl;
   }

   // We can unbind the texture so that other texture calls won't accidentally modify it
   glBindTexture(GL_TEXTURE_2D, 0);

   // Since the texture has been generated, we can delete the texture image
   stbi_image_free(data);

   // Tell OpenGL what texture belongs to each sampler
   // ------------------------------------------------

   // Use the shader program before setting the uniforms
   glUseProgram(shaderProgram);

   // Tell the fragment shader to look for texture1's data in texture unit 0
   glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
   // Tell the fragment shader to look for texture2's data in texture unit 1
   glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

   // Render loop
   // ****************************************************************************************************

   // Uncomment this call to draw in wireframe polygons.
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   // The opposite is:
   //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Activate texture unit 0
      // A subsequent glBindTexture call will bind a texture to the currently active texture unit
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      // Activate texture unit 1
      // A subsequent glBindTexture call will bind a texture to the currently active texture unit
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);

      // glUseProgram specifies the shader program that is to be used in all subsequent drawing commands
      glUseProgram(shaderProgram);

      glm::mat4 model;
      glm::mat4 view;
      glm::mat4 projection;

      model = glm::rotate(model, ((float) glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      // Move scene 3 units in the negative Z-direction
      // This is equivalent to moving the camera 3 units in the positive Z-direction
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

      // Get the locations of the model, view and projection matrices in the vertex shader
      unsigned int modelLoc      = glGetUniformLocation(shaderProgram, "model");
      unsigned int viewLoc       = glGetUniformLocation(shaderProgram, "view");
      unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

      // Pass the model, view and projection matrices to the vertex shader
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
      // Note: We currently calculate and set the projection matrix for each frame, but since this matrix
      //       rarely changes it's often best to calculate it and set it outside of the main loop
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

      // There is no need to bind the VAO in every iteration of the render loop, since there is only 1, but we will do it anyway in this example
      glBindVertexArray(VAO);

      // Draw
      glDrawArrays(GL_TRIANGLES, 0, 36);

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

// Shader structure:
// - Version declaration
// - List of input, output and uniform variables
// - Main function

// When we're talking about the vertex shader, each input variable is also known as a vertex attribute.
// The hardware determines the maximum number of vertex attributes we are allowed to declare.
// OpenGL guarantees there are always at least 16 4-component vertex attributes available.
// To know the maximum number of vertex attributes supported by your hardware, query GL_MAX_VERTEX_ATTRIBS:

// int numVertexAttributes;
// glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numVertexAttributes);

// GLSL basic types: int, float, double, uint and bool.
// GLSL container types: vectors and matrices.

// A vector in GLSL is a 1,2,3 or 4 component container for any of the basic types mentioned above.
// They can take the following form (n represents the number of components):
// vecn:  the default vector of n floats.
// bvecn: a vector of n booleans.
// ivecn: a vector of n integers.
// uvecn: a vector of n unsigned integers.
// dvecn: a vector of n double components.

// The components of vectors can be accessed with xyzw, rgba and stpw.
// Swizzling is also supported.

// Inputs and outputs:
// Each shader can specify inputs and outputs using the in and out keywords.
// When an output variable matches an input variable of the next shader stage, the output values are passed along.

// Note that vertex shaders and fragment shaders differ in terms of their inputs.
// A vertex shader receives its input straight from the vertex data, since it is the first stage of the rendering pipeline.
// To define how the vertex data is organized, we specify the input variables of the vertex shader with location metadata (e.g. layout (location = 0)).
// By doing this, we are allowed to configure the vertex attributes on the CPU.
// The vertex shader thus requires an extra layout specification for its inputs so that we can link them with the vertex data.
// Note that it is possible to omit the layout (location = 0) specifier and query for the attribute locations in your code using glGetAttribLocation.

// Another difference is that fragment shaders require a vec4 color output variable, since fragment shaders need to generate a final output color.
// If you do not to specify an output color in your fragment shader, OpenGL will render your object black (or white).

// If we want to send data from one shader to another, we have to declare an output in the sending shader and a similar input in the receiving shader.
// When the output of the sending shader and the input of the receiving shader have the same names and types, OpenGL links them together.
// Once this is done, it is possible to send data between the two shaders.
// This connection between input and outputs is established when we link a program object.

// Uniforms:
// Uniforms are another way to pass data from our application on the CPU to the shaders on the GPU.
// Uniforms differ from vertex attributes in two ways:
// - Uniforms are global.
//   Global, meaning that a uniform variable is unique per shader program object, and can be accessed from any shader at any stage in the shader program.
// - When you set the value of a uniform variable, it keeps its value until it is either reset or updated.

// If you declare a uniform that is not used anywhere in your GLSL code, the compiler will silently
// remove the variable from the compiled versiom. This can lead to frustrating errors!

// Here is an example where we use a uniform variable in the fragment shader:

// #version 330 core
// out vec4 FragColor;
//
// uniform vec4 ourColor;
//
// void main()
// {
//    FragColor = ourColor;
// }

   // Time in seconds since GLFW was initialized
// float timeValue  = glfwGetTime();

   // greenValue ranges from 0.0 to 1.0
// float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

   // Retrieve the location of the uniform ourColor from the given shader program
   // If the location of the uniform is not found, the function returns -1
// int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

   // To retrieve the location of a uniform variable from a shader program, we do not have to call glUseProgram before
   // But to update the value of a uniform variable, we do need to call glUseProgram first,
   // because the function used to we set the value of a uniform variable acts on the currently active shader program
// glUseProgram(shaderProgram);

   // Set the value of the uniform in the currently active shader program
   // The postfix of the glUniform call specifies the type of the uniform we want to set
   // A few of the possible postfixes are:
   //  f: float
   //  i: int
   // ui: unsigned int
   // 3f: 3 floats
   // fv: vector/array of floats -> Syntax: glUniform(location, count, array[count] or vec[count])
// glUniform4f(vertexColorLocation, // Location of uniform variable
//             0.0f,                // R
//             greenValue,          // G
//             0.0f,                // B
//             1.0f);               // A

// Up to this point we know how to:
// - Fill a VBO
// - Configure vertex attribute pointers
// - Store the configuration in a VAO
// Let's add color to the vertex data now

// When we supply 1 color for each vertex of a triangle, why do we end up with a gradient?
// The gradient you see is the result of something called fragment interpolation, which is done in the fragment shader
// The rasterization stage usually results in a number of fragment that is much larger than the number of vertices we provided
// The rasterizer determines the positions of the fragments based on where they reside on the triangle shape
// It then interpolates all the input variables of the fragment shader based on these positions

// Texturing:

// A texture is a 1D, 2D or 3D image used to add detail to an object.
// Aside from images, textures can also be used to store a large collection of data to send to shaders.
// In order to map a texture to a triangle, we need to tell each vertex of the triangle which part of the texture it corresponds to.
// Each vertex should thus have a texture coordinate associated with it that specifies what part of the texture image to sample from.
// Fragment interpolation then does the rest for all the fragments.

// Texture coordinates range from 0 to 1 in the x and y axes.
// Retrieving the color of a texture using coordinates is called sampling.
// Texture coordinates start at (0, 0) on the lower left corner of a texture image, and go up to (1, 1) at the upper right corner.

// For a triangle we only need to pass 3 texture coordinate points to the vertex shader, which passes them to the fragment shader.
// The fragment shader then interpolates all the texture coordinates for each fragment.

// Texture wrapping:

// Texture coordinates range from (0, 0) to (1, 1)
// What happens if we specify coordinates outside of this range?

// GL_REPEAT:          Repeat the texture image (default behaviour).
// GL_MIRRORED_REPEAT: Repeat the texture image but mirror it with each repeat.
// GL_CLAMP_TO_EDGE:   Clamp the coordinates between 0 and 1. This results in a streched edge pattern.
// GL_CLAMP_TO_BORDER: Coordinates outside the range are given a user-specified border color.

// Each mode can be set per coordinate axis (S, T and R = X, Y and Z):
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // Target, option and mode
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

// With the GL_CLAMP_TO_BORDER option we need to specify the border color:
// float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

// Texture filtering:

// Texture coordinates can be any floating point value, so OpenGL has to figure out what texture pixel (texel) corresponds to a given texture coordinate
// Texels are limited by the resolution, while texture coordinates are not.

// GL_NEAREST (Nearest Neighbor Filtering): Select the pixel whose center is closest to the texture coordinate (default behavior).
// GL_LINEAR ((Bi)linear filtering): Select an interpolated value, calculated using the texture coordinate's neighbording texels.

// Texture filtering can be set for magnifying and minifying operations (when scaling up or down)
// You could have GL_NEAREST when textures are scaled down, and GL_LINEAR when they are scaled up.

// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// Mipmaps:

// Say we have thousands of objects, each with an attached texture.
// The ones far away have the same high resolution texture as the ones close by.
// If a far away object only results in a few fragments,
// OpenGL has trouble retrieving the right color value for each fragment from the high resolution texture,
// since each fragment covers a large part of the texture image.
// This results in visual artifacts and wasted memory on high resolution textures.

// Solution: mipmaps.
// A mipmap is a collection of texture images, where each subsequent texture is half the size of the previous one.
// After a certain distance between the viewer and an object, OpenGL will use the mipmap texture that best suits the distance to the object.

// OpenGL might show visual artifacts like sharp edges between two mipmap levels.
// Just like normal texture filtering, it is also possible to filter between mipmap levels.
//    Texture        Mipmap
// GL_NEAREST_MIPMAP_NEAREST: Takes the nearest mipmap to match the pixel size and uses GL_NEAREST for texture sampling.
// GL_LINEAR_MIPMAP_NEAREST:  Takes the nearest mipmap level and samples using GL_LINEAR.
// GL_NEAREST_MIPMAP_LINEAR:  Linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples via GL_NEAREST.
// GL_LINEAR_MIPMAP_LINEAR:   Linearly interpolates between the two closest mipmaps and samples the texture via GL_LINEAR.

// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// You cannot set one the mipmap filtering options as the magnification filter.
// Mipmaps are used for when textures get downscaled.
// Trying to the one of the mipmap filtering options as the magnification filter results in the GL_INVALID_ENUM error code.

// int width, height, nrChannels;
// unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

// unsigned int texture;
// glGenTextures(1, &texture);

// Bind so any subsequent texture commands will configure the currently bound texture
// glBindTexture(GL_TEXTURE_2D, texture);

// Now that the texture is bound, we can generate a texture using the previously loaded image data:

// glTexImage2D generates a texture image on the currently bound texture object at the active texture unit
// glTexImage2D(GL_TEXTURE_2D,    // target:         Common ones are GL_TEXTURE_1D, 2D or 3D.
//              0,                // level:          Specifies the level-of-detail number.
                                  //                 Level 0 is the base image level.
                                  //                 Level n is the nth mipmap reduction image.
//              GL_RGB,           // internalFormat: Specifies the number of color components in the texture.
                                  //                 So it tells OpenGL the format in which we want to store the texture.
//              width,            // Width
//              height,           // Height
//              0,                // Border:         Legacy value, must be equal to 0.
//              GL_RGB,           // format:         Specifies the format of the pixel data.
//              GL_UNSIGNED_BYTE, // type:           Specifies the type of the pixel data.
//              data);            // data:           Array of bytes.

// Now the currently bound texture object has the texture image attached to it.
// But it only has the base-level of the texture image loaded.
// If we want to use mipmaps, we have to specify all the different images manually by continually increasing the 2nd argument.
// Or we can call glGenerateMipMap after generating the texture:

// Automatically generates all the mipmap levels for the currently bound texture image of the active texture unit.
// Each subsequent mipmap image is halved until the mipmap image has a width or height of 1 pixel.
// glGenerateMipmap(GL_TEXTURE_2D);

// After generating the texture and its mipmaps, we can free the image memory
// stbi_image_free(data);

// So generating a texture looks like this:

// unsigned int texture;
// glGenTextures(1, &texture);
// glBindTexture(GL_TEXTURE_2D, texture);
// // set the texture wrapping/filtering options (on the currently bound texture object)
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// // load and generate the texture
// int width, height, nrChannels;
// unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
// if (data)
// {
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
// }
// else
// {
//    std::cout << "Failed to load texture" << std::endl;
// }
// stbi_image_free(data);

// Now to apply a texture to a rectangle:

//                     Position             Color               TexCoords
//                    <--------------->    <-------------->    <-------->
// GLfloat data[] = { 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top right
//                    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom right
//                   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom left
//                   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f }; // Top left

// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
// glEnableVertexAttribArray(0);
// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
// glEnableVertexAttribArray(1);
// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
// glEnableVertexAttribArray(2);

// The vertex shader must forward the texture coordinates to the fragment shader

// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// layout (location = 2) in vec2 aTexCoord;
// 
// out vec3 ourColor;
// out vec2 TexCoord;
// 
// void main()
// {
//    gl_Position = vec4(aPos, 1.0);
//    ourColor = aColor;
//    TexCoord = aTexCoord;
// }

// How do we pass the texture object to the fragment shader? Using the GLSL built-in data-type sampler2D.
// To sample the color of a texture we use GLSL's built in texture function. The texture function samples the color value from the texture using the nearest/linear options we set earlier.

// #version 330 core
// out vec4 FragColor;
// 
// in vec3 ourColor;
// in vec2 TexCoord;
// 
// uniform sampler2D ourTexture;
// 
// void main()
// {
//    FragColor = texture(ourTexture, TexCoord);
// }

// All that is left is to bind the texture before we draw:

// glBindTexture(GL_TEXTURE_2D, texture);
// glBindVertexArray(VAO);
// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// Texture units:

// Why is the sampler2D variable a uniform if we didn't assign it some value with glUniform?
// Using glUniform1i we can assign a location value to the texture sampler, so that we can have multiple textures at once in a fragment shader.
// The location of a shader is known as its texture unit.
// The default texture unit for a texture is 0, which is also the default active texture unit.
// That's why we did not have to assign a location to our texture.

// Texture units allow us to have more than 1 texture in our shaders.
// By assigning texture units to the samplers, we can bind to multiple textures at once as long as we activate the corresponding texture unit first.
// Just like glBindTexture we can activate texture units using glActiveTexture:

// glActiveTexture(GL_TEXTURE0); // Activate the texture unit
// glBindTexture(GL_TEXTURE_2D, texture);

// After activating a texture unit, a subsequent glBindTexture call will bind that texture to the currently active texture unit.
// GL_TEXTURE0 is activated by default.
// OpenGL should have a minimum of 16 texture units.

// To accept another sampler in the fragment shader:

// #version 330 core
// ...
// 
// uniform sampler2D texture1;
// uniform sampler2D texture2;
// 
// void main()
// {
//    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
// }

// The mix function takes two values as input and linearly interpolates between them based on its third argument.
// If the 3rd argument is 0.0, the function returns the first input.
// If it is 1.0, it return the second input.
// If it is 0.2, it return 80% of the first input and 20% of the second input.

// This is how we give the two textures to the shader:

// ... Generate the two textures ...
// Tell OpenGL how to connect the texture samplers with the texture units using glUniform1i:

// ourShader.use()                                                 // Using our Shader class
// glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // Texture unit 0
// ourShader.setInt("texture2", 1);                                // Texture unit 1

// Inside the render loop:
// glActiveTexture(GL_TEXTURE0);
// glBindTexture(GL_TEXTURE_2D, texture1);
// glActiveTexture(GL_TEXTURE1);
// glBindTexture(GL_TEXTURE_2D, texture2);
// 
// glBindVertexArray(VAO);
// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// Texture is flipped upside down?
// OpenGL expects the 0.0 coordinate of the Y-axis to be on the bottom side of the image,
// but images usually have 0.0 at the top of the Y-axis?
// The stb_image library has a setting for flipping images while loading them:

// stbi_set_flip_vertically_on_load(true);

// Coordinate systems

// Vertices must be in NDC after the vertex shader is executed
// We usually specify the vertex coordinates in a range we configure ourselves,
// and then we transform these coordinates to NDC in the vertex shader
// The NDC are then given to the rasterizer, which transforms them to 2D coordinates/pixels on the screen

// Vertex coordinates -> NDC -> Screen coordinates
// Transformations done in a step-by-step fashion
// The advantage of transforming an object vertices to several intermediate coordinate systems is that
// some of the calculations are easier to perform in certain coordinate systems

// Five coordinate systems:
// 1) Local space or object space
// 2) World space
// 3) View space or eye space
// 4) Clip space
// 5) Screen space

// Let's examine each one in detail:

// 1) Local space:
// Local coordinates are the coordinates of an object relative to its local origin
// All the vertices of an object are in local space

// 2) World space:
// World coordinates are coordinates that are specified relative to the global origin of the world
// The model matrix transforms local coordinates into world coordinates
// The model matrix is a transformation matrix that translates, rotates and scales objects to give them
// a specific location/orientation in the world

// 3) View space:
// Also known as the camera space of the eye space
// The view space is the result of transforming world-space coordinates to coordinates that are in front
// of the user's view
// The view matrix transforms world coordinates to camera coordinates

// 4) Clip space:
// At the end of each vertex shader run, OpenGL expects the coordinates to be within a specific range.
// Any coordinate that falls outside of this range is clipped.
// Coordinates that are clipped are discarded.

// Because specifying the visible coordinates to be within the range -1.0 and 1.0 isn't intuitive,
// we specify our own coordinate range to work with and then convert the coordinates in that range to NDC

// To transform vertex coordinates from view space to clip space, we define a projection matrix that
// specifies a range of coordinates in each dimension. The projection matrix then transforms the coordinates
// within this specified range to NDC (-1.0, 1.0).
// All coordinates outside the specified range will not be mapped between -1.0 and 1.0, so they will be clipped.

// Example: If we specify the range [-1000, 1000] in the X, Y and Z axes, a coordinate of (1250, 500, 750)
// would not be visible, since the X coordinate is outside of the range and thus gets converted to a
// coordinate higher than 1.0 in NDC.

// Note that if only a part of a primitive e.g. a triangle is outside of the clipping volume,
// OpenGL will reconstruct the triangle as one or more triangles to fit inside the clipping range.

// The viewing box that a projection matrix creates is called a frustum
// Each coordinate inside this frustum will end up on the user's screen
// The total process of converting coordinates within a specified range to NDC that can be easily mapped
// to 2D view-space coordinates is called projection, since the projection matrix projects 3D coordinates
// to easy-to-map-to-2D normalized device coordinates

// Once all the vertices are transformed to the clip space, a final operation called perspective division
// is performed, where we divide the X, Y and Z components of the position vectors by the vector's
// homogenous W component.
// Perspective division is what transforms the 4D clip space coordinates to 3D normalized device coordinates
// This step is performed automatically at the end of each vertex shader run
// It is after this stage where the resulting coordinates are mapped to screen coordinates
// using the settings of glViewport, and turned into fragments

// The projection matrix that transforms view coordinates to clip coordinates can take two different forms,
// where each form defines its own unique frustum:

// 1) Orthographic projection:
// An ortographic projection matrix defines a cube-like frustum
// Because of this, when we create an ortographic projection matrix we need to specify the width,
// height and length of the visible frustum
// All the coordinates that end up inside this frustum after being transformed from the view space to the
// clip space will not be clipped

// The dimensions of the frustum are specified with a width, a height, a near plane and a far plane.
// The ortographic frustum directly maps all coordinates inside the frustum to NDC since the W
// component of each vector is untouched
// If the W component is equal to 1.0, perspective division does not change the coordinates

// glm::ortho(GLint left, GLint right, GLint bottom, GLint top, GLfloat near, GLfloat far)
// glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

// An ortographic projection matrix directly maps coordinates to the 2D plane that is the screen
// This produces unrealistic results since the projection doesn't take perspective into account

// 2) Perspective projection:
// Objects that are far away are smaller than objects that are nearby
// Parallel lines seem to converge at infinity
// A projection matrix maps a given frustum range to clip space, but it also manipulates the W component
// of each vertex coordinate in such a way that the further away a vertex coordinate is from the viewer,
// the higher this W component becomes.
// Once the coordinates are transformed to the clip space, they are in the range -W to W
// Anything outside this range is clipped.
// OpenGL requires that the visible coordinates fall between -1.0 and 1.0, thus once the coordinates are
// in the clip space, perspective division is applied
// Each component of the each vertex coordinate is divided by its W component, resulting in smaller vertex
// coordinates the further away a vertex is from the viewer
// This is another reason why the W component is important, aside from the fact that it enables translations
// The resulting coordinates are then in normalized device space

// glm::mat4 proj = glm::perspective(glm::radians(45.0f),          // FoV
//                                   (float)width / (float)height, // Aspect ratio
//                                   0.1f,                         // Near
//                                   100.0f);                      // Far

// Whenever the near value of your perspective matrix is set a bit too high (like 10.0f),
// OpenGL will clip all coordinates close to the camera (between 0.0f and 10.0f),
// which gives a familiar visual result in videogames where you can see through certain objects
// if you move too close to them.

// Ortographic projection -> Each of the vertex coordinates is directly mapped to clip space
//                           The W component is not manipulated, so perspective division has no effect
//                           So objects far away do not seem smaller
//                           Good for 2D renderings and architectural and engineering applications

// V_clip = M_proj * M_view * M_model * V_local
// Result should be assigned to gl_Position in the vertex shader
// OpenGL handles the perspective division and clipping

// So vertex shader outputs clip space coordinates
// OpenGL then transforms those coordinates to NDC by doing the perspective division
// Finally, OpenGL uses the parameters from glViewport to map the NDC to screen coordinates, where each
// coordinate corresponds to a point on the screen. This process is called the viewport transform.

// Going 3D

// Say we want to move the camera backwards
// This the same as moving the scene forward
// Which is exactly what the view matrix does
// Because we want to move backwards and because OpenGL uses a right-handed coordinate system,
// we need to move the camera in the positive Z-axis direction.
// So the scene must move in the negative Z-axis direction.
// Note that in NDC OpenGL actually uses a left-handed coordinate system (the projection matrix switches the handedness)

// Z-buffer

// OpenGL stores all its depth information in what is called the z-buffer.
// GLFW automatically creates a z-buffer, just like it creates a color-buffer to store the colors of the output image
// The depth is stored within each fragment
// When a fragment is going to be displayed, OpenGL compares the depth of the fragment with the depth stored in the z-buffer
// If the depth of the current fragment is greater than the depth stored in the z-buffer, the fragment is discarded
// If it is smaller, the fragment is displayed
// This process is called depth testing

// If we want OpenGL to perform depth testing, we need to enable it (it is disabled by default)
// We can enable depth testing using glEnable:

// glEnable(GL_DEPTH_TEST);

// We must also clear the z-buffer for each iteration:

// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

