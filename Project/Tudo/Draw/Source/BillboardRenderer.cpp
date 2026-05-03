/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "BillboardRenderer.hpp"
#include "Memory.hpp"
#include "Transformation.hpp"
#include "GraphicsDevice.hpp"
#include "SpriteAnimation.hpp"
#include "DrawPipeline.hpp"
#include "DrawSurface.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"

using namespace Tudo;

BillboardRenderer::BillboardRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : Renderer(gdevice, pipeline) {}

void BillboardRenderer::DrawSprite(Sprite& sprite, const BillboardTransform& transform)
{
	DrawTexture(*pPipeline->GetActiveShader(), sprite, transform);
}

void BillboardRenderer::DrawSpriteInstanced(const SpriteInstanceData& idata)
{
	Shader* shader = pPipeline->GetActiveShader();
	Sprite* sprite = idata.pSprite;

	bgfx::setState(TUDO_RENDERER_MESH_TRANSPARENT_STATE);
	bgfx::setVertexBuffer(0, pGDevice->GetQuadVertexHandle());
	bgfx::setInstanceDataBuffer(&idata.Buffer);

	vec4 tvec = vec4(0, 0, sprite->RotationPivot.X, sprite->RotationPivot.Y);
	pGDevice->SetShaderUniform("u_transform", tvec.Ptr());

	pGDevice->SetShaderTexture(0, "s_texColor", sprite->GetTexture());

	shader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}

void BillboardRenderer::DrawSpriteAtlas(Sprite& sprite, const BillboardTransformAtlas& transform, vec2 subSize)
{
	Shader* shader = pPipeline->GetActiveShader();

	vec4 atinf[2];
	atinf[0] = vec4(transform.Index.X, transform.Index.Y, sprite.Size.X, sprite.Size.Y);
	atinf[1] = vec4(subSize.X, subSize.Y, 1.0f, 0.0f);
	pGDevice->SetShaderUniform("u_atlasInfo", atinf, 2);

	DrawTexture(*shader, sprite, transform);
}

void BillboardRenderer::DrawSpriteAtlasInstanced(const SpriteInstanceData& idata, vec2 subSize)
{
	Shader* shader = pPipeline->GetActiveShader();
	Sprite* sprite = idata.pSprite;

	bgfx::setState(TUDO_RENDERER_MESH_TRANSPARENT_STATE);
	bgfx::setVertexBuffer(0, pGDevice->GetQuadVertexHandle());
	bgfx::setInstanceDataBuffer(&idata.Buffer);

	vec4 atinf[2];
	atinf[0] = vec4(0, 0, sprite->Size.X, sprite->Size.Y);
	atinf[1] = vec4(subSize.X, subSize.Y, 1.0f, 0.0f);
	pGDevice->SetShaderUniform("u_atlasInfo", atinf, 2);

	vec4 tvec = vec4(0, 0, sprite->RotationPivot.X, sprite->RotationPivot.Y);
	pGDevice->SetShaderUniform("u_transform", tvec.Ptr());

	pGDevice->SetShaderTexture(0, "s_texColor", sprite->GetTexture());

	shader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}

void BillboardRenderer::DrawSpriteAnimation(Sprite& sprite, const BillboardTransform& transform, const SpriteAnimator& animator)
{
	BillboardTransformAtlas transf;
	transf.Location		= transform.Location;
	transf.Scale		= transform.Scale;
	transf.ImageColor	= transform.ImageColor;
	transf.bCylindric	= transform.bCylindric;
	transf.bFlipU		= transform.bFlipU;
	transf.bFlipV		= transform.bFlipV;
	transf.Index		= animator.GetCurrentIndex();
	DrawSpriteAtlas(sprite, transf, animator.GetAnimation()->FrameSize);
}

void BillboardRenderer::PrepareSpriteInstancing(Sprite& sprite, SpriteInstanceData& idata, std::vector<BillboardTransform>& tdata)
{
	const int insCnt = (int)tdata.size();
	const uint16 insStride = 3 * sizeof(vec4);
	uint drawnIns = bgfx::getAvailInstanceDataBuffer(insCnt, insStride);

	idata.MissedAmount = insCnt - drawnIns;
	idata.pSprite = &sprite;

	bgfx::allocInstanceDataBuffer(&idata.Buffer, drawnIns, insStride);

	uint8* data = idata.Buffer.data;

	for (uint i = 0; i < drawnIns; i++)
	{
		auto& transf = tdata[i];

		vec4 loc = vec4(transf.Location.X, transf.Location.Y, transf.Location.Z, 1);
		Memory::Copy(data, loc.Ptr(), sizeof(vec4));
		data += sizeof(vec4);

		vec4 idata = vec4(transf.Scale.X, transf.Scale.Y, transf.bCylindric ? 1.0f : 0.0f, 0.0f);
		Memory::Copy(data, idata.Ptr(), sizeof(vec4));
		data += sizeof(vec4);

		vec4 col = transf.ImageColor.ToVec();
		Memory::Copy(data, col.Ptr(), sizeof(vec4));
		data += sizeof(vec4);
	}
}

void BillboardRenderer::PrepareSpriteAtlasInstancing(Sprite& sprite, SpriteInstanceData& idata, std::vector<BillboardTransformAtlas>& tdata)
{
	const int insCnt = (int)tdata.size();
	const uint16 insStride = 4 * sizeof(vec4);
	uint drawnIns = bgfx::getAvailInstanceDataBuffer(insCnt, insStride);

	idata.MissedAmount = insCnt - drawnIns;
	idata.pSprite = &sprite;

	bgfx::allocInstanceDataBuffer(&idata.Buffer, drawnIns, insStride);

	uint8* data = idata.Buffer.data;

	for (uint i = 0; i < drawnIns; i++)
	{
		auto& transf = tdata[i];

		vec4 loc = vec4(transf.Location.X, transf.Location.Y, transf.Location.Z, transf.bFlipV ? 1.0f : 0.0f);
		Memory::Copy(data, loc.Ptr(), sizeof(vec4));
		data += sizeof(vec4);

		vec4 idata = vec4(transf.Scale.X, transf.Scale.Y, transf.bCylindric ? 1.0f : 0.0f, transf.bFlipU ? 1.0f : 0.0f);
		Memory::Copy(data, idata.Ptr(), sizeof(vec4));
		data += sizeof(vec4);

		vec4 col = transf.ImageColor.ToVec();
		Memory::Copy(data, col.Ptr(), sizeof(vec4));
		data += sizeof(vec4);

		vec4 iddata = vec4(transf.Index.X, transf.Index.Y, 0, 0);
		Memory::Copy(data, iddata.Ptr(), sizeof(vec4));
		data += sizeof(vec4);
	}
}

void BillboardRenderer::DrawTexture(Shader& shader, Sprite& sprite, const BillboardTransform& transform)
{
	bgfx::setState(TUDO_RENDERER_MESH_TRANSPARENT_STATE);
	bgfx::setVertexBuffer(0, pGDevice->GetQuadVertexHandle());

	vec4 tvec = vec4(transform.Scale.X, transform.Scale.Y, sprite.RotationPivot.X, sprite.RotationPivot.Y);
	pGDevice->SetShaderUniform("u_transform", tvec.Ptr());

	vec4 lvec = vec4(transform.Location.X, transform.Location.Y, transform.Location.Z, 0.0f);
	pGDevice->SetShaderUniform("u_location", lvec.Ptr());

	vec4 fvec = vec4(transform.bCylindric ? 1.0f : 0.0f, transform.bFlipV ? 1.0f : 0.0f, transform.bFlipU ? 1.0f : 0.0f, 0.0f);
	pGDevice->SetShaderUniform("u_flags", fvec.Ptr());

	vec4 col = transform.ImageColor.ToVec();
	pGDevice->SetShaderUniform("u_color", col.Ptr());

	pGDevice->SetShaderTexture(0, "s_texColor", sprite.GetTexture());

	shader.Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}
