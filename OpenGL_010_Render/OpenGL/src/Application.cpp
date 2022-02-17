#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"

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
	{

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
		//unsigned int vao;
		//glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;

		layout.Push<float>(2);

		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
		
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		Renderer renderer;
		
		/////////////////////////////////////////////////////////////////////////////////Parameter Set Up


		renderer.Clear();
		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{////////////////////////////////////////////////////////////////////////////////Draw
			/* Render here */
			

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			renderer.Draw(va,ib,shader);
			//glBindBuffer(GL_ARRAY_BUFFER, buffer);///
			//GLCall(glEnableVertexAttribArray(0);)
			//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//

			//glDrawArrays(GL_TRIANGLES, 0, 6);//DRAW


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

	}
	glfwTerminate();
	return 0;
}
