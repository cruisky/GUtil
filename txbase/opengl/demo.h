#pragma once

#include "opengl.h"
#include <GLFW/glfw3.h>
#include <txbase/math/matrix.h>
#include <txbase/shape/mesh.h>
#include <txbase/scene/camera.h>
#include <txbase/image/image.h>

namespace TX {
	namespace GL {
		class Demo2D {
		public:
			Program program;
			VertexBuffer vbo;
			VertexArray vao;
		public:
			Demo2D() {
				GLfloat verts[] = {
					-0.4f, -0.3f, 0.0f,
					0.4f, -0.3f, 0.0f,
					0.0f,  0.4f, 0.0f
				};
				vao.Bind();
				vbo.Bind();
				vbo.Data(sizeof(verts), verts);
				glEnableVertexAttribArray(ATTRIB_POS);
				glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
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
				program.Compile(vs, fs);
			}

			inline void Draw(){
				program.Use();
				program.SetUniform("iGlobalTime", (float)glfwGetTime());
				vao.Bind();
				glDrawArrays(GL_TRIANGLES, 0, 3);
				vao.Unbind();
			}
		};

		class Demo3DPlane {
		public:
			Program program;
			VertexBuffer vboVerts;
			VertexBuffer vboNorms;
			VertexArray vao;
			IndexBuffer ebo;
		public:
			Demo3DPlane()
			{
				GLfloat verts[] = {
					-1.f, -1.f, 0.0f,
					1.f, -1.f, 0.0f,
					-1.f, 1.f, 0.0f,
					1.f, 1.f, 0.0f
				};
				GLfloat normals[] = {
					-0.7f, -0.7f, 0.1f,
					0.7f, -0.7f, 0.1f,
					-0.7f, 0.7f, 0.1f,
					0.7f, 0.7f, 0.1f
				};
				GLuint indices[] = {
					0, 1, 2,
					2, 1, 3
				};
				vao.Bind();
					vboVerts.Data(sizeof(verts), verts);
					glEnableVertexAttribArray(ATTRIB_POS);
					glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
					vboNorms.Data(sizeof(normals), normals);
					glEnableVertexAttribArray(ATTRIB_NORMAL);
					glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
					ebo.Data(sizeof(indices), indices);
				vao.Unbind();

				// Shaders
				Shader vs(GL_VERTEX_SHADER,
				R"(
					#version 330 core
					layout (location = 0) in vec3 position;
					layout (location = 1) in vec3 normal;
					uniform mat4 iModel;
					uniform mat4 iView;
					uniform mat4 iProj;
					out vec3 norm;
					void main()
					{
						gl_Position = iProj * iView * iModel * vec4(position, 1.0f);
						norm = normal;
					}
				)");
				Shader fs(GL_FRAGMENT_SHADER,
				R"(
					#version 330 core
					in vec3 norm;
					out vec4 color;
					void main()
					{
						color = vec4(norm, 1.);
					}
				)");
				program.Compile(vs, fs);
			}

			inline void Draw(const Camera& camera){
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CCW);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);

				program.Use();
				program.SetUniform("iModel", Matrix4x4::IDENTITY);
				program.SetUniform("iView", camera.transform.WorldToLocalMatrix());
				program.SetUniform("iProj", camera.CameraToViewport());

				vao.Bind();
				int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
				glDrawElements(
					GL_TRIANGLES,
					size / sizeof(GLuint),
					GL_UNSIGNED_INT,
					(void *)0
				);

				vao.Unbind();
			}
		};

		class Demo3DMesh {
		public:
			Program program;
			Mesh glObj;
			Texture2D tex;
			const int texSize = 128;
		public:
			Demo3DMesh(const TX::Mesh& mesh){
				// Shape & Texture
				glObj.Upload(mesh);
				std::unique_ptr<Color[]> checkerboard(Image::LoadCheckerboard(texSize, texSize, 4));
				tex.Data(checkerboard.get(), texSize, texSize);
				tex.Unbind();

				// Shaders
				Shader vs(GL_VERTEX_SHADER,
				R"(
					#version 330 core
					layout (location = 0) in vec3 position;
					layout (location = 1) in vec3 normal;
					layout (location = 2) in vec2 texCoord;
					uniform mat4 iModel;
					uniform mat4 iView;
					uniform mat4 iProj;

					out VS_OUT {
						vec3 pos;
						vec3 norm;
						vec2 uv;
					} vs_out;

					void main()
					{
						gl_Position = iProj * iView * iModel * vec4(position, 1.0f);
						vs_out.pos = gl_Position.xyz;
						vs_out.norm = normal;
						vs_out.uv = texCoord;
					}
				)");
				Shader fs(GL_FRAGMENT_SHADER,
				R"(
					#version 330 core
					uniform sampler2D iTex2D;

					in VS_OUT {
						vec3 pos;
						vec3 norm;
						vec2 uv;
					} fs_in;

					out vec4 color;
					void main()
					{
						color = vec4((fs_in.norm+1.)*.5, 1.) * texture(iTex2D, fs_in.uv);
					}
				)");
				program.Compile(vs, fs);
			}

			void Draw(Camera& camera){
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CCW);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);

				program.Use();
				program.SetUniform("iModel", Matrix4x4::IDENTITY);
				program.SetUniform("iView", camera.transform.WorldToLocalMatrix());
				program.SetUniform("iProj", camera.CameraToViewport());
				tex.Bind();

				glObj.Draw();
			}
		};
	}
}