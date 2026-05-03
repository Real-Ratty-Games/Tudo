/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef DRAWSURFACE3D_HPP_
#define DRAWSURFACE3D_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "DrawSurface.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class Texture;

	class DrawSurface3D : public DrawSurface
	{
	public:
		DrawSurface3D(GraphicsDevice& gdevice, uint16 viewid, vec2 size, void* wndHandle, bool depthOnly);
		~DrawSurface3D();
		Texture& GetDepthTexture();

	protected:
		void DestroyFB();
		void UpdateFB(vec2i texSize, bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8) override;

	private:
		Texture*	mFbDepthTex;
		bool		bDepthOnly;
	};
}
#endif