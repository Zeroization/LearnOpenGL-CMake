#pragma once

#include "pch.hpp"
#include "OpenGL/glVertexArray.h"
#include "OpenGL/glIndexBuffer.h"
#include "OpenGL/glShader.h"

#include "Core/Object.h"

class Renderer
{
public:
	Renderer(GLFWwindow* window);
	virtual ~Renderer();

	void draw(const GLVertexArray& va, const GLShader& shader, int pointsCount) const;
	void draw(const GLVertexArray& va, const GLIndexBuffer& ib, const GLShader& shader) const;
	void draw(const GLObject& object) const;
	void clear() const;

	void imGuiOnAttach() const;
	void imGuiOnDetach() const;
	void imGuiNewFrame() const;
	void imGuiRender() const;

private:
	GLFWwindow* m_window = nullptr;
	std::string m_resPath;
};