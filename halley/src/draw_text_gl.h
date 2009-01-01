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
#include "interfaces.h"
#include <map>
#include <vector>

class wxBitmap;
class wxString;
class wxFont;
class wxImage;

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
		int emptyBorder;
		float outline;
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

		void Insert(OpenGLTextGlyph &glyph, shared_ptr<wxFont> font, float outline);
		unsigned char* GetGlyphAlpha(wxImage &img);
		unsigned char* ExtendBorder(const unsigned char* img, size_t w, size_t h, float borderWidth);
		unsigned char* Subtract(const unsigned char* img1, const unsigned char* img2, size_t w, size_t h);
		unsigned char* GetLuminanceAlpha(const unsigned char* img, size_t len);
		unsigned char* GetConvolutionMatrix(float border);

	public:
		GLuint tex;

		bool TryToInsert(OpenGLTextGlyph &glyph, shared_ptr<wxFont> font, float outline);

		OpenGLTextTexture(int w,int h);
		~OpenGLTextTexture();
	};


	/////////////////////////////
	// OpenGL Text Drawing class
	class OpenGLText : public TextDrawer {
		friend class TextDrawer;

	private:
		std::map<float, shared_ptr<OpenGLText> > outlines;

		Colour col, borderCol;
		int lineHeight;
		int fontSize;
		bool fontBold;
		bool fontItalics;
		float outline;
		float drawOutline;
		String fontFace;
		shared_ptr<wxFont> font;

		glyphMap glyphs;
		std::vector<shared_ptr<OpenGLTextTexture> > textures;

		OpenGLText();

		OpenGLTextGlyph GetGlyph(int i);
		OpenGLTextGlyph CreateGlyph(int i);
		void Reset();
		shared_ptr<OpenGLText> GetOutline(float width);

		void DrawString(String text,Vector2f pos, float scale);

	public:
		virtual void SetFont(String face,int size,bool bold,bool italics);
		virtual void SetColour(Colour col);
		virtual void SetBorder(Colour col, float width);
		virtual void Print(String text, Vector2f pos, float scale=1.0f);
		virtual void GetExtent(String text, Vector2f &pos);

		virtual void LoadGlyphs(String text);
		virtual void Clear();

		~OpenGLText();
	};
}
