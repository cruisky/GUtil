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
			ATTRIB_TEXCOORD,
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
			inline void Bind() const { glBindVertexArray(id); }
			inline static void Unbind() { glBindVertexArray(0); }
		};

		class Shader : public Object {
		public:
			Shader(GLenum type, const std::string& source);
			Shader(const std::string& file, GLenum type);
			Shader(Shader&& that) : Object(std::move(that)){}
			~Shader();

			std::string GetLog();
		};

		class Texture : public Object {
		public:
			// GL_TEXTURE_2D only
			GLenum type = GL_TEXTURE_2D;
			// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
			GLenum wrap = GL_REPEAT;
			// GL_LINEAR, GL_NEAREST
			GLenum filter = GL_LINEAR;
			Texture() { glGenTextures(1, &id); }
			Texture(Texture&& that) : Object(std::move(that)){}
			~Texture() { glDeleteTextures(1, &id); }
			inline void Bind() const { glBindTexture(type, id); }
			inline void Unbind() const { glBindTexture(type, 0); }
			inline void Data(const Color *image, int width, int height) {
				Bind();
				glTexImage2D(GL_TEXTURE_2D,
					0,			// mipmap level
					GL_RGB,		// target texture format
					width,
					height,
					0,			// (legacy) border
					GL_RGBA,	// source format
					GL_FLOAT,	// source data type
					image		// source data
				);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		};

		class Program : public Object {
		private:
			struct Impl;
			const std::unique_ptr<Impl> p;
		public:
			Program();
			Program(Program&& that);
			~Program();
			void Use() const;
			void Link();
			void Attach(const Shader& shader);
			void Detach(const Shader& shader);

			template <class T, class... Ts>
			inline void Attach(const T& first, const Ts&... rest){
				Attach(first);
				Attach(rest...);
			}

			template <class T, class... Ts>
			inline void Detach(const T& first, const Ts&... rest){
				Detach(first);
				Detach(rest...);
			}

			template <class T, class... Ts>
			inline void Compile(const T& first, const Ts&... rest){
				Attach(first);
				Attach(rest...);
				Link();
				Detach(first);
				Detach(rest...);
			}

			inline GLuint GetUniformLoc(const char *name) const { return glGetUniformLocation(id, name); }

			template<typename T>
			inline void SetUniform(const char *name, T v) const { GL::SetUniform(GetUniformLoc(name), v); }
			inline void SetUniform(const char *name, const Matrix3x3& v, bool transpose) const { GL::SetUniform(GetUniformLoc(name), v, transpose); }
			inline void SetUniform(const char *name, const Matrix4x4& v, bool transpose) const { GL::SetUniform(GetUniformLoc(name), v, transpose); }

			void SetTexture(const char *name, std::shared_ptr<Texture> tex);

			void BindAttribLoc(const char *name, GLuint index);
			std::string GetLog();
		};

		class Mesh {
		public:
			VertexBuffer vertices;
			VertexBuffer normals;
			VertexBuffer uvs;
			IndexBuffer indices;
			VertexArray vao;
		public:
			Mesh(){}
			void Upload(const TX::Mesh& mesh);
			void Draw() const;
		};
	}
}
