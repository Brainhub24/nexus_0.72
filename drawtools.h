// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

class CNexDraw {
	public:
		int		CG_Text_Width_Ext(const char *text, float scale, int limit, fontInfo_t* font);
		void	CG_Text_PaintChar_Ext(float x, float y, float w, float h, float scalex, float scaley, float s, float t, float s2, float t2, qhandle_t hShader);
		void	CG_Text_Paint_Ext(float x, float y, float scalex, float scaley, vec4_t color, const char *text, float adjust, int limit, int style, fontInfo_t* font);
		void	adjustFrom640(float *x, float *y, float *w, float *h);
		void	adjustTo640(float *x, float *y, float *w, float *h);
		void	drawRect(float x, float y, float width, float height, vec4_t color, int borderSize);
		void	drawFillRect(float x, float y, float width, float height, const float *color);
		void	drawPic(float x, float y, float width, float height, qhandle_t hShader);
		bool	worldToScreen(vec3_t worldCoord, int *x, int *y);
		void	drawText(float x, float y, float scale, char *text, vec4_t color, qboolean forceColor, qboolean shadow, fontInfo_t *font);
		int		sizeTextHeight(float scale, fontInfo_t *font);
		int		sizeTextWidth(float scale, char *text, fontInfo_t *font);
		void	drawTextRect(float x, float y, float scale, char *text, vec4_t textColor, qboolean forceColor, qboolean textShadow, qboolean background, qboolean border, vec4_t backgroundColor, vec4_t borderColor, fontInfo_t *font);
	private:
};

extern CNexDraw nexDraw;
