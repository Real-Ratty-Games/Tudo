/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ASSETLOADER_HPP_
#define ASSETLOADER_HPP_
#include "SystemTypes.hpp"
#include "Globals.hpp"
#include "DrawObject.hpp"
#include "DrawData.hpp"
#include <vector>

namespace Tudo
{
	class	GraphicsDevice;
	class	WindowCursor;
	class	Texture;
	class	Model3D;
	class	Shader;
	class	SoundWav;
	class	SoundWavStream;

	class AssetLoader : public DrawObject
	{
	public:
		AssetLoader(GraphicsDevice& gdevice, strgv rootFolder);

		// For uncompressed textures
		bool LoadTextureFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename, bool flipUV, bool mipgen);
		bool LoadTextureFromMemory(Texture& texture, const std::vector<uint8>& memData, uint64 flags, strgv texturename, bool flipUV, bool mipgen);

		// For GPU friendly textures
		// DDS on Windows and KTX on macOS
		// Supports Cubemaps
		bool LoadTextureGPUFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename);
		bool LoadTextureGPUFromMemory(Texture& texture, const std::vector<uint8>& memData, uint64 flags, strgv texturename);

		bool LoadModelFromFile(Model3D& model, strgv filename, strgv name);
		bool LoadModelFromMemory(Model3D& model, const std::vector<uint8>& data, strgv name);

		bool LoadSoundFromFile(SoundWav& obj, strgv filepath, strgv name);
		bool StreamSoundFromFile(SoundWavStream& obj, strgv filepath, strgv name);
		bool LoadSoundFromMemory(SoundWav& obj, const std::vector<uint8>& data, strgv name);

		bool LoadShader(Shader& shader, strgv rootFolder, strgv shadername);

		/// file must be bmp
		bool LoadHardwareCursorImage(WindowCursor& cursor, strgv filename, strgv name);

	private:
		bool LoadTexture(Texture& texture, uint8* data, uint64 flags, int nrComponents,
			strgv texturename, int width, int height, bool mipgen);
		bool LoadGPUTexture(Texture& texture, const std::vector<uint8>& data, uint64 flags, strgv texturename);

		bool CreateMesh(Mesh3D& modelMesh, MeshData& mdata);

		const bgfx::Memory* ShaderGetMemory(std::ifstream& file);

	private:
		strg mRootDir;
	};
}
#endif