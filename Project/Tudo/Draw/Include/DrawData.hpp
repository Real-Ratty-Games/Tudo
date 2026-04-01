/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWDATA_HPP_
#define DRAWDATA_HPP_
#include "SystemTypes.hpp"
#include "Transformation.hpp"
#if _DEBUG
#define BX_CONFIG_DEBUG 1
#else
#define BX_CONFIG_DEBUG 0
#endif
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <vector>

#define TUDO_SAMPLER_U_MIRROR			BGFX_SAMPLER_U_MIRROR
#define TUDO_SAMPLER_U_CLAMP			BGFX_SAMPLER_U_CLAMP
#define TUDO_SAMPLER_U_BORDER			BGFX_SAMPLER_U_BORDER
#define TUDO_SAMPLER_U_SHIFT			BGFX_SAMPLER_U_SHIFT
#define TUDO_SAMPLER_U_MASK				BGFX_SAMPLER_U_MASK
#define TUDO_SAMPLER_V_MIRROR			BGFX_SAMPLER_V_MIRROR
#define TUDO_SAMPLER_V_CLAMP			BGFX_SAMPLER_V_CLAMP
#define TUDO_SAMPLER_V_BORDER			BGFX_SAMPLER_V_BORDER
#define TUDO_SAMPLER_V_SHIFT			BGFX_SAMPLER_V_SHIFT
#define TUDO_SAMPLER_V_MASK				BGFX_SAMPLER_V_MASK
#define TUDO_SAMPLER_W_MIRROR			BGFX_SAMPLER_W_MIRROR
#define TUDO_SAMPLER_W_CLAMP			BGFX_SAMPLER_W_CLAMP
#define TUDO_SAMPLER_W_BORDER			BGFX_SAMPLER_W_BORDER
#define TUDO_SAMPLER_W_SHIFT			BGFX_SAMPLER_W_SHIFT
#define TUDO_SAMPLER_W_MASK				BGFX_SAMPLER_W_MASK
#define TUDO_SAMPLER_MIN_POINT			BGFX_SAMPLER_MIN_POINT
#define TUDO_SAMPLER_MIN_ANISOTROPIC	BGFX_SAMPLER_MIN_ANISOTROPIC
#define TUDO_SAMPLER_MIN_SHIFT			BGFX_SAMPLER_MIN_SHIFT
#define TUDO_SAMPLER_MIN_MASK			BGFX_SAMPLER_MIN_MASK
#define TUDO_SAMPLER_MAG_POINT			BGFX_SAMPLER_MAG_POINT
#define TUDO_SAMPLER_MAG_ANISOTROPIC	BGFX_SAMPLER_MAG_ANISOTROPIC
#define TUDO_SAMPLER_MAG_SHIFT			BGFX_SAMPLER_MAG_SHIFT
#define TUDO_SAMPLER_MAG_MASK			BGFX_SAMPLER_MAG_MASK
#define TUDO_SAMPLER_MIP_POINT			BGFX_SAMPLER_MIP_POINT
#define TUDO_SAMPLER_MIP_SHIFT			BGFX_SAMPLER_MIP_SHIFT
#define TUDO_SAMPLER_MIP_MASK			BGFX_SAMPLER_MIP_MASK
#define TUDO_SAMPLER_COMPARE_LESS		BGFX_SAMPLER_COMPARE_LESS
#define TUDO_SAMPLER_COMPARE_LEQUAL		BGFX_SAMPLER_COMPARE_LEQUAL
#define TUDO_SAMPLER_COMPARE_EQUAL		BGFX_SAMPLER_COMPARE_EQUAL
#define TUDO_SAMPLER_COMPARE_GEQUAL		BGFX_SAMPLER_COMPARE_GEQUAL
#define TUDO_SAMPLER_COMPARE_GREATER	BGFX_SAMPLER_COMPARE_GREATER
#define TUDO_SAMPLER_COMPARE_NOTEQUAL	BGFX_SAMPLER_COMPARE_NOTEQUAL
#define TUDO_SAMPLER_COMPARE_NEVER		BGFX_SAMPLER_COMPARE_NEVER
#define TUDO_SAMPLER_COMPARE_ALWAYS		BGFX_SAMPLER_COMPARE_ALWAYS
#define TUDO_SAMPLER_COMPARE_SHIFT		BGFX_SAMPLER_COMPARE_SHIFT
#define TUDO_SAMPLER_COMPARE_MASK		BGFX_SAMPLER_COMPARE_MASK
#define TUDO_SAMPLER_BORDER_COLOR_SHIFT BGFX_SAMPLER_BORDER_COLOR_SHIFT
#define TUDO_SAMPLER_BORDER_COLOR_MASK	BGFX_SAMPLER_BORDER_COLOR_MASK
#define TUDO_SAMPLER_BORDER_COLOR(v)	BGFX_SAMPLER_BORDER_COLOR(v)
#define TUDO_SAMPLER_RESERVED_SHIFT		BGFX_SAMPLER_RESERVED_SHIFT
#define TUDO_SAMPLER_RESERVED_MASK		BGFX_SAMPLER_RESERVED_MASK
#define TUDO_SAMPLER_NONE				BGFX_SAMPLER_NONE
#define TUDO_SAMPLER_SAMPLE_STENCIL		BGFX_SAMPLER_SAMPLE_STENCIL
#define TUDO_SAMPLER_POINT				BGFX_SAMPLER_POINT
#define TUDO_SAMPLER_UVW_MIRROR			BGFX_SAMPLER_UVW_MIRROR
#define TUDO_SAMPLER_UVW_CLAMP			BGFX_SAMPLER_UVW_CLAMP
#define TUDO_SAMPLER_UVW_BORDER			BGFX_SAMPLER_UVW_BORDER
#define TUDO_SAMPLER_BITS_MASK			BGFX_SAMPLER_BITS_MASK

namespace Tudo
{
	class Sprite;
	class Model3D;

	enum class DrawAPI
	{
		DIRECT3D11	= bgfx::RendererType::Direct3D11,
		DIRECT3D12	= bgfx::RendererType::Direct3D12,
		VULKAN		= bgfx::RendererType::Vulkan,
        METAL       = bgfx::RendererType::Metal
	};

	enum class ShaderUniformType
	{
		Sampler = bgfx::UniformType::Sampler,
		Vec4	= bgfx::UniformType::Vec4,
		Mat3	= bgfx::UniformType::Mat3,
		Mat4	= bgfx::UniformType::Mat4
	};

	/// range: [0, 1]
	struct Color
	{
		float R;
		float G;
		float B;
		float A;

		Color() : R(0), G(0), B(0), A(0) {}

		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}

		Color(const Color& other)
		{
			R = other.R;
			G = other.G;
			B = other.B;
			A = other.A;
		}

		Color(uint rgba)
		{
			R = ((rgba >> 16) & 0xff)	/ 255.0f;
			G = ((rgba >> 8) & 0xff)	/ 255.0f;
			B = (rgba & 0xff)			/ 255.0f;
			A = ((rgba >> 24) & 0xff)	/ 255.0f;
		}

		uint ToInt() const
		{
			uint r = (uint)(R * 255);
			uint g = (uint)(G * 255);
			uint b = (uint)(B * 255);
			uint a = (uint)(A * 255);
			return (r << 28) | (g << 16) | (b << 8) | a;
		}

		vec4 ToVec() const
		{
			return vec4(R, G, B, A);
		}

		Color operator=(const uint& rgba)
		{
			return Color(rgba);
		}
	};

	struct QuadVertex
	{
		float X;
		float Y;
		float U;
		float V;
	};

	struct SpriteInstanceData
	{
		Sprite*						pSprite;
		bgfx::InstanceDataBuffer	Buffer;
		int							MissedAmount; // Amount of instances not rendered due to hardware limitations
	};

	struct ModelInstanceData
	{
		Model3D*					pModel;
		bgfx::InstanceDataBuffer	Buffer;
		int							MissedAmount;
	};

	struct Viewport2D
	{
		vec2 Location;
		vec2 Size;
	};

	struct Transform2D
	{
		vec2	Location	= vec2(0.0f);
		vec2	Scale		= vec2(1.0f);
		float	Rotation	= 0.0f;
		Color	ImageColor	= 0xffffffff;
	};

	struct TransformAtlas2D : public Transform2D
	{
		vec2 Index = vec2(0.0f);
	};

	struct ModelInstanceTransform
	{
		mat4 ModelMatrix;
		vec4 IData;
	};

	struct BillboardTransform
	{
		vec3	Location	= vec3(0.0f);
		vec2	Scale		= vec2(1.0f);
		Color	ImageColor	= 0xffffffff;
		bool	bCylindric	= false;
		bool	bFlipU		= false;
		bool	bFlipV		= false;
	};

	struct BillboardTransformAtlas : public BillboardTransform
	{
		vec2 Index = vec2(0.0f);
	};

	struct SpriteFont
	{
		Sprite* pSprite;
		strg	Glyphs;
		vec2	GlyphSize;
	};

	struct Mesh3D
	{
		bgfx::VertexBufferHandle	VBH;
		bgfx::IndexBufferHandle		IBH;
	};
}
#endif
