/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Texture.hpp"
#include "Logger.hpp"
#include "GraphicsDevice.hpp"

using namespace Tudo;

Texture::Texture(GraphicsDevice& gdevice) : DrawObject(gdevice)
{
	mHandle		= BGFX_INVALID_HANDLE;
	bIsCubemap	= false;
}

Texture::~Texture()
{
	if (bgfx::isValid(mHandle))
	{
		bgfx::destroy(mHandle);
		mHandle = BGFX_INVALID_HANDLE;
		Logger::Log("Texture '" + mName + "' released!");
	}
}

bgfx::TextureHandle& Texture::Handle()
{
	return mHandle;
}

bool Texture::IsCubemap()
{
	return bIsCubemap;
}

strg Texture::Name()
{
	return mName;
}

const vec2i& Texture::Size()
{
	return mSize;
}
