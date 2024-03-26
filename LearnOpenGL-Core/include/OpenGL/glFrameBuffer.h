#pragma once
#include "glTexture.h"

namespace GLCore
{
	enum class FBAttachmentType
	{
		ColorAttachment = 0,
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
		void unbindTextures() const;

		// 如果你不需要从一个缓冲中采样数据，就用 RBO；反之则选择纹理附件。
		void addRBOAttachment(FBAttachmentType attachType, int width = 1024, int height = 1024);
		void addTextureAttachment(FBAttachmentType attachType, int width = 1024, int height = 1024, 
								  int glTexFilterParam = GL_LINEAR, int glTexWrapParam = GL_CLAMP_TO_EDGE,
								  int glDataType = -1);

		inline int getColorAttachmentCount() const
		{
			int cnt = 0;
			for (auto& attachment : m_textureAttachList)
			{
				if (attachment.first == FBAttachmentType::ColorAttachment)
					++cnt;
			}
			return cnt;
		}

	private:
		static int getGLAttachmentValue(FBAttachmentType attachType);
		static int getGLComponentValue(FBAttachmentType attachType);
		static int getGLType(FBAttachmentType attachType);

	private:
		unsigned int m_rendererID;
		std::vector<std::pair<FBAttachmentType, std::unique_ptr<GLTexture>>> m_textureAttachList;
		std::vector<unsigned int> m_rboAttachList;
	};
}
