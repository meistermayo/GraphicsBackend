#ifndef _TerrainModel
#define _TerrainModel

#include "Math/Matrix.h"
#include <tuple>
#include <d3d11.h>
#include "DirectXTex.h"
#include "Model.h"

struct ID3D11DeviceContext;
struct ID3D11Device;

class TerrainModel : public Model
{

private:
	Matrix WorldMat;

	void ConstructTerrain(const DirectX::Image* hMap, float maxHeight, float len,int repeatU, int repeatV);

	Vect normalHelper(Vect p, Vect p6, Vect p2, Vect p1, Vect p4, Vect p8, Vect p9);

public:
	TerrainModel(std::string heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV);
	Matrix GetWorldMat() { return WorldMat; }
};



#endif _TerrainModel