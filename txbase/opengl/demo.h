#pragma once

#include "opengl.h"
#include <GLFW/glfw3.h>

namespace TX {
	namespace GL {
		class Demo {
		public:
			Program program;
			VertexBuffer vbo;
			VertexArray vao;
		public:
			Demo() {
				GLfloat verts[] = {
					-0.4f, -0.3f, 0.0f,
					0.4f, -0.3f, 0.0f,
					0.0f,  0.4f, 0.0f
				};
				vao.Bind();
				vbo.Bind();
				vbo.Data(sizeof(verts), verts);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
				glEnableVertexAttribArray(0);
				vao.Unbind();

				Shader vs(GL_VERTEX_SHADER,
				R"(
					#version 330 core
					layout (location = 0) in vec3 position;

					#define clamp01(x) clamp(x, 0.0, 1.0)
					#define PI 3.1415926

					out vec4 vertexColor;

					uniform float iGlobalTime;

					void main()
					{
						gl_Position = vec4(position, 1.0);
						vertexColor = vec4(
							clamp01(position + sin(iGlobalTime * PI)),
							1.0);
					}
				)");
				Shader fs(GL_FRAGMENT_SHADER,
				R"(
					#version 330 core
					in vec4 vertexColor;
					out vec4 color;
					void main()
					{
						color = vertexColor;
					}
				)");
				program.Attach(vs);
				program.Attach(fs);
				program.Link();
				program.Detach(vs);
				program.Detach(fs);
			}

			inline void Draw(){
				program.Use();
				program.SetUniform("iGlobalTime", (float)glfwGetTime());
				vao.Bind();
				glDrawArrays(GL_TRIANGLES, 0, 3);
				vao.Unbind();
			}
		};
	}
}