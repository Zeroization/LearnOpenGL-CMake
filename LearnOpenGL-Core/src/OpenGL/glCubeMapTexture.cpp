#include "OpenGL/glCubeMapTexture.h"

#include "OpenGL/glDebug.h"
#include "stbi/stb_image.h"

namespace GLCore
{
	GLCubeMapTexture::GLCubeMapTexture(const std::vector<CubeMapTexDesc>& descList)
		: GLTexture()
	{
		if (descList.size() != 6)
		{
			LOG_ERROR("[CubeMapTexture](): DescList's size != 6!");
			__debugbreak();
		}
		m_cubeTexData.reserve(6);

		// 排序CubeMap, 以便顺利加载
		m_cubeTexDesc = descList;
		std::sort(m_cubeTexDesc.begin(), m_cubeTexDesc.end(),
		          [](auto& v1, auto&v2)
		          {
			          return v1.dir < v2.dir;
		          });

		// 加载贴图
		unsigned int cubeMapID;
		GLCall(glGenTextures(1, &cubeMapID));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID));

		for (int i = 0; i < m_cubeTexDesc.size(); ++i)
		{
			int width, height, nrChannels;
			unsigned char* texBuffers = stbi_load(m_cubeTexDesc.at(i).filePath.c_str(), &width, &height, &nrChannels, 0);
			if (texBuffers)
			{
				GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texBuffers));
				m_cubeTexData.emplace_back(texBuffers, width, height, nrChannels);
			}
			else
			{
				LOG_ERROR(std::format("[CubeMapTexture](): Reading texture[{}] error!", i));
			}
		}

		// 设定贴图 
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		m_rendererID = cubeMapID;
	}

	GLCubeMapTexture::~GLCubeMapTexture()
	{
		// 删除CPU缓存的纹理
		for (auto& data : m_cubeTexData)
		{
			if (data.localBuffer)
			{
				stbi_image_free(data.localBuffer);
				data.localBuffer = nullptr;
			}
		}
		// 删除GPU缓存的纹理
		GLCall(glDeleteTextures(1, &m_rendererID));

		m_cubeTexData.clear();
		m_cubeTexDesc.clear();
	}

	void GLCubeMapTexture::bind(unsigned slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));
	}

	void GLCubeMapTexture::unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
}
