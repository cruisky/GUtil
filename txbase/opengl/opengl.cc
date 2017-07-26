#include "txbase/stdafx.h"
#include "opengl.h"
#include "txbase/math/vector.h"
#include "txbase/math/matrix.h"
#include "txbase/sys/memory.h"
#include "txbase/sys/tools.h"
#include <map>

namespace TX
{
	namespace GL
	{
		void SetUniform(GLuint loc, const Vec2& v) { glUniform2fv(loc, 1, v.data); }
		void SetUniform(GLuint loc, const Vec3& v) { glUniform3fv(loc, 1, v.data); }
		void SetUniform(GLuint loc, const Vec4& v) { glUniform4fv(loc, 1, v.data); }
		void SetUniform(GLuint loc, const Color& v) { glUniform4fv(loc, 1, v.v); }
		void SetUniform(GLuint loc, const Matrix3x3& v, bool transpose) { glUniformMatrix3fv(loc, 1, transpose, v); }
		void SetUniform(GLuint loc, const Matrix4x4& v, bool transpose) { glUniformMatrix4fv(loc, 1, transpose, v); }

		Shader::Shader(const std::string& file, GLenum type) try : Shader(type, ReadAllLines(file)) {}
			catch (const std::runtime_error&) {
				std::cerr << "Shader file: " << file << std::endl;
				throw;
			}
		Shader::Shader(GLenum type, const std::string& src){
			id = glCreateShader(type);
			const char *srcStr = src.c_str();
			glShaderSource(id, 1, &srcStr, NULL);
			glCompileShader(id);
			GLint status = GL_FALSE;
			glGetShaderiv(id, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE){
				std::cerr << GetLog() << std::endl;
				throw std::runtime_error("Shader failed to compile: \n" + src);
			}
		}
		Shader::~Shader(){ if(id) glDeleteShader(id); }
		std::string Shader::GetLog(){
			GLint length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			GLchar *buffer = new GLchar[length];
			glGetShaderInfoLog(id, length, NULL, buffer);
			std::string log(buffer);
			delete[] buffer;
			return log;
		}

		struct Texture2D::Impl {
			std::shared_ptr<Texture2D> BLACK;
			std::shared_ptr<Texture2D> WHITE;
		};
		const std::unique_ptr<Texture2D::Impl> Texture2D::sp(new Texture2D::Impl);
		std::shared_ptr<Texture2D> Texture2D::GetBlack(){
			if(!sp->BLACK){
				sp->BLACK = std::make_shared<Texture2D>();
				sp->BLACK->Data(&Color::BLACK, 1, 1);
				sp->BLACK->Unbind();
			}
			return sp->BLACK;
		}
		std::shared_ptr<Texture2D> Texture2D::GetWhite(){
			if(!sp->WHITE){
				sp->WHITE = std::make_shared<Texture2D>();
				sp->WHITE->Data(&Color::WHITE, 1, 1);
				sp->WHITE->Unbind();
			}
			return sp->WHITE;
		}

		struct Program::Impl {
			std::map<std::string, std::shared_ptr<ITexture>> textures;
		};
		Program::Program(): p(new Impl){ id = glCreateProgram(); }
		Program::Program(Program&& that) :
			Object(std::move(that)),
			p(std::move(that.p)) {}
		Program::~Program(){ if(id) glDeleteProgram(id); }
		void Program::Use() const {
			glUseProgram(id);

			// bind all textures
			int texUnitId = 0;
			for (auto const& pair : p->textures){
				SetUniform(pair.first.c_str(), texUnitId);
				glActiveTexture(GL_TEXTURE0 + texUnitId);
				pair.second->Bind();
				texUnitId ++;
			}
		}
		void Program::Attach(const Shader& shader){
			glAttachShader(id, shader);
		}
		void Program::Detach(const Shader& shader){
			glDetachShader(id, shader);
		}
		void Program::Link(){
			glLinkProgram(id);
			GLint status = GL_FALSE;
			glGetProgramiv(id, GL_LINK_STATUS, &status);
			if (status == GL_FALSE){
				std::clog << GetLog() << std::endl;
				throw "Failed to link program";
			}
			glValidateProgram(id);
			status = GL_FALSE;
			glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
			if (status == GL_FALSE){
				std::clog << GetLog() << std::endl;
				throw "Failed to validate program";
			}
		}

		void Program::SetTexture(const char *name, std::shared_ptr<ITexture> tex) {
			if (GetUniformLoc(name) != -1)
				p->textures.insert(std::make_pair(std::string(name), tex));
		}

		void Program::BindAttribLoc(const char *name, GLuint index){ glBindAttribLocation(id, index, name); }
		std::string Program::GetLog(){
			GLint length;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			GLchar *buffer = new GLchar[length];
			glGetProgramInfoLog(id, length, NULL, buffer);
			std::string log(buffer);
			delete[] buffer;
			return log;
		}

		static std::shared_ptr<Texture2D> GetBlack();
		static std::shared_ptr<Texture2D> GetWhite();

		void Mesh::Upload(const TX::Mesh& mesh){
			vao.Bind();
			if (mesh.vertices.size() > 0){
				vertices.Data(mesh.vertices.size() * sizeof(mesh.vertices[0]), mesh.vertices.data());
				glEnableVertexAttribArray(ATTRIB_POS);
				glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			}
			if (mesh.normals.size() > 0){
				normals.Data(mesh.normals.size() * sizeof(mesh.normals[0]), mesh.normals.data());
				glEnableVertexAttribArray(ATTRIB_NORMAL);
				glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			}
			if (mesh.uv.size() > 0){
				uvs.Data(mesh.uv.size() * sizeof(mesh.uv[0]), mesh.uv.data());
				glEnableVertexAttribArray(ATTRIB_TEXCOORD);
				glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			}
			if (mesh.indices.size() > 0){
				indices.Data(mesh.indices.size() * sizeof(mesh.indices[0]), mesh.indices.data());
			}
			vao.Unbind();
		}

		void Mesh::Draw() const {
			// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
			// https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices

			vao.Bind();
			int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glDrawElements(
				GL_TRIANGLES, 			// mode
				size / sizeof(uint32_t),	// count
				GL_UNSIGNED_INT, 		// type
				(void *)0				// offset
			);
			vao.Unbind();
		}
	}
}
