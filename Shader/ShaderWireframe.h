#pragma once

#include "ShaderBase.h"
#include "../Math/Matrix.h"

template <typename T>
struct GenericBufferObject;

class ShaderWireframe : public ShaderBase
{
public:
	ShaderWireframe(const ShaderWireframe&) = delete;				
	ShaderWireframe(ShaderWireframe&&) = default;                   
	ShaderWireframe& operator=(const ShaderWireframe&) & = default; 
	ShaderWireframe& operator=(ShaderWireframe&&) & = default;      
	~ShaderWireframe();		  										

	ShaderWireframe(const std::string& filename);
	ShaderWireframe() = delete;

	virtual void SetToContext() override;

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendWorldColor(const Matrix& world, const Vect& col);
	void SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol);
	void GetEyePos(const Vect& _eyepos);

private:
	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	GenericBufferObject<CamMatrices>*           mpBufferCamMatrices;

	struct Data_WorldColor
	{
		Matrix World;
		Matrix WorldInv;
		Vect Color;
	};

	GenericBufferObject<Data_WorldColor>*           mpBuffWordColor;
public:
	struct FogData {
		float fogMin;
		float fogMax;
		Vect fogCol;
		Vect eyePos;
	};
private:
	GenericBufferObject<FogData>* mpFog;
	Vect eyepos;
};
