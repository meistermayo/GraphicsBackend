#include "LightStructs.h"

void SetPhong(PhongADS& phong, const Vect& amb, const Vect& dif, const Vect& sp)
{
	phong.Ambient = amb;
	phong.Diffuse = dif;
	phong.Specular = sp;
}