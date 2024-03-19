#pragma once

namespace GLCore
{
	enum class TextureType
	{
		Unknown = 0,
		AmbientMap,
		DiffuseMap,
		SpecularMap
	};

	struct TextureData
	{
		std::string filePath;
		TextureType texType;
		bool needVerticalFlip = false;
	};

	class GLTexture
	{
	public:
		GLTexture(const std::string& filePath, TextureType texType = TextureType::Unknown, bool needVerticalFlip = false);
		GLTexture(const TextureData& textureData);
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
		TextureType m_textureType;
		char* m_localBuffer;
		int m_width, m_height, m_bpp;
	};
}