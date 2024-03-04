#pragma once

#include "imgui/imgui.h"

#include "pch.hpp"
#include "OpenGL/glDebug.h"
#include "OpenGL/glVertexArray.h"
#include "OpenGL/glIndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer(GLFWwindow* window);
	virtual ~Renderer();

	void draw(const GLVertexArray& va, const GLIndexBuffer& ib, const Shader& shader) const;
	void clear() const;

	void imGuiOnAttach() const;
	void imGuiOnDetach() const;
	void imGuiNewFrame() const;
	void imGuiRender() const;

private:
	GLFWwindow* m_window = nullptr;
	std::string m_resPath;
};