//
// Created by adamyuan on 4/14/18.
//

#ifndef MYGL2_TEXTURE_HPP
#define MYGL2_TEXTURE_HPP

#include <GL/gl3w.h>
#include <cassert>
#include <cstdio>

namespace mygl3
{
	struct ImageInfo
	{
		GLsizei width, height, depth;
		GLenum internal_format, format, type;
		GLvoid *data;
		ImageInfo()
				: width(1), height(1), depth(1), internal_format(GL_RGBA), format(GL_RGBA), type(GL_UNSIGNED_BYTE),
				  data(nullptr) {}
		ImageInfo(GLsizei width, GLsizei height, GLsizei depth, GLenum internal_format, GLenum format,
				  GLenum type, GLvoid *data)
				: width(width), height(height), depth(depth), internal_format(internal_format), format(format),
				  type(type), data(data) {}
	};

	class ImageLoader
	{
	private:
		ImageInfo info_;
	public:
		explicit ImageLoader(const char *filename);
		explicit ImageLoader(const char *filename, GLsizei depth);
		const ImageInfo &GetInfo() const { return info_; }
		~ImageLoader();
	};

	template <GLenum T>
	class Texture
	{
	private:
		GLuint tex_;
	public:
		Texture() { tex_ = 0; }
		Texture& operator= (const Texture&) = delete;
		Texture(Texture &&texture) noexcept : tex_(texture.tex_)
		{
			texture.tex_ = 0;
		}
		~Texture()
		{
			if(tex_ != 0)
				glDeleteTextures(1, &tex_);
		}
		Texture (const Texture&) = delete;
		Texture& operator= (Texture &&texture) noexcept
		{
			assert(tex_ == 0);
			tex_ = texture.tex_;
			texture.tex_ = 0;
			return *this;
		}
#define IS_1D (T == GL_TEXTURE_1D || T == GL_PROXY_TEXTURE_1D)
#define IS_1D_ARRAY (T == GL_TEXTURE_1D_ARRAY || T == GL_PROXY_TEXTURE_1D_ARRAY)
#define IS_2D (T == GL_TEXTURE_2D || T == GL_PROXY_TEXTURE_2D || T == GL_TEXTURE_1D_ARRAY || \
	T == GL_PROXY_TEXTURE_1D_ARRAY || T == GL_TEXTURE_RECTANGLE || T == GL_PROXY_TEXTURE_RECTANGLE)
#define IS_2D_ARRAY (T == GL_TEXTURE_2D_ARRAY || T == GL_PROXY_TEXTURE_2D_ARRAY)
#define IS_3D (T == GL_TEXTURE_3D || T == GL_PROXY_TEXTURE_3D || T == GL_TEXTURE_2D_ARRAY || \
	T == GL_PROXY_TEXTURE_2D_ARRAY)

		void Initialize()
		{
			assert(tex_ == 0);
			glCreateTextures(T, 1, &tex_);
		}

		void Bind() const
		{
			assert(tex_ != 0);
			glBindTexture(T, tex_);
		}
		void Bind(GLuint unit) const
		{
			assert(tex_ != 0);
			glBindTextureUnit(unit, tex_);
		}

		static GLsizei GetLevel1D(GLsizei w)
		{
			int levels = 1;
			while(w >> levels) levels ++;
			return levels;
		}
		static GLsizei GetLevel2D(GLsizei w, GLsizei h)
		{
			int levels = 1;
			while((w | h) >> levels) levels ++;
			return levels;
		}
		static GLsizei GetLevel3D(GLsizei w, GLsizei h, GLsizei d)
		{
			int levels = 1;
			while((w | h | d) >> levels) levels ++;
			return levels;
		}

		void Load(const ImageInfo &info, GLsizei levels)
		{
			assert(tex_ != 0);
			if (IS_1D)
			{
				glTextureStorage1D(tex_, levels, info.internal_format, info.width);
				if(info.data) glTextureSubImage1D(tex_, 0, 0, info.width, info.format, info.type, info.data);
			}
			else if (IS_2D)
			{
				glTextureStorage2D(tex_, levels, info.internal_format, info.width, info.height);
				if(info.data) glTextureSubImage2D(tex_, 0, 0, 0, info.width, info.height, info.format, info.type, info.data);
			}
			else if (IS_3D)
			{
				glTextureStorage3D(tex_, levels, info.internal_format, info.width, info.height, info.depth);
				if(info.data) glTextureSubImage3D(tex_, 0, 0, 0, 0, info.width, info.height, info.depth, info.format, info.type, info.data);
			}
			//if(mipmap)
			//	glGenerateTextureMipmap(tex_);
		}

		void Load(const ImageInfo &info, bool mipmap = false)
		{
			GLsizei levels = 1;
			if(mipmap)
			{
				if(IS_1D || IS_1D_ARRAY) levels = GetLevel1D(info.width);
				else if(IS_2D || IS_2D_ARRAY) levels = GetLevel2D(info.width, info.height);
				else if(IS_3D) levels = GetLevel3D(info.width, info.height, info.depth);
			}
			Load(info, levels);
		}

		void GenerateMipmap()
		{
			glGenerateTextureMipmap(tex_);
		}

		void SetWrapFilter(GLenum filter)
		{
			if (IS_1D)
				glTextureParameteri(tex_, GL_TEXTURE_WRAP_S, filter);
			else if (IS_2D)
			{
				glTextureParameteri(tex_, GL_TEXTURE_WRAP_S, filter);
				glTextureParameteri(tex_, GL_TEXTURE_WRAP_T, filter);
			}
			else if (IS_3D)
			{
				glTextureParameteri(tex_, GL_TEXTURE_WRAP_S, filter);
				glTextureParameteri(tex_, GL_TEXTURE_WRAP_T, filter);
				glTextureParameteri(tex_, GL_TEXTURE_WRAP_R, filter);
			}
		}

		void SetSizeFilter(GLenum min_filter, GLenum mag_filter)
		{
			glTextureParameteri(tex_, GL_TEXTURE_MIN_FILTER, min_filter);
			glTextureParameteri(tex_, GL_TEXTURE_MAG_FILTER, mag_filter);
		}

		GLuint Get() const { return tex_; }
	};
	using Texture1D = Texture<GL_TEXTURE_1D>;
	using Texture1DArray = Texture<GL_TEXTURE_1D_ARRAY>;
	using Texture2D = Texture<GL_TEXTURE_2D>;
	using Texture2DArray = Texture<GL_TEXTURE_2D_ARRAY>;
	using Texture3D = Texture<GL_TEXTURE_3D>;
}

#endif //MYGL2_TEXTURE_HPP
