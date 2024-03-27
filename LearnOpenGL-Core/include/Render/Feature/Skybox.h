#pragma once

#include "Core/Object.h"
#include "OpenGL/glCubeMapTexture.h"

namespace GLCore
{
	class SkyBox : public GLObject
	{
	public:
		SkyBox(const std::vector<GLCore::CubeMapTexDesc>& cubeTextures);

		// void bind() const override;
		// void unbind() const override;

		// void onRender(const Renderer& renderer) override;
		void onImGuiRender(const std::string& ObjectName) override;
		void onUpdate() override;

	private:
		std::shared_ptr<GLCore::GLCubeMapTexture> m_cubeMapTexture;
	};

}
