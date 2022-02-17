#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include<fstream>
#include<string>
#include<sstream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

//static void GLCheckError() {
//	while (GLenum error = glGetError()) {
//		std::cout << "[OpenGL Error](" << error << ")" << std::endl;
//	}
//}
///////////////170
static bool GLLogCall(const char* function,const char* file,int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error](" << error << ") " <<function<<" "<<file<<":"<<line<< std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
	std::cout << "TEST:" << std::endl;
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE=-1,VERTEX=0,FRAGMENT=1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos){
				type = ShaderType::FRAGMENT;

			}
		}
		else {
			ss[(int)type] << line<<'\n';
		}

	}
	return{ ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//todo:error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " Shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;

}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;


}
////////////////////////////////////////////////////////////////////////////////Shader Set Up
int main(void)
{
	 
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}
	std::cout << "TESTV:" << glGetString(GL_VERSION) << std::endl;
	/////////////////////////////////////////////////////////////////////OpenGLPreparation
	float positions[] = {
		-0.5f,-0.5f,
		 0.5f,-0.5f,
		 0.5f, 0.5f,
		-0.5f, 0.5f,
	};
	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int buffer;
	glGenBuffers(1, &buffer);//CREATE BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//SELECT BUFFER
	glBufferData(GL_ARRAY_BUFFER, 4 *2* sizeof(float), positions, GL_STATIC_DRAW);//WHERE

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//HOW

	unsigned int ibo;//IndexBufferObject
	glGenBuffers(1, &ibo);//CboREATE BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//SELECT BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);//WHERE

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	std::cout << "Vertex" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "Fragment" << std::endl;
	std::cout << source.FragmentSource << std::endl;


	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color");
	ASSERT(location != -1);
	glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);
	/////////////////////////////////////////////////////////////////////////////////Parameter Set Up
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);//SELECT 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//


	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{////////////////////////////////////////////////////////////////////////////////Draw
		/* Render here */
		GLClearError();//glClear(GL_COLOR_BUFFER_BIT);
		
		glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
		glUseProgram(shader);///
		//glBindBuffer(GL_ARRAY_BUFFER, buffer);///
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);///
		//GLCall(glEnableVertexAttribArray(0);)
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//

		//glDrawArrays(GL_TRIANGLES, 0, 6);//DRAW
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);)///Draw

		if (r > 1.0f)
			increment = -0.01f;
		else if (r < 0.0f)
			increment = 0.01f;
		r += increment;
		//DRAW
		//ASSERT(GLLogCall());
		//        glBegin(GL_TRIANGLES);
		//        glVertex2f(-0.5f,-0.5f);
		//        glVertex2f( 0.0f, 0.5f);
		//        glVertex2f( 0.5f,-0.5f);
		//        glEnd();

				/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}/////////////////////////////////////////////////////////////////////////////////

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
