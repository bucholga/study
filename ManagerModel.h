#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"

namespace NManager {
	using namespace NGeometry3d;
	void load_model(const Model& model);
};