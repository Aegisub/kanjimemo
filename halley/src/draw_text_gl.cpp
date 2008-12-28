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


/////////
// Print
void OpenGLText::DoPrint(std::string text,Vector2f pos) {
	// Set OpenGL
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// Draw border
	/*
	float x = pos.x;
	float y = pos.y;
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	DrawString(text,x-1,y);
	DrawString(text,x+1,y);
	DrawString(text,x,y-1);
	DrawString(text,x,y+1);
	*/

	// Draw primary string
	glColor4f(col.r,col.g,col.b,col.a);
	DrawString(text,pos);

	// Disable blend
	glDisable(GL_BLEND);
}


/////////////////
// Draw a string
void OpenGLText::DrawString(std::string text,Vector2f pos) {
	// Variables
	float x = pos.x;
	float y = pos.y;
	size_t len = text.length();
	OpenGLTextGlyph glyph;
	lineHeight = 0;
	int dx=x,dy=y;

	// Draw string
	for (size_t i=0;i<len;i++) {
		// Get current character
		int curChar = text[i];

		// Handle carriage returns
		if (curChar == '\n') {
			dx = x;
			dy -= lineHeight;
		}

		// Handle normal glyphs
		else {
			glyph = GetGlyph(curChar);
			glyph.Draw(dx,dy);
			dx += glyph.w;
			if (glyph.h > lineHeight) lineHeight = glyph.h;
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
	glyph.GetMetrics();

	// Insert into some texture
	bool ok = false;
	for (unsigned int i=0;i<textures.size();i++) {
		if (textures[i]->TryToInsert(glyph, font)) {
			ok = true;
			break;
		}
	}

	// No texture could fit it, create a new one
	if (!ok) {
		textures.push_back(shared_ptr<OpenGLTextTexture>(new OpenGLTextTexture(256,256)));
		textures.back()->TryToInsert(glyph, font);
	}

	// Set glyph and return it
	glyphs[n] = glyph;
	return glyph;
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
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
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
bool OpenGLTextTexture::TryToInsert(OpenGLTextGlyph &glyph, shared_ptr<wxFont> font) {
	// Get size
	int w = glyph.w;
	int h = glyph.h;
	if (w > width) return false;
	if (y+h > height) return false;

	// Can fit in this row?
	if (x + w < width) {
		Insert(glyph, font);
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
		Insert(glyph, font);
		return true;
	}
}


//////////
// Insert
void OpenGLTextTexture::Insert(OpenGLTextGlyph &glyph, shared_ptr<wxFont> font) {
	// Glyph data
	wxString str = wxChar(glyph.value);
	int w = glyph.w;
	int h = glyph.h;

	// Fill glyph structure
	glyph.x1 = float(x)/width;
	glyph.y1 = float(y)/height;
	glyph.x2 = float(x+w)/width;
	glyph.y2 = float(y+h)/height;
	glyph.tex = tex;
	glyph.font = font;

	// Create bitmap and bind it to a DC
	wxBitmap bmp(((w+1)/2)*2,((h+1)/2)*2,24);
	wxMemoryDC dc(bmp);

	// Draw text and convert to image
	dc.SetBackground(wxBrush(wxColour(0,0,0)));
	dc.Clear();
	dc.SetFont(*font);
	dc.SetTextForeground(wxColour(255,255,255));
	dc.DrawText(str,0,0);
	//bmp.SaveFile(wxString::Format(_T("glyph%i.bmp"),glyph.value),wxBITMAP_TYPE_BMP);
	wxImage img = bmp.ConvertToImage();
	//img.SaveFile(str + _T(".bmp"));

	// Convert to alpha
	int imgw = img.GetWidth();
	int imgh = img.GetHeight();
	size_t len = imgw*imgh;
	const unsigned char *src = img.GetData();
	const unsigned char *read = src;
	unsigned char *alpha = new unsigned char[len*2];
	unsigned char *write = alpha;
	for (size_t i=0;i<len;i++) {
		*write++ = 255;
		*write++ = *read++;
		read += 2;
	}

	// Upload image to video memory
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,imgw,imgh,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,alpha);
	delete[] alpha;
	if (glGetError()) throw _T("Internal OpenGL text renderer error: Error uploading glyph data to video memory.");
}


////////////////
// Draw a glyph
void OpenGLTextGlyph::Draw(int x,int y) {
	// Store matrix and translate
	glPushMatrix();
	glTranslatef((float)x,(float)y,0.0f);

	// Set texture
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Draw quad
	glBegin(GL_QUADS);
	// Top-left
	glTexCoord2f(x1,y1);
	glVertex2f(0,h);
	// Bottom-left
	glTexCoord2f(x1,y2);
	glVertex2f(0,0);
	// Bottom-right
	glTexCoord2f(x2,y2);
	glVertex2f(w,0);
	// Top-right
	glTexCoord2f(x2,y1);
	glVertex2f(w,h);
	glEnd();

	// Restore matrix
	glPopMatrix();
}


/////////////////////
// Get glyph metrics
wxBitmap *OpenGLTextGlyph::tempBmp = NULL;
void OpenGLTextGlyph::GetMetrics() {
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
	}
}
