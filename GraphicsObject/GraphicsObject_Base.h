// GraphicsObject
// Andre Berthiaume, July 2016

#ifndef _GraphicsObject_Base
#define _GraphicsObject_Base

#include "../Math/Matrix.h"

class Model;
class ShaderMeshData;
struct ID3D11DeviceContext;
class ShaderBase;
class Camera;

class GraphicsObject_Base : public Align16
{
public:
	GraphicsObject_Base() : pModel(nullptr), mWorld(Matrix::Identity) {};
	~GraphicsObject_Base() = default;

	GraphicsObject_Base(const GraphicsObject_Base&) = delete;				  
	GraphicsObject_Base(GraphicsObject_Base&&) = default;                   
	GraphicsObject_Base& operator=(const GraphicsObject_Base&) = default; 
	GraphicsObject_Base& operator=(GraphicsObject_Base&&) = default;   

	virtual void Render(Camera* inCamera) = 0;

	Model* GetModel() { return pModel; }
	void SetModel(Model* inModel) { pModel = inModel; }

	virtual const Matrix& GetWorld() const { return mWorld; }
	void SetWorld(const Matrix& inWorld) { mWorld = inWorld; }

protected:
	Model* pModel;
	Matrix mWorld;
};

#endif _GraphicsObject_Base
