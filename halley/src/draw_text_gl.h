/*********************************************************\
         __      __           __  __
        / /     / / _____    / / / / ______   __     __
       / /     / / /___  \  / / / / / ___  | / /    / /
      / /_____/ / _____| / / / / / / /__/ / / /    / /
     / ______  / / ___  / / / / / / _____/ / /    / /
    / /     / / / /__/ / / / / / / |____  / |____/ /
   /_/     /_/ |______/ /_/ /_/  \_____/  \_____  /
                                         ______/ /
            High Level Game Framework   /_______/

  ------------------------------------------------------

  Copyright (c) 2008 - Rodrigo Braz Monteiro.
  This file is subject to the terms of license.txt.

\*********************************************************/

#pragma once

#include "draw_text.h"
#include "SDL_opengl.h"
#include <map>
#include <vector>

class wxBitmap;
class wxString;
class wxFont;

namespace Halley {
	/////////////////////
	// Glyph information
	class OpenGLTextGlyph {
	public:
		int value;
		int tex;
		float x1,y1,x2,y2;
		int w,h;
		int border;
		shared_ptr<wxFont> font;

		void GetMetrics();
		void Draw(float x, float y, float scale);
	};

	typedef std::map<int,OpenGLTextGlyph> glyphMap;


	///////////////
	// Texture map
	class OpenGLTextTexture {
	private:
		int x,y,nextY;
		int width,height;

		void Insert(OpenGLTextGlyph &glyph, shared_ptr<wxFont> font);

	public:
		GLuint tex;

		bool TryToInsert(OpenGLTextGlyph &glyph, shared_ptr<wxFont> font);

		OpenGLTextTexture(int w,int h);
		~OpenGLTextTexture();
	};


	/////////////////////////////
	// OpenGL Text Drawing class
	class OpenGLText : public TextDrawer {
		friend class TextDrawer;

	private:
		Colour col;
		int lineHeight;
		int fontSize;
		bool fontBold;
		bool fontItalics;
		std::string fontFace;
		shared_ptr<wxFont> font;

		glyphMap glyphs;
		std::vector<shared_ptr<OpenGLTextTexture> > textures;

		OpenGLText();

		OpenGLTextGlyph GetGlyph(int i);
		OpenGLTextGlyph CreateGlyph(int i);
		void Reset();

		void DrawString(std::string text,Vector2f pos, float scale);

	protected:
		virtual void DoSetFont(std::string face,int size,bool bold,bool italics);
		virtual void DoSetColour(Colour col);
		virtual void DoPrint(std::string text, Vector2f pos, float scale);
		virtual void DoGetExtent(std::string text, Vector2f &pos);

	public:
		~OpenGLText();
	};
}
