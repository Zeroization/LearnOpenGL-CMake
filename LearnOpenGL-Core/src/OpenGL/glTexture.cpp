#include "OpenGL/glTexture.h"
#include "OpenGL/glDebug.h"
#include "stbi/stb_image.h"

namespace GLCore
{
	GLTexture::GLTexture(const std::string& filePath, TextureType texType, bool needVerticalFlip)
		: m_rendererID(0), m_textureDesc(filePath, texType, needVerticalFlip)
	{
		// 翻转纹理上下坐标
		stbi_set_flip_vertically_on_load(needVerticalFlip);

		// 加载纹理
		m_textureData.localBuffer = stbi_load(filePath.c_str(),
											  &m_textureData.width, &m_textureData.height, 
											  &m_textureData.nrChannels, 4);
		if (m_textureData.localBuffer)
		{
			// 生成纹理
			GLCall(glGenTextures(1, &m_rendererID));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
			// 配置纹理
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureData.width, m_textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData.localBuffer));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
			// 解绑纹理
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}
		else
		{
			LOG_ERROR("[Texture] read texture file ERROR!!");
		}
	}

	GLTexture::GLTexture(const TextureDesc& textureData)
		: GLTexture(textureData.filePath, textureData.texType, textureData.needVerticalFlip)
	{
	}

	GLTexture::GLTexture(unsigned id, int width, int height)
		: m_rendererID(id), m_textureData(nullptr, width, height, 0)
	{
	}

	GLTexture::~GLTexture()
	{
		// 删除CPU缓存的纹理
		if (m_textureData.localBuffer)
		{
			stbi_image_free(m_textureData.localBuffer);
			m_textureData.localBuffer = nullptr;
		}
		// 删除GPU缓存的纹理
		GLCall(glDeleteTextures(1, &m_rendererID));
	}

	void GLTexture::bind(unsigned slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
	}

	void GLTexture::unbind() const
	{
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
