// ShaderBase
// AB, June 2016

#ifndef _ShaderBase
#define _ShaderBase

#include "Math/Matrix.h"
#include <string>

struct ShaderInterface;

class ShaderBase : public Align16
{
protected:
	// void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layoutdesc, UINT size); // todo
	void SetToContext_VS_PS_InputLayout();

	ShaderInterface* mpShaderInterface;

public:
	virtual void SetToContext() {};

	ShaderBase(std::string filename);
	ShaderBase(const ShaderBase&) = default;				// Copy constructor
	ShaderBase(ShaderBase&&) = delete;						// Move constructor
	ShaderBase& operator=(const ShaderBase&) & = delete;	// Copy assignment operator
	ShaderBase& operator=(ShaderBase&&) & = delete;			// Move assignment operator
	ShaderBase() = delete;
	virtual ~ShaderBase();
};


#endif _ShaderBase