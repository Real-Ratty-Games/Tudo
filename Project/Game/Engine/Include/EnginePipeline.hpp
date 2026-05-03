/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ENGINEPIPELINE_HPP_
#define ENGINEPIPELINE_HPP_
#include <SystemTypes.hpp>
#include <DrawPipeline.hpp>
#include <AssetLoader.hpp>
#include <AssetManager.hpp>

#include <GraphicsDevice.hpp>
#include <DrawSurface2D.hpp>
#include <DrawSurface3D.hpp>

#include <Shader.hpp>

#include <SpriteRenderer.hpp>
#include <UnlitModelRenderer.hpp>
#include <ColorModelRenderer.hpp>
#include <BillboardRenderer.hpp>
#include <SkyboxModelRenderer.hpp>

#include <Sprite.hpp>

using namespace Tudo;

class EnginePipeline : public DrawPipeline
{
public:
	EnginePipeline(GraphicsDevice& gdevice, AssetLoader& assetLoader, vec2 resolution);
	~EnginePipeline();
	void OnResize(vec2 size);

	void SetWorldViewport(Viewport3D& viewport);

protected:
	void Draw();
	void LoadShaders(AssetLoader& assetLoader);

private:
	AssetManager<Shader>	mShaderManager;

	Viewport2D				mScreenCamera;
	DrawSurface2D*			mBackBufferSurface;

	DrawSurface3D*			mWorldSurface;
	Sprite*					mWorldSprite;

	SpriteRenderer*			mSpriteRenderer;
	UnlitModelRenderer*		mUnlitRenderer;
	ColorModelRenderer*		mColorModelRenderer;
	BillboardRenderer*		mBillboardRenderer;
	SkyboxModelRenderer*	mSkyboxModelRenderer;

	Viewport3D*				pWorldView;
};
#endif