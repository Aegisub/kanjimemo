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

#include <wx/wxprec.h>
#include <wx/dcmemory.h>
#include <wx/image.h>
#include "draw_text_gl.h"
using namespace Halley;


///////////////////////////////////////////////////////////////
// Get the smallest power of two that is greater or equal to x
// Code from http://bob.allegronetwork.com/prog/tricks.html
int SmallestPowerOf2(int x) {
	x--;
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);
	x++;
	return x;
}


///////////////
// Constructor
OpenGLText::OpenGLText() {
	font = shared_ptr<wxFont>(new wxFont);
	drawOutline = 0;
	outline = 0;
}


//////////////
// Destructor
OpenGLText::~OpenGLText() {
	Reset();
}


/////////
// Reset
void OpenGLText::Reset() {
	textures.clear();
	glyphs.clear();
	outlines.clear();
	drawOutline = 0;
	outline = 0;
}


////////////
// Set font
void OpenGLText::DoSetFont(std::string face,int size,bool bold,bool italics) {
	// No change required
	if (size == fontSize && face == fontFace && bold == fontBold && italics == fontItalics) return;

	// Set font
	fontFace = face;
	fontSize = size;
	fontBold = bold;
	fontItalics = italics;
	font->SetFaceName(wxString(fontFace.c_str(),wxConvUTF8));
	font->SetPointSize(size);
	font->SetWeight(bold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);

	// Delete all old data
	Reset();
}


//////////////
// Set colour
void OpenGLText::DoSetColour(Colour _col) {
	col = _col;
}


//////////////
// Set border
void OpenGLText::DoSetBorder(Colour _col, float width) {
	borderCol = _col;
	outline = width;
}


/////////
// Print
void OpenGLText::DoPrint(std::string text, Vector2f pos, float scale) {
	// Draw outline
	if (outline != 0) {
		shared_ptr<OpenGLText> bord = GetOutline(outline);
		bord->DoSetColour(borderCol);
		bord->DoPrint(text, pos, scale);
	}

	// Set OpenGL
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// Draw primary string
	glColor4f(col.r,col.g,col.b,col.a);
	DrawString(text,pos,scale);

	// Disable blend
	glDisable(GL_BLEND);
}


/////////////////
// Draw a string
void OpenGLText::DrawString(std::string text,Vector2f pos, float scale) {
	// Variables
	float x = pos.x;
	float y = pos.y;
	size_t len = text.length();
	OpenGLTextGlyph glyph;
	lineHeight = 0;
	float dx=x,dy=y;

	// Draw string
	for (size_t i=0;i<len;i++) {
		// Get current character
		int curChar = text[i];

		// Handle carriage returns
		if (curChar == '\n') {
			dx = x;
			dy -= lineHeight * scale;
		}

		// Handle normal glyphs
		else {
			glyph = GetGlyph(curChar);
			glyph.Draw(dx,dy,scale);
			float gw = glyph.w - 2*glyph.border;
			float gh = glyph.h - 2*glyph.border;
			dx += gw * scale;
			if (gh > lineHeight) lineHeight = gh;
		}
	}
}


/////////////////////////
// Calculate text extent
void OpenGLText::DoGetExtent(std::string text,Vector2f &pos) {
	// Variables
	size_t len = text.length();
	OpenGLTextGlyph glyph;
	lineHeight = 0;
	int dx=0,dy=0;
	pos.x = 0;
	pos.y = 0;

	// Simulate drawing of string
	for (size_t i=0;i<len;i++) {
		// Get current character
		int curChar = text[i];

		// Handle carriage returns
		if (curChar == '\n') {
			if (dx > pos.x) pos.x = dx;
			dx = 0;
			dy += lineHeight;
			lineHeight = 0;
		}

		// Handle normal glyphs
		else {
			glyph = GetGlyph(curChar);
			dx += glyph.w;
			if (glyph.h > lineHeight) lineHeight = glyph.h;
		}
	}

	// Return results
	if (dx > pos.x) pos.x = dx;
	pos.y = dy+lineHeight;
}


///////////////
// Get a glyph
OpenGLTextGlyph OpenGLText::GetGlyph(int i) {
	glyphMap::iterator res = glyphs.find(i);

	// Found
	if (res != glyphs.end()) return res->second;

	// Not found, create it
	return CreateGlyph(i);
}


//////////////////
// Create a glyph
OpenGLTextGlyph OpenGLText::CreateGlyph(int n) {
	// Create glyph
	OpenGLTextGlyph glyph;
	glyph.font = font;
	glyph.value = n;
	glyph.outline = drawOutline;
	glyph.GetMetrics();

	// Insert into some texture
	bool ok = false;
	for (unsigned int i=0;i<textures.size();i++) {
		if (textures[i]->TryToInsert(glyph, font, drawOutline)) {
			ok = true;
			break;
		}
	}

	// No texture could fit it, create a new one
	if (!ok) {
		textures.push_back(shared_ptr<OpenGLTextTexture>(new OpenGLTextTexture(512,512)));
		textures.back()->TryToInsert(glyph, font, drawOutline);
	}

	// Set glyph and return it
	glyphs[n] = glyph;
	return glyph;
}


///////////////////////////////
// Get an outlined text drawer
shared_ptr<OpenGLText> OpenGLText::GetOutline(float width)
{
	if (outlines.find(width) == outlines.end()) {
		shared_ptr<OpenGLText> text(new OpenGLText());
		text->DoSetFont(fontFace, fontSize, fontBold, fontItalics);
		text->drawOutline = width;
		outlines[width] = text;
	}
	
	return outlines[width];
}


///////////////////////
// Texture constructor
OpenGLTextTexture::OpenGLTextTexture(int w,int h) {
	// Properties
	x = y = 0;
	width = SmallestPowerOf2(w);
	height = SmallestPowerOf2(h);
	tex = 0;

	// Generate and bind
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

	// Allocate texture
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_ALPHA,GL_UNSIGNED_BYTE,NULL);
	if (glGetError()) throw _T("Internal OpenGL text renderer error: Could not allocate Text Texture");
}


//////////////////////
// Texture destructor
OpenGLTextTexture::~OpenGLTextTexture() {
	if (tex) {
		glDeleteTextures(1,&tex);
		tex = 0;
	}
}


//////////////////////////
// Can fit a glyph in it?
bool OpenGLTextTexture::TryToInsert(OpenGLTextGlyph &glyph, shared_ptr<wxFont> font, float outline) {
	// Get size
	int w = glyph.w;
	int h = glyph.h;
	if (w > width) return false;
	if (y+h > height) return false;

	// Can fit in this row?
	if (x + w < width) {
		Insert(glyph, font, outline);
		x += w;
		if (y+h > nextY) nextY = y+h;
		return true;
	}

	// Can fit the next row?
	else {
		if (nextY+h > height) return false;
		x = 0;
		y = nextY;
		nextY = y+h;
		Insert(glyph, font, outline);
		x += w;
		return true;
	}
}


//////////
// Insert
void OpenGLTextTexture::Insert(OpenGLTextGlyph &glyph, shared_ptr<wxFont> font, float outline) {
	// Glyph data
	wxString str = wxChar(glyph.value);
	int w = glyph.w;
	int h = glyph.h;
	int border = glyph.emptyBorder;
	int border2 = glyph.border;

	// Fill glyph structure
	glyph.x1 = float(x+border)/width;
	glyph.y1 = float(y+border)/height;
	glyph.x2 = float(x+w-border)/width;
	glyph.y2 = float(y+h-border)/height;
	glyph.tex = tex;
	glyph.font = font;
	glyph.outline = outline;

	// Create bitmap and bind it to a DC
	wxBitmap bmp(((w+1)/2)*2,((h+1)/2)*2,24);
	wxMemoryDC dc(bmp);

	// Draw text and convert to image
	dc.SetBackground(wxBrush(wxColour(0,0,0)));
	dc.Clear();
	dc.SetFont(*font);
	dc.SetTextForeground(wxColour(255,255,255));
	dc.DrawText(str,border2,border2);
	wxImage img = bmp.ConvertToImage();

	// Convert to alpha
	int imgw = img.GetWidth();
	int imgh = img.GetHeight();
	unsigned char *alpha = GetGlyphAlpha(img);
	if (outline > 0) {
		unsigned char *alpha2 = ExtendBorder(alpha, imgw, imgh, outline);
		delete[] alpha;
		alpha = alpha2;
	}
	unsigned char* lumAlpha = GetLuminanceAlpha(alpha, imgw*imgh);
	delete[] alpha;

	// Upload image to video memory
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,imgw,imgh,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,lumAlpha);
	delete[] lumAlpha;
	if (glGetError()) throw std::exception("Internal OpenGL text renderer error: Error uploading glyph data to video memory.");
}


unsigned char* OpenGLTextTexture::ExtendBorder(const unsigned char* __restrict src, size_t w, size_t h, float border)
{
	size_t len = w*h*2;
	int offset = int(ceil(border));
	int convStride = offset*2 + 1;

	unsigned char * __restrict result = new unsigned char[len];
	memset(result,0,len);
	unsigned char * __restrict conv = new unsigned char[convStride * convStride];

	// Generate convolution matrix
	for (int b=0; b<convStride; b++) {
		int convOff = b*convStride;
		for (int a=0; a<convStride; a++) {
			int convPos = convOff + a;

			// Get distance from cell to center
			int da = (a-offset);
			int db = (b-offset);
			float dist = (float)sqrt(float(da*da + db*db));

			if (dist <= border)
				conv[convPos] = 1;
			else {
				if (dist >= border+1)
					conv[convPos] = 0;
				else
					conv[convPos] = dist-border;
			}
		}
	}

	// Loop through the original image
	for (size_t y=offset; y<h-offset; y++) {
		for (size_t x=offset; x<w-offset; x++) {
			// Get the current pixel
			float value = src[y*w+x];

			// Multiply it by the convolution matrix and set values
			for (int b=0; b<convStride; b++) {
				int convOff = b*convStride;
				for (int a=0; a<convStride; a++) {
					int convPos = convOff + a;

					unsigned char* dst = result + ((y+b-offset)*w + x-offset+a);
					float thisVal = conv[convPos] * value;
					if (*dst < thisVal) *dst = thisVal;
				}
			}
		}
	}
	
	return result;
}


unsigned char* OpenGLTextTexture::Subtract(const unsigned char* img1, const unsigned char* img2, size_t w, size_t h)
{
	return NULL;
}


unsigned char* OpenGLTextTexture::GetGlyphAlpha(wxImage &img)
{
	int imgw = img.GetWidth();
	int imgh = img.GetHeight();
	size_t len = imgw*imgh;
	const unsigned char *src = img.GetData();
	const unsigned char *read = src;
	unsigned char *alpha = new unsigned char[len];
	unsigned char *write = alpha;
	for (size_t i=0;i<len;i++) {
		//*write++ = 255;
		*write++ = *read;
		read += 3;
	}

	return alpha;
}


unsigned char* OpenGLTextTexture::GetLuminanceAlpha(const unsigned char* src, size_t len)
{
	unsigned char* dst = new unsigned char[len*2];
	for (size_t i=0; i<len; i++) {
		dst[2*i] = 255;
		dst[2*i+1] = src[i];
	}
	return dst;
}



////////////////
// Draw a glyph
void OpenGLTextGlyph::Draw(float x,float y,float scale) {
	float dw = (w-2*emptyBorder)*scale;
	float dh = (h-2*emptyBorder)*scale;
	float o = outline*scale;

	// Store matrix and translate
	glPushMatrix();
	glTranslatef(x-o,y-o,0.0f);

	// Set texture
	glBindTexture(GL_TEXTURE_2D, tex);

	// Draw quad
	glBegin(GL_QUADS);
	// Top-left
	glTexCoord2f(x1,y1);
	glVertex2f(0,dh);
	// Bottom-left
	glTexCoord2f(x1,y2);
	glVertex2f(0,0);
	// Bottom-right
	glTexCoord2f(x2,y2);
	glVertex2f(dw,0);
	// Top-right
	glTexCoord2f(x2,y1);
	glVertex2f(dw,dh);
	glEnd();

	// Restore matrix
	glPopMatrix();
}


/////////////////////
// Get glyph metrics
void OpenGLTextGlyph::GetMetrics() {
	static wxBitmap *tempBmp = NULL;

	// Glyph data
	wxCoord desc,lead;
	wxString str = wxChar(value);

	// Create bitmap, if needed
	if (!tempBmp) tempBmp = new wxBitmap(16,16,24);

	// Get text extents
	{
		wxMemoryDC dc(*tempBmp);
		dc.SetFont(*font);
		dc.GetTextExtent(str,&w,&h,&desc,&lead);

		emptyBorder = 2;
		border = emptyBorder+ceil(outline);
		w += 2*border;
		h += 2*border;
	}
}
