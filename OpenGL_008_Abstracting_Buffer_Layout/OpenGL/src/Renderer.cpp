#include "Renderer.h"
#include<iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

void GLCheckError() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error](" << error << ")" << std::endl;
	}
}
///////////////170
bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error](" << error << ") " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}