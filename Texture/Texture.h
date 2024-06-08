#ifndef _Texture
#define _Texture

#include <Windows.h>
#include <string>
#include "DirectXTex.h"

struct TextureSampler;

class Texture
{
public:
	Texture(const Texture&) = default;				 // Copy constructor
	Texture(Texture&&) = delete;                    // Move constructor
	Texture& operator=(const Texture&) & = delete;  // Copy assignment operator
	Texture& operator=(Texture&&) & = delete;       // Move assignment operator
	~Texture();									     // Destructor
	Texture() = delete;

	Texture(std::string filepath);
	Texture(std::string filepath, uint32_t filter, uint32_t isotropic_level, bool ComputeMip, size_t miplevel, uint32_t filterflags);
	Texture(float r, float g, float b, float a = 1.0f);

	void SetToContext(int texResSlot = 0, int sampSlot = 0);
	
	int GetWidth();
	int GetHeight();

private:
	TextureSampler* mpTextureSampler;

};

#endif _Texture