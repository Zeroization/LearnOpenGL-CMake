#include "OpenGL/glFrameBuffer.h"
#include "OpenGL/glDebug.h"

namespace GLCore
{
	GLFrameBuffer::GLFrameBuffer()
	{
		// 一般最多有16个
		m_textureAttachList.reserve(16);
		GLCall(glGenFramebuffers(1, &m_rendererID));
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		// 删除RBO和纹理附件
		for (auto& rbo : m_rboAttachList)
		{
			GLCall(glDeleteRenderbuffers(1, &rbo));
		}
		for (auto& [textureType, texture] : m_textureAttachList)
		{
			texture.reset();
		}
		// 最后删除FBO
		GLCall(glDeleteFramebuffers(1, &m_rendererID));
	}

	void GLFrameBuffer::bindFBO() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_ERROR("[FrameBuffer]bind(): Current FBO you binded is not complete!");
		}
	}

	void GLFrameBuffer::unbindFBO() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFrameBuffer::bindTextures() const
	{
		for (int i = 0; i < m_textureAttachList.size(); ++i)
		{
			m_textureAttachList.at(i).second->bind(i);
		}
	}

	/*void GLFrameBuffer::unbindTextures() const
	{
		for (auto& [textureType, texture] : m_textureAttachList)
		{
			texture->unbind();
		}
	}*/

	void GLFrameBuffer::addRBOAttachment(FBAttachmentType attachType, int width, int height)
	{
		// 创建并绑定一个RBO
		unsigned int RBO;
		int componentType = getGLComponentValue(attachType);
		GLCall(glGenRenderbuffers(1, &RBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, RBO));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, componentType, width, height));

		// 将RBO绑定到FBO上
		int attachmentType = getGLAttachmentValue(attachType);
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, RBO));
		m_rboAttachList.push_back(RBO);

		// 解绑RBO和FBO
		unbindFBO();
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	void GLFrameBuffer::addTextureAttachment(FBAttachmentType attachType, int width, int height, int glTexFilterParam,
											 int glTexWrapParam, int glDataType)
	{
		// 先生成一个纹理附件
		unsigned int textureAttach;
		int componentType = getGLComponentValue(attachType);
		glDataType = glDataType == -1 ? getGLType(attachType) : glDataType;
		GLCall(glGenTextures(1, &textureAttach));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureAttach));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, componentType, width, height, 0, componentType, glDataType, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glTexFilterParam));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glTexFilterParam));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glTexWrapParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glTexWrapParam);

		// 然后将该纹理附件绑定到FBO上
		int attachmentType = getGLAttachmentValue(attachType);
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
		if (attachmentType == GL_COLOR_ATTACHMENT0)
		{
			int colorAttachCnt = getColorAttachmentCount();
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType + colorAttachCnt, GL_TEXTURE_2D, textureAttach, 0));
		}
		else
		{
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, textureAttach, 0));
		}
		m_textureAttachList.emplace_back(attachType, std::make_unique<GLTexture>(textureAttach, width, height));

		// 别忘了解绑FBO
		unbindFBO();
	}

	void GLFrameBuffer::updateCubeMapTexAttachment(unsigned cubeMapID, int cubeMapSourceType, FBAttachmentType attachType,
		int width, int height)
	{
		// 将一面纹理绑定到FBO上
		int attachmentType = getGLAttachmentValue(attachType);

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, cubeMapSourceType, cubeMapID, 0));

		if (m_textureAttachList.empty())
			m_textureAttachList.emplace_back(attachType, std::make_unique<GLTexture>(cubeMapID, width, height));

		unbindFBO();
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

	int GLFrameBuffer::getGLAttachmentValue(FBAttachmentType attachType)
	{
		switch (attachType)
		{
			case FBAttachmentType::ColorAttachment:
				return GL_COLOR_ATTACHMENT0;
			case FBAttachmentType::DepthAttachment:
				return GL_DEPTH_ATTACHMENT;
			case FBAttachmentType::StencilAttachment:
				return GL_STENCIL_ATTACHMENT;
			case FBAttachmentType::DepthStencilAttachment:
				return GL_DEPTH_STENCIL_ATTACHMENT;
		}

		LOG_CRITICAL("[FrameBuffer]getGLAttachmentValue(): Unknown attachType in this func!");
		__debugbreak();

		return -1;
	}

	int GLFrameBuffer::getGLComponentValue(FBAttachmentType attachType)
	{
		switch (attachType)
		{
			case FBAttachmentType::ColorAttachment:
				return GL_RGB;
			case FBAttachmentType::DepthAttachment:
				return GL_DEPTH_COMPONENT;
			case FBAttachmentType::StencilAttachment:
				return GL_STENCIL_INDEX;
			case FBAttachmentType::DepthStencilAttachment:
				return GL_DEPTH24_STENCIL8;
		}

		LOG_CRITICAL("[FrameBuffer]getGLComponentValue(): Unknown attachType in this func!");
		__debugbreak();

		return -1;
	}

	int GLFrameBuffer::getGLType(FBAttachmentType attachType)
	{
		switch (attachType)
		{
			case FBAttachmentType::ColorAttachment:
				return GL_UNSIGNED_BYTE;
			case FBAttachmentType::DepthAttachment:
				return GL_FLOAT;
			case FBAttachmentType::StencilAttachment:
				return GL_FLOAT;
			case FBAttachmentType::DepthStencilAttachment:
				return GL_UNSIGNED_INT_24_8;
		}

		LOG_CRITICAL("[FrameBuffer]getGLType(): Unknown attachType in this func!");
		__debugbreak();

		return -1;
	}
}
