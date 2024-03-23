#include "OpenGL/glTexture.h"
#include "OpenGL/glDebug.h"
#include "stbi/stb_image.h"

namespace GLCore
{
	GLTexture::GLTexture(const std::string& filePath, TextureType texType, bool needVerticalFlip)
		: m_rendererID(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0)
	{
		m_textureData = {filePath, texType, needVerticalFlip};
		// ��ת������������
		stbi_set_flip_vertically_on_load(needVerticalFlip);

		// ��������
		m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_bpp, 4);
		if (m_localBuffer)
		{
			// ��������
			GLCall(glGenTextures(1, &m_rendererID));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
			// �������� todo�� �������������
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
			// �������
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}
		else
		{
			LOG_ERROR("[Texture] read texture file ERROR!!");
		}
	}

	GLTexture::GLTexture(const TextureData& textureData)
		: GLTexture(textureData.filePath, textureData.texType, textureData.needVerticalFlip)
	{
		m_textureData = textureData;
	}

	GLTexture::~GLTexture()
	{
		// ɾ��CPU���������
		if (m_localBuffer)
		{
			stbi_image_free(m_localBuffer);
			m_localBuffer = nullptr;
		}
		// ɾ��GPU���������
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
