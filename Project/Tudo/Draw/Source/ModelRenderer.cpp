/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "ModelRenderer.hpp"
#include "GraphicsDevice.hpp"
#include "DrawPipeline.hpp"
#include "Model3D.hpp"

using namespace Tudo;

ModelRenderer::ModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline) : Renderer(gdevice, pipeline) {}

void ModelRenderer::DrawModel(const Model3D& model, const mat4& transform)
{
	for (auto& mesh : model.Meshes())
	{
		bgfx::setTransform(transform.Ptr());
		DrawMesh(mesh);
	}
}

void ModelRenderer::DrawModel(const Model3D& model, const mat4& transform, uint mesh)
{
	const uint64 mcnt = model.Meshes().size();
	if (mesh > mcnt || mesh < 0) return;

	bgfx::setTransform(transform.Ptr());
	DrawMesh(model.Meshes()[mesh]);
}

void ModelRenderer::DrawModelInstanced(const ModelInstanceData& idata)
{
	bgfx::setInstanceDataBuffer(&idata.Buffer);
	for (auto& mesh : idata.pModel->Meshes())
		DrawMeshInstanced(mesh);
}

void ModelRenderer::DrawMeshInstanced(const Mesh3D& mesh)
{
	DrawMesh(mesh);
}
