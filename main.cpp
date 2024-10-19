// Headers for OpenGL and SFML
// #include "stdafx.h"  // This line might be needed in some IDEs
#include <GL/glew.h>
#include <SFML/Window.hpp>

// Vertex shader source code
const GLchar* vertexSource = R"glsl(
#version 150 core
in vec2 position;  // Input vertex position
in vec3 color;     // Input vertex color
out vec3 Color;    // Output color passed to the fragment shader

void main() {
    Color = color;  // Pass the color to the fragment shader
    gl_Position = vec4(position, 0.0, 1.0);  // Set the position of the vertex
}
)glsl";

// Fragment shader source code
const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec3 Color;       // Color received from the vertex shader
out vec4 outColor;   // Output color to the framebuffer

void main() {
    outColor = vec4(Color, 1.0);  // Set the fragment color with full opacity
}
)glsl";

int main() {
    // Setup OpenGL context settings
    sf::ContextSettings settings;
    settings.depthBits = 24;     // Bits for depth buffer
    settings.stencilBits = 8;    // Bits for stencil buffer

    // Create a rendering window with OpenGL context
    sf::Window window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

    // Initialize GLEW (must be done after creating the window and OpenGL context)
    glewExperimental = GL_TRUE;
    glewInit();

    // Create and bind a Vertex Array Object (VAO) to store vertex state
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object (VBO) and upload vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Vertex data: positions (x, y) and colors (r, g, b) for each vertex
    GLfloat vertices[] = {
        0.0f, 0.5f, 1.0f, 0.0f, 0.0f,  // Vertex 1: position (0.0, 0.5), color (red)
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: position (0.5, -0.5), color (green)
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f // Vertex 3: position (-0.5, -0.5), color (blue)
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link both shaders into a single shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");  // Bind fragment output
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // Main event loop
    bool running = true;
    while (running) 
    {
        sf::Event windowEvent;
        while (window.pollEvent(windowEvent)) 
        {
            if (windowEvent.type == sf::Event::Closed) 
            {
                running = false;  // Exit loop if the window is closed
            }
        }

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle using the vertex data
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap the front and back buffers
        window.display();
    }

    // Cleanup: delete shaders, buffers, and close the window
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    window.close();  // Close the rendering window
    return 0;
}
