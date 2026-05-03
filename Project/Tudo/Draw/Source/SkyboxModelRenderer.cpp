/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "SkyboxModelRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawPipeline.hpp"
#include "DrawSurface.hpp"
#include "Model3D.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

using namespace Tudo;

SkyboxModelRenderer::SkyboxModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : ModelRenderer(gdevice, pipeline), pTexture(nullptr) {}

void SkyboxModelRenderer::DrawMesh(const Mesh3D& mesh)
{
	Shader* shader = pPipeline->GetActiveShader();
	bgfx::setState(TUDO_RENDERER_MESH_SKYBOX_STATE);
	pGDevice->SetShaderTexture(0, "s_envMap", *pTexture);
	pGDevice->SetMesh(0, mesh);
	shader->Submit(pPipeline->GetActiveDrawSurface()->ViewID(), TUDO_RENDERER_MESH_DEFAULT_DISCARD, true);
}

void SkyboxModelRenderer::SetTexture(Texture* texture)
{
	pTexture = texture;
}
