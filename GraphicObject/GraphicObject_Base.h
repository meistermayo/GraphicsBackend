// GraphicObject
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Base
#define _GraphicObject_Base

#include "Math/Matrix.h"

class Model;
class ShaderMeshData;
struct ID3D11DeviceContext;
class ShaderBase;
class Camera;


class GraphicObject_Base : public Align16
{
public:
	GraphicObject_Base(const GraphicObject_Base&) = delete;				   // Copy constructor
	GraphicObject_Base(GraphicObject_Base&&) = default;                    // Move constructor
	GraphicObject_Base& operator=(const GraphicObject_Base&) & = default;  // Copy assignment operator
	GraphicObject_Base& operator=(GraphicObject_Base&&) & = default;       // Move assignment operator
	~GraphicObject_Base() = default;		  							   // Destructor
	GraphicObject_Base();

	Model* GetModel() { return pModel; }
	void SetModel(Model* mod);
	virtual void Render(Camera* inCamera) = 0;
	virtual Matrix& GetWorld() const { return *pWorld; }
	void SetWorld(const Matrix& m) { *pWorld = m; }

protected:
	Model* pModel;
	Matrix* pWorld;
};

#endif _GraphicObject_Base
