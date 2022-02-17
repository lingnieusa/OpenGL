#include "VertexBuffer.h"
#include"Renderer.h"
VertexBuffer::VertexBuffer(const void* data, unsigned int size) {

	glGenBuffers(1, &m_RendererID);//CREATE BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);//SELECT BUFFER
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);//WHERE
}
VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);//SELECT BUFFER
}
void VertexBuffer::Unbind() const{
	glBindBuffer(GL_ARRAY_BUFFER, 0);//SELECT BUFFER
}