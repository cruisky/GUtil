#pragma once
// IMPORTANT: include this header before any gl.h or windows.h, as required by gl3w.h

#include "txbase/fwddecl.h"
#include <GL/gl3w.h>
#include <GL/glu.h>
#include "txbase/shape/obj.h"
#include "txbase/sys/memory.h"
#include "txbase/image/image.h"

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
			inline operator GLuint() const { return id; }
		protected:
			Object(): id(0){}
			Object(Object&& that){
				id = that.id;
				that.id = 0;
			}
		};

		class Error: public std::runtime_error
		{
		public:
			GLenum code;
		public:
			Error(GLenum code): code(code), runtime_error(GetErrorString(code)) {}
		public:
			static std::string GetErrorString(GLenum code) {
				return (char *) gluErrorString(code);
			}
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

		public:
			const std::string source;
		};

		class ITexture {
		public:
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;
		protected:
			ITexture(){}
		};

		template<GLenum Type>
		class Texture : public Object, public ITexture {
		public:
			// // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
			// GLenum wrap = GL_REPEAT;
			// // GL_LINEAR, GL_NEAREST
			// GLenum filter = GL_LINEAR;
			inline void Bind() const { glBindTexture(Type, id); }
			inline void Unbind() const { glBindTexture(Type, 0); }

			void Parameter(GLenum name, int value){ glTexParameteri(Type, name, value); }
		protected:
			Texture() { glGenTextures(1, &id); }
			Texture(Texture&& that) : Object(std::move(that)){}
			~Texture() { glDeleteTextures(1, &id); }
		};

		class Texture2D: public Texture<GL_TEXTURE_2D> {
		private:
			struct Impl;
			static const std::unique_ptr<Impl> sp;
		public:
			using Texture::Texture;
			static std::shared_ptr<Texture2D> GetBlack();
			static std::shared_ptr<Texture2D> GetWhite();
			inline void Data(const Color *image, int width, int height, bool genMipmap = true) {
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
				Parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				if (genMipmap) {
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
		};

		class Cubemap: public Texture<GL_TEXTURE_CUBE_MAP> {
		public:
			using Texture::Texture;

			// 0	GL_TEXTURE_CUBE_MAP_POSITIVE_X	right
			// 1	GL_TEXTURE_CUBE_MAP_NEGATIVE_X	left
			// 2	GL_TEXTURE_CUBE_MAP_POSITIVE_Y	top
			// 3	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	bottom
			// 4	GL_TEXTURE_CUBE_MAP_POSITIVE_Z	front
			// 5	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	back
			inline void Data(const Image faces[6]) {
				Bind();
				for (GLuint i = 0; i < 6; i++){
					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0,
						GL_RGB,
						faces[i].Width(),
						faces[i].Height(),
						0,
						GL_RGBA,
						GL_FLOAT,
						faces[i].Data()
					);
				}
				Parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				Parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				Parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				Parameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}

			//      +----+
			//      | TP |
			// +----+    +----+----+
			// | LT   FT   RT   BK |
			// +----+    +----+----+
			//      | BT |
			//      +----+
			inline void Data_HorizontalCross(const Image map) {
				assert(map.Width() % 4 == 0);
				assert(map.Height() % 3 == 0);
				assert(map.Width() / 4 == map.Height() / 3);

				static const Vec2i FACE_MAP[] = {
					Vec2i(2, 1),	// RT
					Vec2i(0, 1),	// LT
					Vec2i(1, 0),	// TP
					Vec2i(1, 2),	// BT
					Vec2i(1, 1),	// FT
					Vec2i(3, 1),	// BK
				};
				const int faceSize = map.Width() / 4;
				const int stride = map.Width();
				std::vector<Image> faces;
				for (int i = 0; i < 6; i++) {
					faces.emplace_back(faceSize, faceSize);
					Image& face = faces.back();

					Vec2i faceBegin = FACE_MAP[i] * faceSize;
					Vec2i faceEnd = faceBegin + faceSize;

					const Color *srcRow = map.Data() + faceBegin.y * map.Width() + faceBegin.x;
					Color *dstRow = face.Data();

					for (int y = faceBegin.y; y < faceEnd.y; y++) {
						memcpy(dstRow, srcRow, faceSize * sizeof(Color));
						srcRow += stride;
						dstRow += faceSize;
					}
				}
				return Data(faces.data());
			}
		};

		// Usually as stencil/depth buffer attachment.
		template <GLenum AttachmentType>
		class Renderbuffer : public Object {
		public:
			const GLenum InternalFormat;
		public:
			Renderbuffer(GLenum internalFormat, int width, int height):
				InternalFormat(internalFormat)
			{
				glGenRenderbuffers(1, &id);
				Size(width, height);
			}
			Renderbuffer(Renderbuffer&& that):
				Object(std::move(that)),
				InternalFormat(that.InternalFormat)
				{}
			~Renderbuffer(){ if(id) glDeleteRenderbuffers(1, &id); }

			inline void Bind() const { glBindRenderbuffer(GL_RENDERBUFFER, id); }
			inline void Unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

			inline void Size(int width, int height) {
				Bind();
				glRenderbufferStorage(
					GL_RENDERBUFFER,
					InternalFormat,
					width,
					height);
				Unbind();
			}

			inline void Parameter(GLenum pname) const {
				int v; Parameter(pname, &v); return v;
			}
			inline void Parameter(GLenum pname, int *params) const {
				Bind();
				glGetRenderbufferParameteriv(GL_RENDERBUFFER, pname, params);
				Unbind();
			}
		};
		typedef Renderbuffer<GL_DEPTH_STENCIL_ATTACHMENT> DepthStencilRenderbuffer;

		class Framebuffer : public Object {
		public:
			Texture2D					texture;
			DepthStencilRenderbuffer	renderbuffer;
		private:
			Vec2i size_;
		public:
			Framebuffer(int width, int height):
				size_(width, height),
				renderbuffer(GL_DEPTH24_STENCIL8, width, height)
			{
				glGenFramebuffers(1, &id);

				texture.Data(nullptr, width, height, false);

				Attach(texture);
				Attach(renderbuffer);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
					throw std::runtime_error("Framebuffer is not complete");
				}
			}
			Framebuffer(Framebuffer&& that):
				Object(std::move(that)),
				renderbuffer(std::move(that.renderbuffer)),
				texture(std::move(that.texture)),
				size_(that.size_)
				{}
			~Framebuffer(){ if(id) glDeleteFramebuffers(1, &id); }

			inline void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id); }
			inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

			inline void Attach(const Texture2D& tex){
				Bind();
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
				Unbind();
			}
			template <GLenum AttachmentType>
			inline void Attach(const Renderbuffer<AttachmentType>& buf){
				Bind();
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, AttachmentType, GL_RENDERBUFFER, buf);
				Unbind();
			}
			inline Vec2i Size() const { return size_; }
			inline void Size(int width, int height){
				size_.x = width;
				size_.y = height;

				// there is nothing required on the FBO
				// update the size of texture and render buffer
				texture.Data(nullptr, width, height);
				renderbuffer.Size(width, height);
			}
		};

		class Program : public Object {
		private:
			struct Impl;
			std::unique_ptr<Impl> p;
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

			void SetTexture(const char *name, std::shared_ptr<ITexture> tex);

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
