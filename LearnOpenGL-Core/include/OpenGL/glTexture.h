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
		TextureType texType = TextureType::Unknown;
		bool needVerticalFlip = false;
	};

	class GLTexture
	{
	public:
		// 一般构造函数
		GLTexture(const std::string& filePath, TextureType texType = TextureType::Unknown, bool needVerticalFlip = false);
		GLTexture(const TextureData& textureData);
		// 特殊构造函数(用于FBO, Skybox等无需立即加载纹理的情况)
		GLTexture(unsigned int id, int width, int height);
		~GLTexture();

		// slot: 绑定纹理的位置, 纹理可一次性绑定多个, PC可能是32, 手机可能是8
		void bind(unsigned int slot = 0) const;
		void unbind() const;

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline std::string getFilePath() const { return m_textureData.filePath; }
		inline TextureType getTextureType() const { return m_textureData.texType; }
		inline unsigned int getID() const { return m_rendererID; }

	private:
		unsigned int m_rendererID;
		TextureData m_textureData;
		unsigned char* m_localBuffer;
		int m_width, m_height, m_bpp;
	};
}
