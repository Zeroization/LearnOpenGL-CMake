#pragma once

namespace GLCore
{
	class GLTexture
	{
	public:
		GLTexture(const std::string& filePath);
		~GLTexture();

		// slot: �������λ��, �����һ���԰󶨶��, PC������32, �ֻ�������8
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