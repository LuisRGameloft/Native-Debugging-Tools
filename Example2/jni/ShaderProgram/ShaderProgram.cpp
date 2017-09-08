

#include <ShaderProgram/ShaderProgram.hpp>
#include <fstream>
#include <Utils/debug.h>

using namespace std;

ShaderProgram::ShaderProgram()
    : m_vertex_shader(0), m_fragment_shader(0),m_geometry_shader(0), m_program(0) {}

void ShaderProgram::init(const char *vertex_shader, const char *fragment_shader, const char *geometry_shader)
{
  // It creates the shaders
  m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  #ifndef OS_ANDROID
  if (geometry_shader)
	  m_geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
  #endif // OS_ANDROID

  // It sends the source code to opengl (Vertex shader)
  {
      char *buffer = readFile(vertex_shader);
      glShaderSource(m_vertex_shader, 1, (const GLchar **)&buffer, 0);
      delete[] buffer;
  }

  // It sends the source code to opengl (Fragment shader)
  {
      char *buffer = readFile(fragment_shader);
      glShaderSource(m_fragment_shader, 1, (const GLchar **)&buffer, 0);
      delete[] buffer;
  }

  #ifndef OS_ANDROID
  {
      if (geometry_shader) {
	      // It sends the source code to opengl (Geometry shader)
	      const char * buffer = readFile(geometry_shader);
	      glShaderSource(m_geometry_shader, 1, (const GLchar **)&buffer, 0);
	      delete[] buffer;
      }
  }
  #endif // OS_ANDROID

  // It compiles the vertex shader
  glCompileShader(m_vertex_shader);
  printCompilationError(m_vertex_shader, GL_VERTEX_SHADER);

  // It compiles the fragment shader
  glCompileShader(m_fragment_shader);
  printCompilationError(m_fragment_shader, GL_FRAGMENT_SHADER);

  #ifndef OS_ANDROID
  if (geometry_shader) {
	  glCompileShader(m_geometry_shader);
	  printCompilationError(m_geometry_shader, GL_GEOMETRY_SHADER);
  }
  #endif // OS_ANDROID

  // It creates and links the program
  m_program = glCreateProgram();
  glAttachShader(m_program, m_vertex_shader);
  glAttachShader(m_program, m_fragment_shader);
  #ifndef OS_ANDROID
  if (geometry_shader)
	  glAttachShader(m_program, m_geometry_shader);
  #endif //OS_ANDROID
  glLinkProgram(m_program);
}

// you can call makecurrent or useProgram both do the same thing
void ShaderProgram::makeCurrent() { glUseProgram(m_program); } // this is a glUseProgram wrapper
void ShaderProgram::useProgram() { glUseProgram(m_program); }  // this is a glUseProgram wrapper


// Returns the location/address/index of an attribute
GLint ShaderProgram::getAttribLocation(const char *attrib) {
  return glGetAttribLocation(m_program, attrib);
}

// Returns the location/address/index of an uniform
GLint ShaderProgram::getUniformLocation(const char *uniform) {
  return glGetUniformLocation(m_program, uniform);
}

// Destructor deletes opengl objects
ShaderProgram::~ShaderProgram() {
  glDeleteShader(m_vertex_shader);
  glDeleteShader(m_fragment_shader);
  glDeleteProgram(m_program);
}

// It reads a file and returns its content
// IMPORTANT: you need to free the memory (use: delete [ ] )
char* ShaderProgram::readFile(const char* filename)
{
    ifstream _file(filename, ios::binary);
    _file.seekg(0, _file.end);
    int len = _file.tellg();
    len++;
    _file.seekg(0, _file.beg);
    char *buffer = new char[len];
    _file.read(buffer, len);
    buffer[len - 1] = 0x00;
    _file.close();
    return buffer;
}


// If an error exists then prints the message
void ShaderProgram::printCompilationError(GLuint shader_handler, GLenum target)
{
  GLint isCompiled = 0;
  glGetShaderiv(shader_handler, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(shader_handler, GL_INFO_LOG_LENGTH, &maxLength);
    char *error = new char[maxLength];
    glGetShaderInfoLog(shader_handler, maxLength, &maxLength, error);
    if(target == GL_VERTEX_SHADER)
    {
        debug("Vertex Shader Error: %s\n", error);
    }
    else if(target == GL_FRAGMENT_SHADER)
    {
        debug("Fragment Shader Error: %s\n", error);
    }

    #ifndef OS_ANDROID
	else if (target == GL_GEOMETRY_SHADER)
    {
		debug("Geometry Shader Error: %s\n", error);
    }
    #endif //OS_ANDROID
    delete[] error;
  }
}
