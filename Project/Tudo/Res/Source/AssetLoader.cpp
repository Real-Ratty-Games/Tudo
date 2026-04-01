/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "AssetLoader.hpp"
#include "Memory.hpp"
#include "Logger.hpp"
#include "FileSystem.hpp"
#include "Math.hpp"
#include "GraphicsDevice.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Model3D.hpp"
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bx/allocator.h>
#include "Sound.hpp"
#include "SoundWav.hpp"
#include "WindowCursor.hpp"
#include "Window.hpp"
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

using namespace Tudo;

AssetLoader::AssetLoader(GraphicsDevice& gdevice, strgv rootFolder) : DrawObject(gdevice)
{
	mRootDir = FileSystem::GetResourcePath(rootFolder).string();
}

bool AssetLoader::LoadTextureFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename, bool flipUV, bool mipgen)
{
	const strg realpath = mRootDir + "/" + strg(filename);

	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	uint8* data = stbi_load(realpath.data(), &width, &height, &nrComponents, 0);

	if (!data)
	{
		Logger::Log("AssetLoader::LoadTextureFromFile", "Failed loading texture from file: " + strg(filename), ELogType::LWARNING);
		return false;
	}

	return LoadTexture(texture, data, flags, nrComponents, texturename, width, height, mipgen);
}

bool AssetLoader::LoadTextureFromMemory(Texture& texture, const std::vector<uint8>& memData, uint64 flags, strgv texturename, bool flipUV, bool mipgen)
{
	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	uint8* data = stbi_load_from_memory(memData.data(), (uint)memData.size(), &width, &height, &nrComponents, 0);

	if (!data)
	{
		Logger::Log("AssetLoader::LoadTextureFromMemory", "Failed loading texture from memory: " + strg(texturename), ELogType::LWARNING);
		return false;
	}

	return LoadTexture(texture, data, flags, nrComponents, texturename, width, height, mipgen);
}

bool AssetLoader::LoadTextureGPUFromFile(Texture& texture, strgv filename, uint64 flags, strgv texturename)
{
	const strg realpath = mRootDir + "/" + strg(filename);

	std::ifstream file(realpath.data(), std::ios::binary);
	if (!file)
	{
		Logger::Log("AssetLoader::LoadTextureGPUFromFile", "Failed loading GPU texture from file: " + strg(filename), ELogType::LWARNING);
		return false;
	}

	file.seekg(0, std::ios::end);
	const uint64 size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<uint8> data(size);
	file.read(reinterpret_cast<char*>(data.data()), size);
	file.close();

	return LoadGPUTexture(texture, data, flags, texturename);
}

bool AssetLoader::LoadTextureGPUFromMemory(Texture& texture, const std::vector<uint8>& memData, uint64 flags, strgv texturename)
{
	return LoadGPUTexture(texture, memData, flags, texturename);
}

bool AssetLoader::LoadModelFromFile(Model3D& model, strgv filename, strgv name)
{
	const strg realpath = mRootDir + "/" + strg(filename);

	std::ifstream file(realpath.data(), std::ios::binary);
	if (!file)
	{
		Logger::Log("AssetLoader::LoadModelFromFile", "Model file could not be found: " + strg(filename), ELogType::LWARNING);
		return false;
	}

	file.seekg(0, std::ios::beg);

	M3DCHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(M3DCHeader));

	if (header.Magic != M3DC_MAGIC)
	{
		Logger::Log("AssetLoader::LoadModelFromFile", "Model file format invalid: " + strg(filename), ELogType::LWARNING);
		return false;
	}

	if (header.Version != M3DC_VERSION)
	{
		Logger::Log("AssetLoader::LoadModelFromFile", "Model file format uses older version: " + strg(filename), ELogType::LWARNING);
		return false;
	}

	model.mMeshes.resize(header.Amount);

	for (uint64 i = 0; i < header.Amount; i++)
	{
		MeshData mdata;

		uint64 vsize;
		uint64 isize;
		file.read(reinterpret_cast<char*>(&vsize), sizeof(uint64));
		file.read(reinterpret_cast<char*>(&isize), sizeof(uint64));

		mdata.VSize = vsize;
		mdata.ISize = isize;

		mdata.Vertices.resize(vsize);
		mdata.Indices.resize(isize);

		file.read(reinterpret_cast<char*>(mdata.Vertices.data()), vsize * sizeof(MeshVertex));
		file.read(reinterpret_cast<char*>(mdata.Indices.data()), isize * sizeof(uint16));

		if (!CreateMesh(model.mMeshes[i], mdata))
			return false;
	}

	model.mName = name;
	Logger::Log("Model3D '" + model.mName + "' loaded!");
	return true;
}

bool AssetLoader::LoadModelFromMemory(Model3D& model, const std::vector<uint8>& data, strgv name)
{
	if (data.empty())
	{
		Logger::Log("AssetLoader::LoadModelFromMemory", "Data is empty!", ELogType::LWARNING);
		return false;
	}

	const uint8* rdat = data.data();
	uint64 offset = 0;

	M3DCHeader header;
	Memory::Copy(&header, rdat + offset, sizeof(M3DCHeader));
	offset += sizeof(M3DCHeader);

	if (header.Magic != M3DC_MAGIC)
	{
		Logger::Log("AssetLoader::LoadModelFromMemory", "Model file format invalid", ELogType::LWARNING);
		return false;
	}

	if (header.Version != M3DC_VERSION)
	{
		Logger::Log("AssetLoader::LoadModelFromMemory", "Model file format uses older version", ELogType::LWARNING);
		return false;
	}

	model.mMeshes.resize(header.Amount);

	for (uint64 i = 0; i < header.Amount; i++)
	{
		MeshData mdata;

		uint64 vsize;
		uint64 isize;

		Memory::Copy(&vsize, rdat + offset, sizeof(uint64));
		offset += sizeof(uint64);

		Memory::Copy(&isize, rdat + offset, sizeof(uint64));
		offset += sizeof(uint64);

		mdata.VSize = vsize;
		mdata.ISize = isize;

		mdata.Vertices.resize(vsize);
		mdata.Indices.resize(isize);

		const uint64 vtsize = vsize * sizeof(MeshVertex);
		Memory::Copy(mdata.Vertices.data(), rdat + offset, vtsize);
		offset += vtsize;

		const uint64 itsize = isize * sizeof(uint16);
		Memory::Copy(mdata.Indices.data(), rdat + offset, itsize);
		offset += itsize;

		if (!CreateMesh(model.mMeshes[i], mdata))
			return false;
	}

	model.mName = name;
	Logger::Log("Model3D '" + model.mName + "' loaded!");
	return true;
}

bool AssetLoader::LoadSoundFromFile(SoundWav& obj, strgv filepath, strgv name)
{
	const strg realpath = mRootDir + "/" + strg(filepath);
	if (obj.mHandle.load(realpath.data()) != SoLoud::SO_NO_ERROR)
	{
		Logger::Log("AssetLoader::LoadSoundFromFile", "Failed loading sound file: " + strg(filepath), ELogType::LWARNING);
		return false;
	}

	obj.mName = name;
	Logger::Log("SoundWav '" + obj.mName + "' loaded!");
	return true;
}

bool AssetLoader::StreamSoundFromFile(SoundWavStream& obj, strgv filepath, strgv name)
{
	const strg realpath = mRootDir + "/" + strg(filepath);
	if (obj.mHandle.load(realpath.data()) != SoLoud::SO_NO_ERROR)
	{
		Logger::Log("AssetLoader::StreamSoundFromFile", "Failed streaming sound file: " + strg(filepath), ELogType::LWARNING);
		return false;
	}

	obj.mName = name;
	Logger::Log("SoundWavStream '" + obj.mName + "' loaded!");
	return true;
}

bool AssetLoader::LoadSoundFromMemory(SoundWav& obj, const std::vector<uint8>& data, strgv name)
{
	if (obj.mHandle.loadMem(data.data(), static_cast<uint>(data.size()), false, false) != SoLoud::SO_NO_ERROR)
	{
		Logger::Log("AssetLoader::LoadSoundFromMemory", "Failed loading sound file from memory!", ELogType::LWARNING);
		return false;
	}

	obj.mName = name;
	Logger::Log("SoundWav '" + obj.mName + "' loaded!");
	return true;
}

bool AssetLoader::LoadShader(Shader& shader, strgv rootFolder, strgv shadername)
{
	const strg realpath = mRootDir + "/" + strg(rootFolder);

	bgfx::RendererType::Enum type = bgfx::getRendererType();
	strg typenm = "D3D";
	if (type == bgfx::RendererType::Vulkan)
		typenm = "SPIRV";
	else if (type == bgfx::RendererType::Metal)
		typenm = "METAL";

	strg path(realpath);
	path += "/" + typenm + "/" + strg(shadername);

	strg vpath = path + ".vsb";
	strg fpath = path + ".fsb";

	std::ifstream vfile(vpath.data(), std::ios::binary);
	if (!vfile)
	{
		Logger::Log("AssetLoader::LoadShader", "Failed loading vertex shader of: " + strg(shadername), ELogType::LWARNING);
		return false;
	}

	std::ifstream ffile(fpath.data(), std::ios::binary);
	if (!ffile)
	{
		Logger::Log("AssetLoader::LoadShader", "Failed loading fragment shader of: " + strg(shadername), ELogType::LWARNING);
		return false;
	}

	bgfx::ShaderHandle vsh = bgfx::createShader(ShaderGetMemory(vfile));
	bgfx::setName(vsh, strg(strg(shadername) + "_vs").c_str());

	bgfx::ShaderHandle fsh = bgfx::createShader(ShaderGetMemory(ffile));
	bgfx::setName(vsh, strg(strg(shadername) + "_fs").c_str());

	shader.mHandle = bgfx::createProgram(vsh, fsh, true);
	shader.mName = shadername;

	Logger::Log("Shader '" + shader.mName + "' loaded!");
	return true;
}

bool AssetLoader::LoadHardwareCursorImage(WindowCursor& cursor, strgv filename, strgv name)
{
	const strg realpath = mRootDir + "/" + strg(filename);
	SDL_Surface* cursorSurface = SDL_LoadBMP(realpath.data());
	if (cursorSurface != nullptr)
	{
		cursor.mCursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
		SDL_DestroySurface(cursorSurface);
	}
	else
	{
		Logger::Log("AssetLoader::LoadHardwareCursorImage", "Image file not found: " + strg(filename), ELogType::LWARNING);
		return false;
	}

	cursor.mName = name;
	Logger::Log("WindowCursor '" + cursor.mName + "' loaded!");
	return true;
}

bool AssetLoader::LoadTexture(Texture& texture, uint8* data, uint64 flags, int nrComponents,
	strgv texturename, int width, int height, bool mipgen)
{
	bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R8;
	if (nrComponents == 3)
		format = bgfx::TextureFormat::RGB8;
	else if (nrComponents == 4)
		format = bgfx::TextureFormat::RGBA8;

	if (mipgen)
	{
		int nmips = Math::Maxi(width, height);
		nmips = 1 + static_cast<int>(std::floor(std::log2(nmips)));

		texture.mHandle = bgfx::createTexture2D(
			static_cast<uint16>(width),
			static_cast<uint16>(height),
			true,
			1,
			format,
			flags,
			nullptr);

		int mipWidth = width;
		int mipHeight = height;
		stbi_uc* mipData = data;

		for (int i = 0; i < nmips; i++)
		{
			bgfx::updateTexture2D(
				texture.mHandle,
				0,
				i,
				0, 0,
				static_cast<uint16>(mipWidth),
				static_cast<uint16>(mipHeight),
				bgfx::copy(mipData, mipWidth * mipHeight * nrComponents));

			mipWidth = Math::Maxi(1, mipWidth / 2);
			mipHeight = Math::Maxi(1, mipHeight / 2);
			stbi_uc* nextMip = (stbi_uc*)malloc(mipWidth * mipHeight * nrComponents);

			stbir_resize_uint8_linear(mipData, mipWidth * 2, mipHeight * 2, 0, nextMip, mipWidth, mipHeight, 0,
				(stbir_pixel_layout)nrComponents);

			if (mipData != data) free(mipData);
			mipData = nextMip;
		}

		if (mipData != data) free(mipData);
	}
	else
	{
		texture.mHandle = bgfx::createTexture2D(
			static_cast<uint16>(width),
			static_cast<uint16>(height),
			false,
			0,
			format,
			flags,
			bgfx::copy(data, width * height * nrComponents));
	}

	if (bgfx::isValid(texture.mHandle))
		bgfx::setName(texture.mHandle, texturename.data());
	else
	{
		Logger::Log("AssetLoader::LoadTexture", "Failed loading texture: " + strg(texturename), ELogType::LWARNING);
		return false;
	}

	texture.mSize = vec2i(width, height);

	texture.mName = texturename;

	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);

	Logger::Log("Texture '" + texture.mName + "' loaded!");
	return true;
}

bool AssetLoader::LoadGPUTexture(Texture& texture, const std::vector<uint8>& data, uint64 flags, strgv texturename)
{
	bx::DefaultAllocator allc;
#if __APPLE__
	bimg::ImageContainer* ic = bimg::imageParseKtx(&allc, data.data(), data.size(), nullptr);
#else
	bimg::ImageContainer* ic = bimg::imageParseDds(&allc, data.data(), (uint)data.size(), nullptr);
#endif
	if (!ic)
	{
		Logger::Log("AssetLoader::LoadGPUTexture", "Failed parsing GPU texture into container: " + strg(texturename), ELogType::LWARNING);
		return false;
	}

	if (ic->m_cubeMap)
	{
		texture.mHandle = bgfx::createTextureCube(
			static_cast<uint16>(ic->m_width),
			(ic->m_numMips > 0), ic->m_numLayers,
			(bgfx::TextureFormat::Enum)ic->m_format,
			flags,
			bgfx::copy(ic->m_data, ic->m_size));
		texture.bIsCubemap = true;
	}
	else
	{
		texture.mHandle = bgfx::createTexture2D(
			static_cast<uint16>(ic->m_width),
			static_cast<uint16>(ic->m_height),
			(ic->m_numMips > 0),
			ic->m_numLayers,
			(bgfx::TextureFormat::Enum)ic->m_format,
			flags,
			bgfx::copy(ic->m_data, ic->m_size));
	}

	if (bgfx::isValid(texture.mHandle))
		bgfx::setName(texture.mHandle, texturename.data());
	else
	{
		Logger::Log("AssetLoader::LoadGPUTexture", "Failed loading GPU texture: " + strg(texturename), ELogType::LWARNING);
		return false;
	}

	texture.mName = texturename;
	texture.mSize = vec2i(ic->m_width, ic->m_height);
	bimg::imageFree(ic);

	Logger::Log("GPU Texture '" + texture.mName + "' loaded!");
	return true;
}

bool AssetLoader::CreateMesh(Mesh3D& modelMesh, MeshData& mdata)
{
	modelMesh.VBH = pGDevice->CreateVertexBuffer(mdata.Vertices.data(), (uint)(mdata.VSize * sizeof(MeshVertex)), pGDevice->mMesh3DVBLayout);
	if (!bgfx::isValid(modelMesh.VBH))
	{
		Logger::Log("AssetLoader::CreateMesh", "Vertex Buffer is invalid!", ELogType::LWARNING);
		return false;
	}

	modelMesh.IBH = pGDevice->CreateIndexBuffer(mdata.Indices.data(), (uint)(mdata.ISize * sizeof(uint16)));
	if (!bgfx::isValid(modelMesh.IBH))
	{
		Logger::Log("AssetLoader::CreateMesh", "Index Buffer is invalid!", ELogType::LWARNING);
		return false;
	}
	return true;
}

const bgfx::Memory* AssetLoader::ShaderGetMemory(std::ifstream& file)
{
	file.seekg(0, std::ios::end);
	const uint64 size = file.tellg();
	file.seekg(0, std::ios::beg);

	const bgfx::Memory* mem = bgfx::alloc((uint)size + 1);
	file.read(reinterpret_cast<char*>(mem->data), size);
	return mem;
}
