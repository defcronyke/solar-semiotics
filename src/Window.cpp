#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

#include "LoadShaders.h"
extern GLuint LoadShaders(ShaderInfo *shaderinfo);

#define PROGRAM_NAME "Red Book - Chapter 3 - Draw Commands"

void Initialize();
void Display();
void Finalize(void);
void Reshape(int width, int height);

float aspect;
GLuint render_prog;
GLuint vao;
GLuint vbo;
GLuint ebo;

GLint render_model_matrix_loc;
GLint render_projection_matrix_loc;

void Initialize() {

    ShaderInfo shader_info[] = {
      { GL_VERTEX_SHADER, "primitive_restart.vs.glsl" },
      { GL_FRAGMENT_SHADER, "primitive_restart.fs.glsl" },
      { GL_NONE, NULL }
    };

  render_prog = LoadShaders(shader_info);
  glUseProgram(render_prog);

  render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
  render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");

  // A single triangle
  static const GLfloat vertex_positions[] = {
      -1.0f, -1.0f,  0.0f, 1.0f,
      1.0f, -1.0f,  0.0f, 1.0f,
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 1.0f,
  };

  // Color for each vertex
  static const GLfloat vertex_colors[] = {
      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f
  };

  // Indices for the triangle strips
  static const GLushort vertex_indices[] = {
      0, 1, 2
  };

  // Set up the element array buffer
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

  // Set up the vertex attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_positions));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Display() {
  glm::mat4 model_matrix;

  // Setup
  glEnable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activate simple shading program
  glUseProgram(render_prog);

  // Set up the model and projection matrix
  glm::mat4 projection_matrix(glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 500.0f));
  glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

  // Set up for a glDrawElements call
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  // Draw Arrays...
  model_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-3.0f, 0.0f, -5.0f));
  glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // DrawElements
  model_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(-1.0f, 0.0f, -5.0f));
  glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);

  // DrawElementsBaseVertex
  model_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(1.0f, 0.0f, -5.0f));
  glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
  glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL, 1);

  // DrawArraysInstanced
  model_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(3.0f, 0.0f, -5.0f));
  glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
  glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);
}

void Finalize(void) {
  glUseProgram(0);
  glDeleteProgram(render_prog);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void Reshape(int width, int height) {
  glViewport(0, 0 , width, height);
  aspect = float(height) / float(width);
}

int main(int argc, char *argv[]){

  SDL_Window *mainwindow; /* Our window handle */
  SDL_GLContext maincontext; /* Our opengl context handle */

  if (SDL_Init(SDL_INIT_VIDEO) < 0) { /* Initialize SDL's Video subsystem */
    std::cout << "Unable to initialize SDL";
    return 1;
  }

  /* Request opengl 4.4 context. */
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

  /* Turn on double buffering with a 24bit Z buffer.
   * You may need to change this to 16 or 32 for your system */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  /* Create our window centered at 512x512 resolution */
  mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (!mainwindow){ /* Die if creation failed */
     std::cout << "SDL Error: " << SDL_GetError() << std::endl;
     SDL_Quit();
     return 1;
  }

  /* Create our opengl context and attach it to our window */
  maincontext = SDL_GL_CreateContext(mainwindow);

  GLenum rev;
  glewExperimental = GL_TRUE;
  rev = glewInit();

  if (GLEW_OK != rev){
      std::cout << "Error: " << glewGetErrorString(rev) << std::endl;
      exit(1);
  } else {
      std::cout << "GLEW Init: Success!" << std::endl;
  }

  /* This makes our buffer swap syncronized with the monitor's vertical refresh */

SDL_GL_SetSwapInterval(1);

  bool quit=false;

  Initialize();
  Reshape(512,512);

  SDL_Event event;

  while(!quit){

    Display();
    SDL_GL_SwapWindow(mainwindow);

    while( SDL_PollEvent( &event ) ){
	if( event.type == SDL_QUIT ){
            quit = true;
	  }
      }
  }

  Finalize();

  /* Delete our opengl context, destroy our window, and shutdown SDL */
  SDL_GL_DeleteContext(maincontext);
  SDL_DestroyWindow(mainwindow);
  SDL_Quit();

  return 0;
}


// #include "Window.hpp"
// #include <string>
// #include <iostream>
// #include <cstdint>
// #define GL3_PROTOTYPES 1
// #include <SDL.h>
// #include <GL/glew.h>
// #include <GL/gl.h>
//
// using namespace std;
//
// Window::Window(const char* name, int posX, int posY, int sizeX, int sizeY, uint32_t flags) {
//   cout << "Creating SDL2 window" << endl;
//
//   // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
//   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//
//   Win = SDL_CreateWindow(name, posX, posY, sizeX, sizeY, flags);
//   if (Win == nullptr) {
//     cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
//     return;
//   }
//
//   glewExperimental = GL_TRUE;
//   glewInit();
//
//   Ctx = SDL_GL_CreateContext(Win);
//
//   SDL_GL_SetSwapInterval(1);
//
//   cout << "Initializing OpenGL" << endl;
//   InitGL();
//   // glClearColor(0.0, 0.5, 0.5, 1.0);
//   // glClear(GL_COLOR_BUFFER_BIT);
//   // SDL_GL_SwapWindow(Win);
// }
//
// Window::~Window() {
//   cout << "Destroying SDL2 window" << endl;
//   if (Win != nullptr) {
//     SDL_DestroyWindow(Win);
//   }
// }
//
// bool Window::InitGL() {
//   const GLfloat diamond[4][2] = {
//   {  0.0,  1.0  }, /* Top point */
//   {  1.0,  0.0  }, /* Right point */
//   {  0.0, -1.0  }, /* Bottom point */
//   { -1.0,  0.0  } }; /* Left point */
//
//   const GLfloat colors[4][3] = {
//   {  1.0,  0.0,  0.0  }, /* Red */
//   {  0.0,  1.0,  0.0  }, /* Green */
//   {  0.0,  0.0,  1.0  }, /* Blue */
//   {  1.0,  1.0,  1.0  } }; /* White */
//
//   cout << "!" << endl;
//
//   glGenVertexArrays(1, &Vao);
//   cout << "!!" << endl;
//   glBindVertexArray(Vao);
//   glGenBuffers(2, Vbo);
//
//
//
//   glBindBuffer(GL_ARRAY_BUFFER, Vbo[0]);
//   glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
//
//   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
//   glEnableVertexAttribArray(0);
//
//   glBindBuffer(GL_ARRAY_BUFFER, Vbo[1]);
//   glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
//   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//   glEnableVertexAttribArray(1);
//   cout << "Loading shaders" << endl;
//   vSrc = FileToBuf("../assets/shaders/v.glsl");
//   fSrc = FileToBuf("../assets/shaders/f.glsl");
//
//
//   //
//   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//   // const GLchar* vertexShaderSource[] = {
//   //   "#version 140\nin vec2 vertPos2DLoc; void main() { gl_Position = vec4( vertPos2DLoc.x, vertPos2DLoc.y, 0, 1 ); }"
//   // };
//   glShaderSource(vertexShader, 1, (const GLchar**)&vSrc, 0);
//   glCompileShader(vertexShader);
//   GLint vShaderCompiled = GL_FALSE;
//   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
//   if (vShaderCompiled != GL_TRUE) {
//     cout << "Vertex shader compile failed: " << endl;
//     ShaderCompileError(vertexShader);
//     return false;
//   }
//   //
//
//   //
//   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//   // const GLchar* fragmentShaderSource[] = {
//   //   "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
//   // };
//   glShaderSource(fragmentShader, 1, (const GLchar**)&fSrc, 0);
//   glCompileShader(fragmentShader);
//   GLint fShaderCompiled = GL_FALSE;
//   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
//   if (vShaderCompiled != GL_TRUE) {
//     cout << "Fragment shader compile failed: " << endl;
//     ShaderCompileError(fragmentShader);
//     return false;
//   }
//   cout << "Shaders loaded. Linking shaders..." << endl;
//
//   ShaderProgram = glCreateProgram();
//
//   glAttachShader(ShaderProgram, vertexShader);
//   glAttachShader(ShaderProgram, fragmentShader);
//
//   glBindAttribLocation(ShaderProgram, 0, "in_Position");
//   glBindAttribLocation(ShaderProgram, 1, "in_Color");
//
//   glLinkProgram(ShaderProgram);
//   cout << "Shaders linked" << endl;
//
//   GLint programSuccess = GL_TRUE;
//   glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &programSuccess);
//   if (programSuccess != GL_TRUE) {
//     cout << "Shader linking failed: " << endl;
//     ProgramLinkError(ShaderProgram);
//     return false;
//   }
//   //
//   // VertPos2DLoc = glGetAttribLocation(ShaderProgramId, "vertPos2DLoc");
//   // if (VertPos2DLoc == -1) {
//   //   cout << "Error: vertPos2DLoc is not a valid glsl program variable" << endl;
//   //   return false;
//   // }
//   //
//   // glClearColor(0.0, 0.5, 0.5, 1.0);
//   //
//   // // // VBO data
//   // // GLfloat vertexData[] =
//   // // {
//   // //   -0.5f, -0.5f,
//   // //    0.5f, -0.5f,
//   // //    0.5f,  0.5f,
//   // //   -0.5f,  0.5f
//   // // };
//   // //
//   // // // IBO data
//   // // GLuint indexData[] = {
//   // //   0, 1, 2, 3
//   // // };
//   //
//   // glGenBuffers(1, &Vbo);
//   // glBindBuffer(GL_ARRAY_BUFFER, Vbo);
//   // glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
//   //
//   // glGenBuffers(1, &Ibo);
//   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibo);
//   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
//
//   return true;
// }
//
// void Window::ShaderCompileError(GLuint shader)
// {
//   if(glIsShader(shader))
//   {
//     //Shader log length
//     int infoLogLength = 0;
//     int maxLength = infoLogLength;
//
//     //Get info string length
//     glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
//
//     //Allocate string
//     char* infoLog = new char[maxLength];
//
//     //Get info log
//     glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
//     if(infoLogLength > 0)
//     {
//         //Print Log
//         printf("%s\n", infoLog);
//     }
//
//     //Deallocate string
//     delete[] infoLog;
//   }
//   else {
//     cout << "Error: " << shader << " is not a shader" << endl;
//   }
// }
//
// void Window::ProgramLinkError(GLuint program)
// {
//   //Make sure name is shader
//     if(glIsProgram(program))
//     {
//         //Program log length
//         int infoLogLength = 0;
//         int maxLength = infoLogLength;
//
//         //Get info string length
//         glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
//
//         //Allocate string
//         char* infoLog = new char[maxLength];
//
//         //Get info log
//         glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
//         if(infoLogLength > 0)
//         {
//             //Print Log
//             printf("%s\n", infoLog);
//         }
//
//         //Deallocate string
//         delete[] infoLog;
//     }
//     else
//     {
//         cout << "Error: " << program << " is not a shader program" << endl;
//     }
// }
//
// void Window::Render() {
//   glClear(GL_COLOR_BUFFER_BIT);
//   glUseProgram(ShaderProgram);
//
//   // glEnableVertexAttribArray(VertPos2DLoc);
//
//   glBindBuffer(GL_ARRAY_BUFFER, Vbo[0]);
//   glVertexAttribPointer(VertPos2DLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
//
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibo);
//   glDrawArrays(GL_LINE_LOOP, 0, 4);
//   SDL_GL_SwapWindow(Win);
//
//   // glDisableVertexAttribArray(VertPos2DLoc);
//   glUseProgram(0);
// }
//
// char* Window::FileToBuf(char const* file)
// {
//     FILE *fptr;
//     long length;
//     char *buf;
//
//     fptr = fopen(file, "rb"); /* Open file for reading */
//     if (!fptr) /* Return NULL on failure */
//         return NULL;
//     fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
//     length = ftell(fptr); /* Find out how many bytes into the file we are */
//     buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
//     fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
//     fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
//     fclose(fptr); /* Close the file */
//     buf[length] = 0; /* Null terminator */
//
//     return buf; /* Return the buffer */
// }
