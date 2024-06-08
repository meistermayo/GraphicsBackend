#include "ModelTools.h"
//#include "Enum.h"
//#include <math.h>
#include "Math/Matrix.h"
#include "Math/Constants.h"
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"
#include "math.h"

/// Creates the unit box centered at the origin
void ModelTools::CreateUnitBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri)
{
	nverts = 8;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(-0.5f, -0.5f, -0.5f, Colors::Black);
	pVerts[1].set(-0.5f, +0.5f, -0.5f, Colors::Lime);
	pVerts[2].set(+0.5f, +0.5f, -0.5f, Colors::Yellow);
	pVerts[3].set(+0.5f, -0.5f, -0.5f, Colors::Red);
	pVerts[4].set(-0.5f, -0.5f, +0.5f, Colors::Blue);
	pVerts[5].set(-0.5f, +0.5f, +0.5f, Colors::Cyan);
	pVerts[6].set(+0.5f, +0.5f, +0.5f, Colors::White);
	pVerts[7].set(+0.5f, -0.5f, +0.5f, Colors::Magenta);

	// back face
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	// front face
	pTriList[2].set(4, 6, 5);
	pTriList[3].set(4, 7, 6);

	// left face
	pTriList[4].set(4, 5, 1);
	pTriList[5].set(4, 1, 0);

	// right face
	pTriList[6].set(3, 2, 6);
	pTriList[7].set(3, 6, 7);

	// top face
	pTriList[8].set(1, 5, 6);
	pTriList[9].set(1, 6, 2);

	// bottom face
	pTriList[10].set(4, 0, 3);
	pTriList[11].set(4, 3, 7);
}

void ModelTools::CreateUnitPlaneXY(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 4;
	pVerts = new StandardVertex[nverts];
	ntri = 2;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(-0.5f, -0.5f, -0.0f,	-0.5f, -0.5f,	Colors::Red);
	pVerts[1].set(0.5f, -0.5f, -0.0f,	0.5f, -0.5f,	Colors::Green);
	pVerts[2].set(-0.5f, 0.5f, -0.0f,	-0.5f, 0.5f,	Colors::Blue);
	pVerts[3].set(0.5f, 0.5f, -0.0f,	0.5f, 0.5f,		Colors::White);

	pTriList[0].set(2, 1, 0);
	pTriList[1].set(0, 2, 3);
}

void ModelTools::CreateUnitPlaneXZ(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 4;
	pVerts = new StandardVertex[nverts];
	ntri = 2;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(-0.5f, -0.0f,	-0.5f,		-0.5f, -0.5f,	Colors::Red);
	pVerts[1].set(0.5f,	 -0.0f,	-0.5f,		0.5f, -0.5f,	Colors::Green);
	pVerts[2].set(-0.5f,-0.0f,	0.5f,		-0.5f, 0.5f,	Colors::Blue);
	pVerts[3].set(0.5f,	-0.0f,	0.5f,		0.5f, 0.5f,		 Colors::White);

	pTriList[0].set(3, 1, 0);
	pTriList[1].set(0, 2, 3);
}

void ModelTools::CreateUnitBoxRepTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1);
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 1);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 1, 0, 0, 1);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 0, 0, -1);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 1, 0, 0, 0, -1);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 1, 1, 0, 0, -1);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 1, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 1, 0, 0);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 1, 0, -1, 0, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, -1, 0, 0);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 1, 1, -1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 0, 1, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, 0, 1, 0);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 1, 1, 0, 1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 0, 0, -1, 0);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 0, 0, -1, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, 0, -1, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitPyramid(StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri)
{
	nverts = 5;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(0, 1.0f, 0, Colors::White);
	pVerts[1].set(0.5f, 0, 0.5f, Colors::White);
	pVerts[2].set(-0.5f, 0, 0.5f, Colors::White);
	pVerts[3].set(-0.5f, 0, -0.5f, Colors::White);
	pVerts[4].set(0.5f, 0, -0.5f, Colors::White);


	pTriList[0].set(0, 2, 1);
	pTriList[1].set(0, 3, 2);
	pTriList[2].set(0, 4, 3);
	pTriList[3].set(0, 1, 4);
	/*/
	pTriList[4].set(3, 2, 1); // Bottoms up?
	pTriList[5].set(3, 1, 4);
	/*/
	pTriList[4].set(3, 1, 2);
	pTriList[5].set(3, 4, 1);
	//*/

}


void ModelTools::CreateUnitPyramidRepTexture(StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri)
{
	nverts = 18;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	/*
	pVerts[0].set(0, 1.0f, 0, 0,0);
	pVerts[1].set(0.5f, 0, 0.5f, 1,0);
	pVerts[2].set(-0.5f, 0, 0.5f, 0,1);
	pVerts[3].set(-0.5f, 0, -0.5f, 1,1);
	pVerts[4].set(0.5f, 0, -0.5f, 0,0);
	*/

	pVerts[0].set(0, 1.0f, 0, 0.5f, 0, 000, 1.0f, 0); //
	pVerts[1].set(-0.5f, 0, 0.5f, 0, 1, -0.5f, 0, 0.5f);
	pVerts[2].set(0.5f, 0, 0.5f, 1, 1, 0.5f, 0, 0.5f);

	pVerts[3].set(0, 1.0f, 0, 0.5f, 0, 000, 1.0f, 0); //
	pVerts[4].set(-0.5f, 0, -0.5f, 0, 1, -0.5f, 0, -0.5f);
	pVerts[5].set(-0.5f, 0, 0.5f, 1, 1, -0.5f, 0, 0.5f);

	pVerts[6].set(0, 1.0f, 0, 0.5f, 0, 000, 1.0f, 0); //
	pVerts[7].set(0.5f, 0, -0.5f, 0, 1, 0.5f, 0, -0.5f);
	pVerts[8].set(-0.5f, 0, -0.5f, 1, 1, -0.5f, 0, -0.5f);

	pVerts[9].set(0, 1.0f, 0, 0.5f, 0, 000, 1.0f, 0); //
	pVerts[10].set(0.5f, 0, 0.5f, 0, 1, 0.5f, 0, 0.5f);
	pVerts[11].set(0.5f, 0, -0.5f, 1, 1, 0.5f, 0, -0.5f);

	pVerts[12].set(-0.5f, 0, -0.5f, 1, 0, -0.5f, -0.5f, -0.5f);
	pVerts[13].set(0.5f, 0, 0.5f, 0, 1, 0.5f, -0.5f, 0.5f);
	pVerts[14].set(-0.5f, 0, 0.5f, 1, 1, -0.5f, -0.5f, 0.5f);

	pVerts[15].set(-0.5f, 0, -0.5f, 1, 0, -0.5f, -0.5f, -0.5f);
	pVerts[16].set(0.5f, 0, -0.5f, 0, 0, 0.5f, -0.5f, -0.5f);
	pVerts[17].set(0.5f, 0, 0.5f, 0, 1, 0.5f, -0.5f, 0.5f);

	pTriList[0].set(0, 1, 2);
	pTriList[1].set(3, 4, 5);
	pTriList[2].set(6, 7, 8);
	pTriList[3].set(9, 10, 11);
	/*/
	pTriList[4].set(3, 2, 1); // Bottoms up?
	pTriList[5].set(3, 1, 4);
	/*/
	pTriList[4].set(12, 13, 14);
	pTriList[5].set(15, 16, 17);
	//*/

}

void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri, float scale)
{
	nverts = (vslice * hslice) + 1;
	ntri = (hslice - 1) * vslice * 2;
	pVerts = new StandardVertex[nverts];
	pTriList = new TriangleByIndex[ntri];

	//Verts
	{
		float angle1 = MATH_PI / vslice;
		float angle2 = 2 * MATH_PI / hslice;

		for (int i = 0; i < vslice; i++)
		{
			float sin1 = sinf(angle1 * i);
			float cos1 = cosf(angle1 * i);

			for (int j = 0; j < hslice; j++)
			{
				float sin2 = sinf(angle2 * j);
				float cos2 = cosf(angle2 * j);

				int ind = j + (i * hslice);
				assert(ind < nverts);
				pVerts[ind].set(sin1*cos2*scale, cos1*scale, sin1*sin2*scale, (float)i / (float)vslice, (float)j / (float)hslice, sin1*cos2, cos1, sin1*sin2);
			}
		}
		assert((vslice*hslice) < nverts);
		pVerts[vslice*hslice].set(0.0f, -1.0f, 0.0f, 1, 1, 0.0f, -1.0f, 0.0f);
	}

	//Tris
	{
		int tri = 0;

		int f = hslice + 1;
		int s = hslice + 1;
		int l = hslice;

		for (int i = 0; i < hslice - 1; i++)
		{
			assert(tri < ntri);
			pTriList[tri].set(0, s++, l++);
			tri++;
		}
		assert(tri < ntri);
		pTriList[tri].set(0, hslice, (2 * hslice) - 1);
		tri++;


		for (int i = 1; i < vslice - 1; i++)
		{
			f = hslice * i + 1;
			s = f + hslice - 1;
			l = hslice * i;

			for (int j = 0; j < hslice - 1; j++)
			{
				assert(tri < ntri);
				pTriList[tri].set(f, s++, l);
				tri++;

				assert(tri < ntri);
				pTriList[tri].set(f++, s, l + hslice);
				tri++;
				l++;
			}
			f = hslice * i;
			assert(tri < ntri);
			pTriList[tri].set(f, s++, l);
			tri++;
			s = hslice + (hslice*i);
			assert(tri < ntri);
			pTriList[tri].set(f, s, l + hslice);
			tri++;
		}

		f = hslice * vslice;
		s = f - 2;
		l = f - 1;

		for (int i = 0; i < hslice - 1; i++)
		{
			assert(tri < ntri);
			pTriList[tri].set(f, s--, l--);
			tri++;
		}

		assert(tri < ntri);
		pTriList[tri].set(f, f - 1, f - hslice);
		tri++;
	}
}
