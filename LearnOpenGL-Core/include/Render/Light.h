#pragma once

#include "pch.hpp"
#include "Core/Object.h"

class Light : public GLObject
{
public:
	Light(glm::vec3 position = glm::vec3(0.0f));
	virtual ~Light() override;

	void onImGuiRender(const std::string& ObjectName) override;

protected:
	// todo: 光源的属性

};

