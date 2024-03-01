#pragma once

class GLVertexBuffer
{
private:
	unsigned int m_rendererID;

public:
	explicit GLVertexBuffer(const void* data, unsigned int size);
	~GLVertexBuffer();

	void bind() const;
	void unbind() const;
};
