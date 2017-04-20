#pragma once
// IMPORTANT: include this header before any gl.h or windows.h, as required by gl3w.h

#include "txbase/fwddecl.h"
#include <GL/gl3w.h>
#include "txbase/shape/obj.h"
#include "txbase/sys/memory.h"

namespace TX
{
	namespace GL
	{
		enum Attribute {
			ATTRIB_POS,
			ATTRIB_NORMAL,
		};

		inline void SetUniform(GLuint loc, int v) { glUniform1i(loc, v); }
		inline void SetUniform(GLuint loc, float v) { glUniform1f(loc, v); }
		void SetUniform(GLuint loc, const Vec2& v);
		void SetUniform(GLuint loc, const Vec3& v);
		void SetUniform(GLuint loc, const Vec4& v);
		void SetUniform(GLuint loc, const Color& v);
		void SetUniform(GLuint loc, const Matrix3x3& v, bool transpose = true);
		void SetUniform(GLuint loc, const Matrix4x4& v, bool transpose = true);

		class Object : public NonCopyable {
		public:
			GLuint id;
			Object(): id(0){}
			Object(Object&& that){
				id = that.id;
				that.id = 0;
			}
			inline operator GLuint() const { return id; }
		};

		template<GLenum Target>
		class Buffer : public Object {
		public:
			Buffer(){ glGenBuffers(1, &id); }
			Buffer(Buffer&& that) : Object(std::move(that)){}
			~Buffer(){ if(id) glDeleteBuffers(1, &id); }
			inline void Data(GLsizeiptr size, const void *data){
				Bind();
				glBufferData(Target, size, data, GL_STATIC_DRAW);
			}
			inline void Bind() const { glBindBuffer(Target, id); }
			inline static void Unbind() { glBindBuffer(Target, 0); }
		};
		typedef Buffer<GL_ARRAY_BUFFER> VertexBuffer;
		typedef Buffer<GL_ELEMENT_ARRAY_BUFFER> IndexBuffer;
		typedef Buffer<GL_UNIFORM_BUFFER> UniformBuffer;
		typedef Buffer<GL_TEXTURE_BUFFER> TextureBuffer;

		class VertexArray : public Object {
		public:
			VertexArray() { glGenVertexArrays(1, &id); }
			VertexArray(VertexArray&& that) : Object(std::move(that)){}
			~VertexArray() { if(id) glDeleteVertexArrays(1, &id); }
			inline void Bind() { glBindVertexArray(id); }
			inline static void Unbind() { glBindVertexArray(0); }
		};

		class Shader : public Object {
		public:
			Shader(GLenum type, const char *source);
			Shader(Shader&& that) : Object(std::move(that)){}
			~Shader();

			std::string GetLog();
		};

		class Program : public Object {
		public:
			Program();
			Program(Program&& that) : Object(std::move(that)){}
			~Program();
			void Use() const;
			void Attach(const Shader& shader);
			void Detach(const Shader& shader);

			inline GLuint GetUniformLoc(const char *name) const { return glGetUniformLocation(id, name); }

			template<typename T>
			inline void SetUniform(const char *name, T v) const { GL::SetUniform(GetUniformLoc(name), v); }
			inline void SetUniform(const char *name, const Matrix3x3& v, bool transpose) const { GL::SetUniform(GetUniformLoc(name), v, transpose); }
			inline void SetUniform(const char *name, const Matrix4x4& v, bool transpose) const { GL::SetUniform(GetUniformLoc(name), v, transpose); }

			void BindAttribLoc(const char *name, GLuint index);
			void Link();
			std::string GetLog();
		};

		class Mesh {
		public:
			VertexBuffer vertices;
			VertexBuffer normals;
			IndexBuffer indices;
		public:
			Mesh(){}
			void Upload(const TX::Mesh& mesh);
			void Draw() const;
		};
	}
}
