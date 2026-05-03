/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "EnginePipeline.hpp"
#include <format>

#define TUDO_ENGINE_LOADSHADER(x, y) {Shader* shader = new Shader(*pGDevice); assetLoader.LoadShader(*shader, x, y); mShaderManager.AddItem(shader);}

EnginePipeline::EnginePipeline(GraphicsDevice& gdevice, AssetLoader& assetLoader, vec2 resolution) : DrawPipeline(gdevice),
	pWorldView(nullptr)
{
	mSpriteRenderer			= new SpriteRenderer(gdevice, *this);
	mUnlitRenderer			= new UnlitModelRenderer(gdevice, *this);
	mColorModelRenderer		= new ColorModelRenderer(gdevice, *this);
	mBillboardRenderer		= new BillboardRenderer(gdevice, *this);
	mSkyboxModelRenderer	= new SkyboxModelRenderer(gdevice, *this);

	mScreenCamera.Size = resolution;

	mBackBufferSurface = new DrawSurface2D(gdevice, 0, resolution, nullptr);

	mWorldSurface = new DrawSurface3D(gdevice, 1, resolution, nullptr, false);
	mWorldSprite = new Sprite(mWorldSurface->GetTexture());

	LoadShaders(assetLoader);
}

EnginePipeline::~EnginePipeline()
{
	TUDO_MEM_FREE(mWorldSprite);
	TUDO_MEM_FREE(mWorldSurface);
	TUDO_MEM_FREE(mBackBufferSurface);
	TUDO_MEM_FREE(mSpriteRenderer);
	TUDO_MEM_FREE(mUnlitRenderer);
	TUDO_MEM_FREE(mColorModelRenderer);
	TUDO_MEM_FREE(mBillboardRenderer);
	TUDO_MEM_FREE(mSkyboxModelRenderer);
}

void EnginePipeline::Draw()
{
	if (pWorldView)
	{
		PrepareDraw3D(*mWorldSurface, *pWorldView);
	}

	PrepareDraw2D(*mBackBufferSurface, mScreenCamera);
		SetActiveShader(&mShaderManager.Get("Sprite2D"));
		mSpriteRenderer->DrawSprite(*mWorldSprite, Transform2D());
}

void EnginePipeline::OnResize(vec2 size)
{
	mBackBufferSurface->OnResize(size);
	mWorldSurface->OnResize(size);

	mScreenCamera.Size = size;
	mWorldSprite->Size = size;
}

void EnginePipeline::SetWorldViewport(Viewport3D& viewport)
{
	pWorldView = &viewport;
}

void EnginePipeline::LoadShaders(AssetLoader& assetLoader)
{
	TUDO_ENGINE_LOADSHADER("Shaders", "Sprite2D");
	TUDO_ENGINE_LOADSHADER("Shaders", "Sprite2DI");
	TUDO_ENGINE_LOADSHADER("Shaders", "Sprite2DAtlas");
	TUDO_ENGINE_LOADSHADER("Shaders", "Sprite2DAtlasI");
	TUDO_ENGINE_LOADSHADER("Shaders", "ColorQuad");
	TUDO_ENGINE_LOADSHADER("Shaders", "UnlitMesh");
	TUDO_ENGINE_LOADSHADER("Shaders", "UnlitMeshI");
	TUDO_ENGINE_LOADSHADER("Shaders", "ColorMesh");
	TUDO_ENGINE_LOADSHADER("Shaders", "ColorMeshI");
	TUDO_ENGINE_LOADSHADER("Shaders", "Billboard");
	TUDO_ENGINE_LOADSHADER("Shaders", "BillboardI");
	TUDO_ENGINE_LOADSHADER("Shaders", "BillboardAtlas");
	TUDO_ENGINE_LOADSHADER("Shaders", "BillboardAtlasI");
	TUDO_ENGINE_LOADSHADER("Shaders", "SkyboxMesh");
	TUDO_ENGINE_LOADSHADER("Shaders", "SkyboxMeshI");

	pGDevice->InitShaderUniform("s_texColor",	EShaderUniformType::Sampler);
	pGDevice->InitShaderUniform("s_envMap",		EShaderUniformType::Sampler);
	pGDevice->InitShaderUniform("u_color",		EShaderUniformType::Vec4);
	pGDevice->InitShaderUniform("u_atlasInfo",	EShaderUniformType::Vec4, 2);
	pGDevice->InitShaderUniform("u_transform",	EShaderUniformType::Vec4);
	pGDevice->InitShaderUniform("u_location",	EShaderUniformType::Vec4);
	pGDevice->InitShaderUniform("u_flags",		EShaderUniformType::Vec4);
}
