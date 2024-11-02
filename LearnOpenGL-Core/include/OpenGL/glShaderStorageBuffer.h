#pragma once

namespace GLCore
{
	class GLShaderStorageBuffer
	{
	public:
		explicit GLShaderStorageBuffer(size_t bufferSize);
		virtual ~GLShaderStorageBuffer();

		template <typename T>
		void writeSsboData(const std::vector<T>& bufferData, int bindingPoint)
		{
			size_t bufferSize = bufferData.size() * sizeof(T);
			if (bufferSize == 0 || bufferSize > m_InitBufferSize)
			{
				LOG_ERROR(std::format("[{}]: BufferSize invalid (0 or > ssbo's bufferSize) !!", __FUNCTION__));
				return;
			}
			m_CurBufferSize = bufferSize;

			bind();
			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_CurBufferSize, bufferData.data()));
			GLCall(glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_rendererID, 0, m_CurBufferSize));
			unbind();
		}

		template <typename T>
		void readSsboData(const T* pBufferDataDst, size_t bufferSizeDst)
		{
			bind();

			// 保证数据修改完成后再读取
			GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
			void* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
			memcpy_s(pBufferDataDst, bufferSizeDst, ptr, m_CurBufferSize);
			GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
			unbind();
		}

		void bind() const;
		void unbind() const;

	private:
		unsigned int m_rendererID;
		size_t m_InitBufferSize;
		size_t m_CurBufferSize;
	};
}
