#ifndef IMAGE_H
#define IMAGE_H

#include "Rect.h"
#include "Texture.h"

// Implicitly owns the rect... fow now (todo)
class Image
{
public:
	// public methods: -------------------------------------------------------------
	Image()
		: pText(nullptr), pImageRect(nullptr) {}

	Image(Texture* pTexture, const Rect& inRect)
		: pText(pTexture), pImageRect(new Rect(inRect)) {}

	void Set(Texture* pTexture, const Rect& inRect)
	{
		pText = pTexture;
		pImageRect = new Rect(inRect);
	}

	~Image()
	{
		delete pImageRect;
	}

public:
	// data: -----------------------------------------------------------------------
	Rect* pImageRect;
	Texture* pText;
};


#endif