#ifndef _ShaderColor
#define _ShaderColor

#include "ShaderBase.h"
#include "../Math/Matrix.h"

template <typename T>
struct GenericBufferObject;

class ShaderColor : public ShaderBase
{
public:
	ShaderColor(const ShaderColor&) = delete;				 
	ShaderColor(ShaderColor&&) = default;                    
	ShaderColor& operator=(const ShaderColor&) & = default;  
	ShaderColor& operator=(ShaderColor&&) & = default;       
	~ShaderColor();		  							         

	ShaderColor(std::string filename);
	ShaderColor() = delete;

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

#endif _ShaderColor
