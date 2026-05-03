/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "StaticModel.hpp"
#include <FileSystem.hpp>

StaticModel::StaticModel() :
	Scale(1),
	Rotation(0)
{
	InitProperty(Scale,		"Scale");
	InitProperty(Rotation,	"Rotation");
}

void StaticModel::Begin()
{
	// TODO RENDER MODEL
}

void StaticModel::End()
{
	// TODO KILL RENDER MODEL
}
