#pragma once

namespace GLCore
{
	class GLIndexBuffer
	{
	public:
		GLIndexBuffer(const unsigned int* data, unsigned int count);
		~GLIndexBuffer();

		void bind() const;
		void unbind() const;

		inline unsigned int getCount() const { return m_count; }

	private:
		unsigned int m_rendererID;
		unsigned int m_count;		// 该IBO内的元素数
	};
}