/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef BILLBOARDRENDERER_HPP_
#define BILLBOARDRENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "Renderer.hpp"
#include <vector>

namespace Tudo
{
	class GraphicsDevice;
	class DrawPipeline;
	class SpriteAnimator;
	class Shader;
	class Sprite;

	class BillboardRenderer : public Renderer
	{
	public:
		BillboardRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);

		void DrawSprite(Sprite& sprite, const BillboardTransform& transform);
		void DrawSpriteInstanced(const SpriteInstanceData& idata);

		void DrawSpriteAtlas(Sprite& sprite, const BillboardTransformAtlas& transform, vec2 subSize);
		void DrawSpriteAtlasInstanced(const SpriteInstanceData& idata, vec2 subSize);

		void DrawSpriteAnimation(Sprite& sprite, const BillboardTransform& transform, const SpriteAnimator& animator);

		void PrepareSpriteInstancing(Sprite& sprite, SpriteInstanceData& idata, std::vector<BillboardTransform>& tdata);
		void PrepareSpriteAtlasInstancing(Sprite& sprite, SpriteInstanceData& idata, std::vector<BillboardTransformAtlas>& tdata);

	private:
		void DrawTexture(Shader& shader, Sprite& sprite, const BillboardTransform& transform);
	};
}
#endif