#pragma once
#include "OpenGL/glTexture.h"


namespace GLCore
{
	enum class FBAttachmentType
	{
		ColorAttachment = 0,
		HDRColorAttachment,
		DepthAttachment,
		StencilAttachment,
		DepthStencilAttachment,
	};

	class GLFrameBuffer
	{
	public:
		GLFrameBuffer();
		virtual ~GLFrameBuffer();

		void bindFBO() const;
		void unbindFBO() const;
		void bindTextures() const;
		// void unbindTextures() const;

		// 如果你不需要从一个缓冲中采样数据，就用 RBO；反之则选择纹理附件。
		void addRBOAttachment(FBAttachmentType attachType, int width = 1024, int height = 1024);
		void addTextureAttachment(FBAttachmentType attachType, int width = 1024, int height = 1024, 
								  int glTexFilterParam = GL_LINEAR, int glTexWrapParam = GL_CLAMP_TO_EDGE,
								  int glDataType = -1);
		void updateCubeMapTexAttachment(unsigned int cubeMapID, int cubeMapSourceType, 
										FBAttachmentType attachType = FBAttachmentType::ColorAttachment,
										int width = 1024, int height = 1024);

		inline int getColorAttachmentCount() const
		{
			int cnt = 0;
			for (auto& attachment : m_textureAttachList)
			{
				if (attachment.first == FBAttachmentType::ColorAttachment ||
					attachment.first == FBAttachmentType::HDRColorAttachment)
					++cnt;
			}
			return cnt;
		}
		inline std::vector<GLTexture*> getTextures(FBAttachmentType attachType = FBAttachmentType::ColorAttachment) const
		{
			std::vector<GLTexture*> textures;
			for (auto& texture : m_textureAttachList)
			{
				if (texture.first == attachType)
				{
					textures.push_back(texture.second.get());
				}
			}
			return textures;
		}
		inline std::vector<unsigned int> getRBOs(FBAttachmentType attachType = FBAttachmentType::DepthAttachment) const
		{
			// todo: std::vector<std::pair<FBAttachmentType, unsigned int>> m_rboAttachList
			return m_rboAttachList;
		}

	private:
		static int getGLAttachmentValue(FBAttachmentType attachType);
		static std::pair<int, int> getGLComponentValue(FBAttachmentType attachType);
		static int getGLType(FBAttachmentType attachType);

	private:
		unsigned int m_rendererID;
		std::vector<std::pair<FBAttachmentType, std::unique_ptr<GLTexture>>> m_textureAttachList;
		std::vector<unsigned int> m_rboAttachList;
	};
}
