#pragma once

namespace GLCore
{
	class GLTexture
	{
	public:
		GLTexture(const std::string& filePath);
		~GLTexture();

		// slot: 绑定纹理的位置, 纹理可一次性绑定多个, PC可能是32, 手机可能是8
		void bind(unsigned int slot = 0) const;
		void unbind() const;

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline std::string getFilePath() const { return m_filePath; }

	private:
		unsigned int m_rendererID;
		std::string m_filePath;
		char* m_localBuffer;
		int m_width, m_height, m_bpp;
	};
}