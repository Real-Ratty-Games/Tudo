/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef MODELRENDERER_HPP_
#define MODELRENDERER_HPP_
#include "SystemTypes.hpp"
#include "DrawData.hpp"
#include "Renderer.hpp"

namespace Tudo
{
	class GraphicsDevice;
	class DrawPipeline;
	class Model3D;

	class ModelRenderer : public Renderer
	{
	public:
		ModelRenderer(GraphicsDevice& gdevice, DrawPipeline& pipeline);

		void			DrawModel(const Model3D& model, const mat4& transform);
		void			DrawModel(const Model3D& model, const mat4& transform, uint mesh);
		void			DrawModelInstanced(const ModelInstanceData& idata);
	
	protected:
		virtual void	DrawMesh(const Mesh3D& mesh) = 0;
		virtual void	DrawMeshInstanced(const Mesh3D& mesh);
	};
}
#endif