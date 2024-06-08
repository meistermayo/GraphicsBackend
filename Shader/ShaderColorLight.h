// ShaderColorLight
// Andre Berthiaume, Feb 2017

#ifndef _ShaderColorLight
#define _ShaderColorLight

#include "ShaderBase.h"
#include "Math/Matrix.h"

template <typename T>
struct GenericBufferObject;

class ShaderColorLight : public ShaderBase
{
	friend class GraphicObject_ColorLight;
public:
	ShaderColorLight(const ShaderColorLight&) = delete;				 // Copy constructor
	ShaderColorLight(ShaderColorLight&&) = default;                    // Move constructor
	ShaderColorLight& operator=(const ShaderColorLight&) & = default;  // Copy assignment operator
	ShaderColorLight& operator=(ShaderColorLight&&) & = default;       // Move assignment operator
	~ShaderColorLight();		  							         // Destructor

	ShaderColorLight(std::string filename);
	ShaderColorLight() = delete;

	virtual void SetToContext() override;

	void SetDirectionalLightParameters(const Vect& dir, const Vect& amb = Vect::One, const Vect& dif = Vect::One, const Vect& sp = Vect::One);
	void SetPointLightParameters1(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect::One, const Vect& dif = Vect::One, const Vect& sp = Vect::One);
	void SetPointLightParameters2(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect::One, const Vect& dif = Vect::One, const Vect& sp = Vect::One);
	void SetPointLightParameters3(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect::One, const Vect& dif = Vect::One, const Vect& sp = Vect::One);
	void SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb = Vect::One, const Vect& dif = Vect::One, const Vect& sp = Vect::One);

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendLightParameters(const Vect& eyepos);
	void SendWorldAndMaterial(const Matrix& world, const Vect& amb = Vect(.5f, .5f, .5f), const Vect& dif = Vect(.5f, .5f, .5f), const Vect& sp = Vect(.5f, .5f, .5f));
	void SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol);

private:
	struct Material : Align16
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

	DirectionalLight DirLightData;

	struct PointLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		float Range;
	};

	PointLight PointLightData1;
	PointLight PointLightData2;
	PointLight PointLightData3;

	struct SpotLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		Vect Direction;
		float SpotExp;
		float Range;
	};

	SpotLight SpotLightData;

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	GenericBufferObject<CamMatrices>*  mpBufferCamMatrices;

	struct Data_WorldAndMaterial
	{
		Matrix World;
		Matrix WorlInv;
		Material Mat;
	};

	GenericBufferObject<Data_WorldAndMaterial>*	mpBuffWordAndMaterial;

	struct Data_LightParams
	{
		DirectionalLight DirLight;
		PointLight PntLight1;
		PointLight PntLight2;
		PointLight PntLight3;
		SpotLight SptLight;
		Vect EyePosWorld;
	};

	GenericBufferObject<Data_LightParams>*  mpBufferLightParams;
	public:
		struct FogData {
			float fogMin;
			float fogMax;
			Vect fogCol;
		};

private:
	GenericBufferObject<FogData>* mpFog;
};

#endif _ShaderColorLight

