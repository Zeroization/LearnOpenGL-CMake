#pragma once
#include "glTexture.h"

namespace GLCore
{
	enum class CubeMapTexDir
	{
		Right = 0,
		Left,
		Top,
		Bottom,
		Front,
		Back
	};

	struct CubeMapTexDesc : TextureDesc
	{
		CubeMapTexDir dir;

		CubeMapTexDesc(const std::string& filePath_, CubeMapTexDir dir_, bool needVerticalFlip_)
			: TextureDesc(filePath_, TextureType::CubeMap, needVerticalFlip_), dir(dir_) {}
	};

	class GLCubeMapTexture : public GLTexture
	{
	public:
		GLCubeMapTexture(const std::vector<CubeMapTexDesc>& descList);
		~GLCubeMapTexture() override;

		void bind(unsigned slot) const override;
		void unbind() const override;

	private:
		std::vector<CubeMapTexDesc> m_cubeTexDesc;
		std::vector<TextureData> m_cubeTexData;
	};
}