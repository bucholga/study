#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>#include <iostream>

#include "ManagerModel.h"

namespace NManager {
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	void load_model(const Model& model) {
		if (cubeVAO == 0)
		{
			const size_t cnt = 1000;// model.count_vertexes();
			float vertices[cnt * 8];
			for (size_t i = 0; i < cnt; ++i) {
				for (int j = 0; j < 3; ++j)
					vertices[8 * i + j] = model.get_vertex(i)[j];
				for (int j = 3; j < 6; ++j)
					vertices[8 * i + j] = model.get_normal(i)[j - 3];
				for (int j = 6; j < 8; ++j)
					vertices[8 * i + j] = 0.0;
			}
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(cubeVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// render Cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6 * model.count_vertexes());
		glBindVertexArray(0);

	}
};