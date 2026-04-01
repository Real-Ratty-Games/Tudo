/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SKYBOXMODELRENDERER_HPP_
#define SKYBOXMODELRENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "ModelRenderer.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawPipeline;
	struct Mesh3D;
	class Texture;
	class Shader;

	class SkyboxModelRenderer : public ModelRenderer
	{
	public:
		SkyboxModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);
		void DrawMesh(const Mesh3D& mesh);
		void SetTexture(Texture* texture);

	protected:
		void SetupMesh() override;

	private:
		Texture*	pTexture;
		Shader*		pShader;
	};
}
#endif