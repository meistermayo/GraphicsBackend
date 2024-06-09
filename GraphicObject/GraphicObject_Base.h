// GraphicObject
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Base
#define _GraphicObject_Base

#include "../Math/Matrix.h"

class Model;
class ShaderMeshData;
struct ID3D11DeviceContext;
class ShaderBase;
class Camera;

class GraphicObject_Base : public Align16
{
public:
	GraphicObject_Base() : pModel(nullptr), mWorld(Matrix::Identity) {};
	~GraphicObject_Base() = default;

	GraphicObject_Base(const GraphicObject_Base&) = delete;				  
	GraphicObject_Base(GraphicObject_Base&&) = default;                   
	GraphicObject_Base& operator=(const GraphicObject_Base&) = default; 
	GraphicObject_Base& operator=(GraphicObject_Base&&) = default;   

	virtual void Render(Camera* inCamera) = 0;

	Model* GetModel() { return pModel; }
	void SetModel(Model* inModel) { pModel = inModel; }

	virtual const Matrix& GetWorld() const { return mWorld; }
	void SetWorld(const Matrix& inWorld) { mWorld = inWorld; }

protected:
	Model* pModel;
	Matrix mWorld;
};

#endif _GraphicObject_Base
