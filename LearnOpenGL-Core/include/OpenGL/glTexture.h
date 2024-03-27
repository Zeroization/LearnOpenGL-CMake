#pragma once

namespace GLCore
{
	enum class TextureType
	{
		Unknown = 0,
		AmbientMap,
		DiffuseMap,
		SpecularMap,
		CubeMap
	};

	// TextureDescription
	struct TextureDesc
	{
		std::string filePath;
		TextureType texType = TextureType::Unknown;
		bool needVerticalFlip = false;

		TextureDesc() = default;
		TextureDesc(const std::string& filePath_, TextureType texType_ = TextureType::Unknown, bool needVerticalFlip_ = false)
			: filePath(filePath_), texType(texType_), needVerticalFlip(needVerticalFlip_) {}
	};

	// TextureData
	struct TextureData
	{
		unsigned char* localBuffer = nullptr;
		int width = 0;
		int height = 0;
		int nrChannels = 0;

		TextureData() = default;
		TextureData(unsigned char* localBuffer_, int width_, int height_, int nrChannels_)
			: localBuffer(localBuffer_), width(width_), height(height_), nrChannels(nrChannels_) {}
	};

	class GLTexture
	{
	public:
		// 一般构造函数
		GLTexture(const std::string& filePath, TextureType texType = TextureType::Unknown, bool needVerticalFlip = false);
		GLTexture(const TextureDesc& textureData);
		// 特殊构造函数(用于FBO, Skybox等无需立即加载纹理的情况)
		GLTexture(unsigned int id = 0, int width = 0, int height = 0);
		virtual ~GLTexture();

		// slot: 绑定纹理的位置, 纹理可一次性绑定多个, PC可能是32, 手机可能是8
		virtual void bind(unsigned int slot = 0) const;
		virtual void unbind() const;

		inline int getWidth() const { return m_textureData.width; }
		inline int getHeight() const { return m_textureData.height; }
		inline std::string getFilePath() const { return m_textureDesc.filePath; }
		inline TextureType getTextureType() const { return m_textureDesc.texType; }
		inline unsigned int getID() const { return m_rendererID; }

	protected:
		unsigned int m_rendererID;
	private:
		TextureDesc m_textureDesc;
		TextureData m_textureData;
	};
}
