/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/GamePipeline.hpp"

using namespace MyGame;

GamePipeline::GamePipeline(GraphicsDevice& gdevice, AssetLoader& assetloader, vec2 resolution) : DrawPipeline(gdevice)
{
	// create sprite renderer
	mSpriteRenderer = new SpriteRenderer(gdevice, *this);

	// create unlit renderer
	mUnlitRenderer = new UnlitModelRenderer(gdevice, *this);

	// create color renderer
	mColorModelRenderer = new ColorModelRenderer(gdevice, *this);

	// create billboard renderer
	mBillboardRenderer = new BillboardRenderer(*pGDevice, *this);

	// create back buffer surface
	mBackBufferSurface = new DrawSurface2D(gdevice, 0, resolution, nullptr);

	mCamera.Size = resolution;

	// load shaders
	LoadShaders(assetloader);



	_ftex = new Texture(gdevice);
	assetloader.LoadTextureFromFile(*_ftex, "Font.png", TUDO_SAMPLER_MIN_POINT |
		TUDO_SAMPLER_MAG_POINT |
		TUDO_SAMPLER_U_CLAMP |
		TUDO_SAMPLER_V_CLAMP, "Font", false, false);

	_spriteFont = new Sprite(*_ftex);

	_ffont.pSprite		= _spriteFont.Get();
	_ffont.GlyphSize	= vec2(18, 30);
	_ffont.Glyphs		=
		"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
		"ÄäÖöÜü"
		"0123456789"
		",;.:_-!?\"§$%&/()=*+~'#|<>²³{[]}\\";

	Transform2D transf;
	transf.Location = vec2(100, 100);
	mSpriteRenderer->PrepareSpriteFontText(_ffont, transf, _simpleTextData, "Hello, Tudo!\nThis is so cool!");
}

void GamePipeline::Draw()
{
	PrepareDraw2D(*mBackBufferSurface, mCamera);

		SetActiveShader(mSprite2DAtlasIShader.Get());
		mSpriteRenderer->DrawSpriteFontText(_ffont, _simpleTextData);
}

void GamePipeline::OnResize(vec2 size)
{
	mBackBufferSurface->OnResize(size);
}

void GamePipeline::LoadShaders(AssetLoader& assetloader)
{
	// default sprite shader
	mSprite2DShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DShader, "Shaders", "Sprite2D");

	// default sprite instancing shader
	mSprite2DIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DIShader, "Shaders", "Sprite2DI");

	// default sprite atlas shader
	mSprite2DAtlasShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DAtlasShader, "Shaders", "Sprite2DAtlas");

	// default sprite atlas instancing shader
	mSprite2DAtlasIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mSprite2DAtlasIShader, "Shaders", "Sprite2DAtlasI");

	// default color quad shader
	mColorQuadShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mColorQuadShader, "Shaders", "ColorQuad");

	// default unlit mesh shader
	mUnlitMeshShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mUnlitMeshShader, "Shaders", "UnlitMesh");

	// default unlit mesh instancing shader
	mUnlitMeshIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mUnlitMeshIShader, "Shaders", "UnlitMeshI");

	// default color mesh shader
	mColorMeshShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mColorMeshShader, "Shaders", "ColorMesh");

	// default color mesh instancing shader
	mColorMeshIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mColorMeshIShader, "Shaders", "ColorMeshI");

	// default billboard shader
	mBillboardShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mBillboardShader, "Shaders", "Billboard");

	// default billboard instancing shader
	mBillboardIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mBillboardIShader, "Shaders", "BillboardI");

	// default billboard atlas shader
	mBillboardAtlasShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mBillboardAtlasShader, "Shaders", "BillboardAtlas");

	// default billboard atlas instancing shader
	mBillboardAtlasIShader = new Shader(*pGDevice);
	assetloader.LoadShader(*mBillboardAtlasIShader, "Shaders", "BillboardAtlasI");

	pGDevice->InitShaderUniform("s_texColor", ShaderUniformType::Sampler);
	pGDevice->InitShaderUniform("u_color", ShaderUniformType::Vec4);
	pGDevice->InitShaderUniform("u_atlasInfo", ShaderUniformType::Vec4, 2);
	pGDevice->InitShaderUniform("u_transform", ShaderUniformType::Vec4);
	pGDevice->InitShaderUniform("u_flags", ShaderUniformType::Vec4);
}
