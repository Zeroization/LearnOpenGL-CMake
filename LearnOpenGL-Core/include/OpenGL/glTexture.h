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
		// һ�㹹�캯��
		GLTexture(const std::string& filePath, TextureType texType = TextureType::Unknown, bool needVerticalFlip = false);
		GLTexture(const TextureData& textureData);
		// ������FBO��ʹ�õĹ��캯��
		GLTexture(unsigned int id, int width, int height);
		~GLTexture();

		// slot: �������λ��, �����һ���԰󶨶��, PC������32, �ֻ�������8
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
