 #pragma once

#include "pch.hpp"
#include "OpenGL/glDebug.h"
#include "OpenGL/glVertexArray.h"
#include "OpenGL/glIndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	void draw(const GLVertexArray& va, const GLIndexBuffer& ib, const Shader& shader) const;
	void clear() const;
};