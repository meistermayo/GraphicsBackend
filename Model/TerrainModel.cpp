#include "TerrainModel.h"
#include "d3dUtil.h"
#include <assert.h>

TerrainModel::TerrainModel(std::string heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
	: Model()
{
	ytrans;
	DirectX::ScratchImage scrtTex;
	HRESULT hr = LoadFromTGAFile((LPCWSTR)heightmapFile.c_str(), nullptr, scrtTex); // todo
	assert(SUCCEEDED(hr));

	const DirectX::Image* hgtmap = scrtTex.GetImage(0, 0, 0);
	assert(hgtmap->height == hgtmap->width );


	WorldMat = Matrix::Scale(0.25f);

	ConstructTerrain(hgtmap, maxheight, len, RepeatU, RepeatV);
	privLoadDataToGPU();
}

void TerrainModel::ConstructTerrain(const DirectX::Image* hMap,float maxHeight,float len,int repeatU, int repeatV)
{
	// do you have to be able and willing to ruin your life to have a semblance of what morality is
	size_t height = hMap->height;
	size_t width =  hMap->width;

	numTris = (height-1)*(height-1) * 2;
	numVerts = numTris * 3;

	pStdVerts = new StandardVertex[numVerts];
	pTriList = new TriangleByIndex[numTris];

	for (size_t x = 0; x < width-1; x++)
	{
		for (size_t y = 0; y < height-1; y++)
		{
			size_t i = (y * 6) + (x * 6 * (height - 1));
			float scale = (maxHeight / 255.0f);
			float side = (float)width;

			float x0 = (float)(x);
			float x1 = (float)(x + 1);
			float y0 = (float)(y);
			float y1 = (float)(y + 1);


			float h_val_f0 = ((float)hMap->pixels[4 * ((x + 0) * hMap->height + y + 0)]) * scale;
			float h_val_f1 = ((float)hMap->pixels[4 * ((x + 1) * hMap->height + y + 1)]) * scale;
			float h_val_f2 = ((float)hMap->pixels[4 * ((x + 1) * hMap->height + y + 0)]) * scale;
			float h_val_f3 = ((float)hMap->pixels[4 * ((x + 0) * hMap->height + y + 0)]) * scale;
			float h_val_f4 = ((float)hMap->pixels[4 * ((x + 0) * hMap->height + y + 1)]) * scale;
			float h_val_f5 = ((float)hMap->pixels[4 * ((x + 1) * hMap->height + y + 1)]) * scale;
			
			// # P.xy store the position for which we want to calculate the normals

			

			pStdVerts[i + 0].set(x0*len, h_val_f0*len, y0*len, ((x0 / side)*repeatU), ((y0 / side)*repeatV), Vect(), Colors::White*h_val_f0);
			pStdVerts[i + 1].set(x1*len, h_val_f1*len, y1*len, ((x1 / side)*repeatU), ((y1 / side)*repeatV), Vect(), Colors::White*h_val_f1);
			pStdVerts[i + 2].set(x1*len, h_val_f2*len, y0*len, ((x1 / side)*repeatU), ((y0 / side)*repeatV), Vect(), Colors::White*h_val_f2);
			pStdVerts[i + 3].set(x0*len, h_val_f3*len, y0*len, ((x0 / side)*repeatU), ((y0 / side)*repeatV), Vect(), Colors::White*h_val_f3);
			pStdVerts[i + 4].set(x0*len, h_val_f4*len, y1*len, ((x0 / side)*repeatU), ((y1 / side)*repeatV), Vect(), Colors::White*h_val_f4);
			pStdVerts[i + 5].set(x1*len, h_val_f5*len, y1*len, ((x1 / side)*repeatU), ((y1 / side)*repeatV), Vect(), Colors::White*h_val_f5);


		}
	}

	for (size_t x = 1; x < width - 2; x++)
	{
		for (size_t y = 1; y < height - 2; y++)
		{
			size_t i = (y * 6) + (x * 6 * (height-1));
			if (x == 1 && y == 1) continue;
				// BL
				pStdVerts[i].normal =
					normalHelper(

						pStdVerts[i].Pos,										// p
						pStdVerts[i + 2].Pos,									// p6
						pStdVerts[i - 3].Pos,									// p2
						pStdVerts[i - (int)(6 * height) - 3].Pos,				// p1
						pStdVerts[i - (int)(6 * height) - 2].Pos,				// p4
						pStdVerts[i + 4].Pos,									// p8
						pStdVerts[i + 1].Pos									// p9

					);
				i++;

				//UR
				pStdVerts[i].normal =
					normalHelper(

						pStdVerts[i].Pos,										// p
						pStdVerts[i + (int)(6 * height)].Pos,									// p6
						pStdVerts[i + 1].Pos,								// p2
						pStdVerts[i + 2].Pos,				// p1
						pStdVerts[i + 3].Pos,				// p4
						pStdVerts[i + 6].Pos,									// p8
						pStdVerts[i + (int)(6 * height)+6].Pos									// p9

					);
				i++;

				//BR
				pStdVerts[i].normal =
					normalHelper(

						pStdVerts[i].Pos,										// p
						pStdVerts[i + (int)(6 * height)].Pos,									// p6
						pStdVerts[i - 6].Pos,									// p2
						pStdVerts[i - 8].Pos,				// p1
						pStdVerts[i + 1].Pos,				// p4
						pStdVerts[i - 1].Pos,									// p8
						pStdVerts[i + (int)(6 * height) - 1].Pos									// p9

					);
				i++;

				//BL2
				pStdVerts[i].normal =
					normalHelper(

						pStdVerts[i].Pos,										// p
						pStdVerts[i - 1].Pos,									// p6
						pStdVerts[i - 6].Pos,									// p2
						pStdVerts[i - (int)(6 * height)-6].Pos,				// p1
						pStdVerts[i - (int)(6 * height)].Pos,				// p4
						pStdVerts[i + 1].Pos,									// p8
						pStdVerts[i + 2].Pos									// p9

					);
				i++;

				//UL
				pStdVerts[i].normal =
					normalHelper(

						pStdVerts[i].Pos,										// p
						pStdVerts[i + 1].Pos,									// p6
						pStdVerts[i - 1].Pos,									// p2
						pStdVerts[i - (int)(6 * height) - 1].Pos,				// p1
						pStdVerts[i - (int)(6 * height)].Pos,				// p4
						pStdVerts[i + 6].Pos,									// p8
						pStdVerts[i + 7].Pos									// p9

					);
				i++;

				//UR2
				pStdVerts[i].normal =
					normalHelper(

						pStdVerts[i].Pos,										// p
						pStdVerts[i + (int)(6 * height)].Pos,									// p6
						pStdVerts[i - 3].Pos,									// p2
						pStdVerts[i - 2].Pos,				// p1
						pStdVerts[i - 1].Pos,				// p4
						pStdVerts[i + 2].Pos,									// p8
						pStdVerts[i + (int)(6 * height) + 2].Pos									// p9

					);
				i++;
		}
	}
	//assert(numLoops == nverts);
	int j = 0;
	for (int i = 0; i < (numTris); i+=2)
	{
		pTriList[i].set(j, 
						j + 1, 
						j + 2);
		pTriList[i+1].set(j+3, 
						j + 4, 
						j + 5);
		j += 6;
	}
	//assert(numLoops == ntris);
}

Vect TerrainModel::normalHelper(Vect p, Vect p6, Vect p2, Vect p1, Vect p4, Vect p8, Vect p9) 
{
	Vect v1 = p9 - p; // urr
	Vect v2 = p6 - p;
	Vect cross1 = v1.cross(v2);

	v1 = p8 - p; // uur
	v2 = p9 - p;
	Vect cross2 = v1.cross(v2);

	v1 = p4 - p; // ul
	v2 = p8 - p;
	Vect cross3 = v1.cross(v2);

	v1 = p1 - p; // bll
	v2 = p4 - p;
	Vect cross4 = v1.cross(v2);

	v1 = p2 - p; // bbl
	v2 = p1 - p;
	Vect cross5 = v1.cross(v2);

	v1 = p6 - p; // br
	v2 = p2 - p;
	Vect cross6 = v1.cross(v2);

	Vect norm = cross1 + cross2 + cross3 + cross4 + cross5 + cross6;
	float mult = 1.0f / 6.0f;
	norm = norm * mult;
	norm = norm.GetNormalized();
	if (norm.y < 0)
		norm.set(norm.x, -norm.y, norm.z);

	if (norm.y == 0)
		norm.set(norm.x, 1, norm.z);

	return norm;
}
