// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexDraw nexDraw;

int CNexDraw::CG_Text_Width_Ext(const char *text, float scale, int limit, fontInfo_t* font) {
	int count, len;
	glyphInfo_t *glyph;
	const char *s = text;
	float out, useScale = scale * font->glyphScale;
	
	out = 0;
	if( text ) {
		len = (int) strlen( text );
		if (limit > 0 && len > limit) {
			len = limit;
		}
		count = 0;
		while (s && *s && count < len) {
			if ( Q_IsColorString(s) ) {
				s += 2;
				continue;
			} else {
				glyph = &font->glyphs[(unsigned char)*s];
				out += glyph->xSkip;
				s++;
				count++;
			}
		}
	}

	return out * useScale;
}

void CNexDraw::CG_Text_PaintChar_Ext(float x, float y, float w, float h, float scalex, float scaley, float s, float t, float s2, float t2, qhandle_t hShader) {
	w *= scalex;
	h *= scaley;
	adjustFrom640(&x, &y, &w, &h);
	nexSyscall.R_DrawStretchPic(x, y, w, h, s, t, s2, t2, hShader);
}

void CNexDraw::CG_Text_Paint_Ext(float x, float y, float scalex, float scaley, vec4_t color, const char *text, float adjust, int limit, int style, fontInfo_t* font) {
	int len, count;
	vec4_t newColor;
	glyphInfo_t *glyph;

	scalex *= font->glyphScale;
	scaley *= font->glyphScale;

	if (text) {
		const char *s = text;
		nexSyscall.R_SetColor( color );
		memcpy(&newColor[0], &color[0], sizeof(vec4_t));
		len = (int) strlen(text);
		if (limit > 0 && len > limit) {
			len = limit;
		}
		count = 0;
		while (s && *s && count < len) {
			glyph = &font->glyphs[(unsigned char)*s];
			if ( Q_IsColorString( s ) ) {
				if( *(s+1) == COLOR_NULL ) {
					memcpy( newColor, color, sizeof(newColor) );
				} else {
					memcpy( newColor, g_color_table[ColorIndex(*(s+1))], sizeof( newColor ) );
					newColor[3] = color[3];
				}
				nexSyscall.R_SetColor(newColor);
				s += 2;
				continue;
			} else {
				float yadj = scaley * glyph->top;
				yadj -= (font->glyphs['['].top + 3) * scaley;	// eth: Add y correction
				if (style == ITEM_TEXTSTYLE_SHADOWED || style == ITEM_TEXTSTYLE_SHADOWEDMORE) {
					int ofs = style == ITEM_TEXTSTYLE_SHADOWED ? 1 : 2;
					colorBlack[3] = newColor[3];
					nexSyscall.R_SetColor( colorBlack );
					CG_Text_PaintChar_Ext(x + (glyph->pitch * scalex) + ofs, y - yadj + ofs, glyph->imageWidth, glyph->imageHeight, scalex, scaley, glyph->s, glyph->t, glyph->s2, glyph->t2, glyph->glyph);
					colorBlack[3] = 1.0;
					nexSyscall.R_SetColor( newColor );
				}
				CG_Text_PaintChar_Ext(x + (glyph->pitch * scalex), y - yadj, glyph->imageWidth, glyph->imageHeight, scalex, scaley, glyph->s, glyph->t, glyph->s2, glyph->t2, glyph->glyph);
				x += (glyph->xSkip * scalex) + adjust;
				s++;
				count++;
			}
		}
		nexSyscall.R_SetColor( NULL );
	}

}

void CNexDraw::adjustFrom640(float *x, float *y, float *w, float *h) {
	*x *= nex.cgs_screenXScale;
	*y *= nex.cgs_screenYScale;
	*w *= nex.cgs_screenXScale;
	*h *= nex.cgs_screenYScale;
}

void CNexDraw::adjustTo640(float *x, float *y, float *w, float *h) {
	*x /= nex.cgs_screenXScale;
	*y /= nex.cgs_screenYScale;
	*w /= nex.cgs_screenXScale;
	*h /= nex.cgs_screenYScale;
}

void CNexDraw::drawRect(float x, float y, float width, float height, vec4_t color, int borderSize) {
	nexSyscall.R_SetColor(color);

	adjustFrom640(&x, &y, &width, &height);
	nexSyscall.R_DrawStretchPic(x, y, borderSize, height, 0, 0, 0, 0, nex.media.whiteShader);
	nexSyscall.R_DrawStretchPic(x + width - borderSize, y, borderSize, height, 0, 0, 0, 0, nex.media.whiteShader);
	nexSyscall.R_DrawStretchPic(x, y, width, borderSize, 0, 0, 0, 0, nex.media.whiteShader);
	nexSyscall.R_DrawStretchPic(x, y + height - borderSize, width, borderSize, 0, 0, 0, 0, nex.media.whiteShader);

	nexSyscall.R_SetColor(NULL);
}

void CNexDraw::drawFillRect(float x, float y, float width, float height, const float *color) {
	nexSyscall.R_SetColor(color);

    adjustFrom640(&x, &y, &width, &height);
    nexSyscall.R_DrawStretchPic(x, y, width, height, 0, 0, 0, 1, nex.media.whiteShader);

	nexSyscall.R_SetColor(NULL);
}

void CNexDraw::drawPic(float x, float y, float width, float height, qhandle_t hShader) {
	adjustFrom640(&x, &y, &width, &height);
	nexSyscall.R_DrawStretchPic(x, y, width, height, 0, 0, 1, 1, hShader);
}

bool CNexDraw::worldToScreen(vec3_t worldCoord, int *x, int *y) {
	vec3_t local, transformed;
	vec3_t vfwd;
	vec3_t vright;
	vec3_t vup;
	float xzi;
	float yzi;
	
	AngleVectors(nex.cg_refdefViewAngles, vfwd, vright, vup);
	VectorSubtract(worldCoord, nex.cg_refdef.vieworg, local);
	
	transformed[0] = DotProduct (local, vright);
	transformed[1] = DotProduct (local, vup);
	transformed[2] = DotProduct (local, vfwd);
	
	// Point is behind
	if (transformed[2] < 0.01)
		return false;

	switch (nex.cg_snap->ps.weapon) {
		case WP_FG42SCOPE:
		case WP_GARAND_SCOPE:
		case WP_K43_SCOPE:
			xzi = 320.0f / transformed[2];
			yzi = 240.0f / transformed[2];
	   		break;
		default:
			xzi = 320.0f / transformed[2] * (90.0 / nex.cg_refdef.fov_x);
			yzi = 240.0f / transformed[2] * (90.0 / nex.cg_refdef.fov_y);
			break;
	}
	
	*x = 320 + xzi * transformed[0];
	*y = 240 - yzi * transformed[1];

	return true;
}

void CNexDraw::drawText(float x, float y, float scale, char *text, vec4_t color, qboolean forceColor, qboolean shadow, fontInfo_t *font) {
	char *finalText = text;

	if (forceColor) {
		finalText = _strdup(text);
		Q_CleanStr(finalText);
	}

	CG_Text_Paint_Ext(x, y, scale, scale, color, finalText, 0.0f, 0, shadow ? ITEM_TEXTSTYLE_SHADOWED : 0, font);

	if (forceColor)
		free(finalText);
}

int CNexDraw::sizeTextHeight(float scale, fontInfo_t *font) {
	return (font->glyphs['['].height + 6) * font->glyphScale * scale;
}

int CNexDraw::sizeTextWidth(float scale, char *text, fontInfo_t *font) {
	return CG_Text_Width_Ext(text, scale, 0, font) + 1;
}

void CNexDraw::drawTextRect(float x, float y, float scale, char *text, vec4_t textColor, qboolean forceColor, qboolean textShadow, qboolean background, qboolean border, vec4_t backgroundColor, vec4_t borderColor, fontInfo_t *font) {
	int width = sizeTextWidth(scale, text, font);
	int height = sizeTextHeight(scale, font);
	float borderSize = font->glyphs['['].pitch * scale * 3;
	if (background)
		drawFillRect(x - borderSize, y - borderSize, width + (2 * borderSize), height + (2 * borderSize), backgroundColor);
	if (border)
		drawRect(x - borderSize, y - borderSize, width + (2 * borderSize), height + (2 * borderSize), borderColor, 1);

	drawText(x, y, scale, text, textColor, forceColor, textShadow, font);
}
