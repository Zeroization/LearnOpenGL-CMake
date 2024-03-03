#pragma once

class GLVertexBuffer
{
public:
	explicit GLVertexBuffer(const void* data, unsigned int size);
	~GLVertexBuffer();

	void bind() const;
	void unbind() const;

private:
	unsigned int m_rendererID;
};
