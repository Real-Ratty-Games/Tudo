/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Shader.hpp"
#include "Logger.hpp"

using namespace Tudo;

Shader::Shader(GraphicsDevice& gdevice) : DrawObject(gdevice),
	mHandle(BGFX_INVALID_HANDLE) { }

Shader::~Shader()
{
	if (bgfx::isValid(mHandle))
	{
		bgfx::destroy(mHandle);
		Logger::Log("Shader '" + mName + "' released!");
	}
}

void Shader::Submit(uint16 viewID, uint8 flags, const bool depth)
{
	bgfx::submit(viewID, mHandle, depth, flags);
}

bgfx::ProgramHandle& Shader::GetProgramHandle()
{
	return mHandle;
}

strg Shader::Name()
{
	return mName;
}
