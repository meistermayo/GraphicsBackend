#pragma once
#ifndef _LightStructs
#define _LightStructs
#include "Math/Vect.h"

struct Material
{
	Vect Ambient;
	Vect Diffuse;
	Vect Specular;
};

struct PhongADS
{
	Vect Ambient;
	Vect Diffuse;
	Vect Specular;
};

struct DirectionalLight
{
	PhongADS Light;
	Vect Direction;
};


struct PointLight
{
	PhongADS Light;
	Vect Position;
	Vect Attenuation;
	float Range;
};


struct SpotLight
{
	PhongADS Light;
	Vect Position;
	Vect Attenuation;
	Vect Direction;
	float SpotExp;
	float Range;
};

void SetPhong(PhongADS& phong, const Vect& amb, const Vect& dif, const Vect& sp);
#endif