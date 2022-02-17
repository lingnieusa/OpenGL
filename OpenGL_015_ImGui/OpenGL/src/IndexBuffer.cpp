#include "IndexBuffer.h"
#include"Renderer.h"
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count):m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));
	glGenBuffers(1, &m_RendererID);//CREATE BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);//SELECT BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);//WHERE
}
IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);//SELECT BUFFER
}
void IndexBuffer::Unbind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//SELECT BUFFER
}