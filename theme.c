#include "widgetz_internal.h"
#include "math.h"

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

/*
Title: Theme Stuff
*/

//My version of it
//returns the token length, and skips the first occurence if it is the first character
//char *wz_ustrpbrk(AL_CONST char *s, AL_CONST char *set, int* index)
//{
//	AL_CONST char *setp;
//	int c, d;
//	int first = 1;
//	ALLEGRO_ASSERT(s);
//	ALLEGRO_ASSERT(set);
//	
//	*index = 0;
//	while ((c = ugetc(s)) != 0)
//	{
//		setp = set;
//		
//		while ((d = ugetxc(&setp)) != 0)
//		{
//			if (c == d && !first)
//				return (char *)s;
//		}
//		(*index)++;
//		s += uwidth(s);
//		first = 0;
//	}
//	
//	return NULL;
//}

//return the new start
int wz_find_eol(ALLEGRO_USTR* text, ALLEGRO_FONT* font, float max_width, int start, int* end)
{
	int a, b;
	int first = 1;
	int last = 0;
	a = start;
	
	while(1)
	{
		/*
		Find the end of current token
		*/
		b = al_ustr_find_set_cstr(text, a, "\t\n ");
		if(b == -1) //found nothing
		{
			b = al_ustr_size(text); //this is the last whole word
			last = 1;
		}
		
		/*
		Check to see if the token fits
		*/
		ALLEGRO_USTR_INFO info;
		ALLEGRO_USTR* token = al_ref_ustr(&info, text, start, b);

		float len = al_get_ustr_width(font, token);
		if (len < max_width || first)
		{
			if(last)
			{
				*end = b + 1;
				return -1;
			}
		}
		else   //we return the last num
		{
			*end = a - 1;
			return a;
		}
		
		/*
		Check what character we found
		*/
		int character = al_ustr_get(text, b);
		if(character == '\n')
		{
			*end = b;
			return b + 1;
		}
		
		a = b + 1;
		first = 0;
	}
}

/*
Function: wz_draw_single_text

Draws a single line of text
*/
void wz_draw_single_text(float x, float y, float w, float h, int halign, int valign, ALLEGRO_COLOR color, ALLEGRO_FONT* font, ALLEGRO_USTR* text)
{
	float xpos;
	float ypos;
	float height = al_get_font_line_height(font);
		
	if (valign == WZ_ALIGN_TOP)
	{
		ypos = y;
	}
	else if (valign == WZ_ALIGN_BOTTOM)
	{
		ypos = y + h - height;
	}
	else
	{
		ypos = y + h / 2 - height / 2;
	}
	
	if (halign == WZ_ALIGN_LEFT)
	{
		xpos = x;
		al_draw_ustr(font, color, floorf(xpos), floorf(ypos), ALLEGRO_ALIGN_LEFT, text);
	}
	else if (halign == WZ_ALIGN_RIGHT)
	{
		xpos = x + w;
		al_draw_ustr(font, color, floorf(xpos), floorf(ypos), ALLEGRO_ALIGN_RIGHT, text);
	}
	else
	{
		xpos = x + w / 2;
		al_draw_ustr(font, color, floorf(xpos), floorf(ypos), ALLEGRO_ALIGN_CENTRE, text);
	}
}

/*
Function: wz_draw_multi_text

Draws multiple lines of text, wrapping it as necessary
*/
void wz_draw_multi_text(float x, float y, float w, float h, int halign, int valign, ALLEGRO_COLOR color, ALLEGRO_FONT* font, ALLEGRO_USTR* text)
{
	float cur_y = y;
	float text_height = al_get_font_line_height(font);
		
	float total_height = 0;
	if (valign == WZ_ALIGN_BOTTOM || valign == WZ_ALIGN_CENTRE)
	{
		int ret = 0;
		do
		{
			int start = ret;
			int end;
			ret = wz_find_eol(text, font, w, start, &end);
			total_height += text_height;
		}
		while (ret > 0);
	}
	
	if (valign == WZ_ALIGN_BOTTOM)
	{
		cur_y = y + h - total_height;
	}
	else if (valign == WZ_ALIGN_CENTRE)
	{
		cur_y = y + (h - total_height) / 2;
	}

	int ret = 0;
	do
	{
		int start = ret;
		int end;
		ret = wz_find_eol(text, font, w, start, &end);
		
		ALLEGRO_USTR_INFO info;
		ALLEGRO_USTR* token = al_ref_ustr(&info, text, start, end);
		
		wz_draw_single_text(x, cur_y, w, h, halign, WZ_ALIGN_TOP, color, font, token);
		cur_y += text_height;
	}
	while (ret > 0);
}

void wz_def_draw_box(struct WZ_THEME* theme, float x, float y, float w, float h, int style)
{
	WZ_DEF_THEME* thm = (WZ_DEF_THEME*)theme;
	al_draw_filled_rectangle(x, y, x + w, y + h, wz_scale_color(thm->color1, 0.5));
	if (style & WZ_STYLE_FOCUSED)
		al_draw_rectangle(x, y, x + w, y + h, wz_scale_color(thm->color1, 1.5), 1);
	else
		al_draw_rectangle(x, y, x + w, y + h, thm->color1, 1);
}

void wz_def_draw_button(WZ_THEME* theme, float x, float y, float w, float h, ALLEGRO_USTR* text, int style)
{
	WZ_DEF_THEME* thm = (WZ_DEF_THEME*)theme;
	
	ALLEGRO_COLOR border_col;
	ALLEGRO_COLOR text_col;
	
	if (style & WZ_STYLE_FOCUSED)
	{
		border_col = wz_scale_color(thm->color1, 1.5);
	}
	else if (style & WZ_STYLE_DISABLED)
	{
		border_col = wz_scale_color(thm->color1, 0.5);
	}
	else if (style & WZ_STYLE_DOWN)
	{
		border_col = thm->color2;
	}
	else
	{
		border_col = thm->color1;
	}
	if (style & WZ_STYLE_DISABLED)
		text_col = wz_scale_color(thm->color2, 0.5);
	else
		text_col = thm->color2;
		
	al_draw_filled_rectangle(x, y, x + w, y + h, wz_scale_color(thm->color1, 0.75));
	al_draw_rectangle(x, y, x + w, y + h, border_col, 1);
	wz_draw_multi_text(x, y, w, h, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, text_col, thm->font, text);
}

void wz_def_draw_textbox(struct WZ_THEME* theme, float x, float y, float w, float h, int halign, int valign, ALLEGRO_USTR* text, int style)
{
	WZ_DEF_THEME* thm = (WZ_DEF_THEME*)theme;
	ALLEGRO_COLOR text_col;
	if (style & WZ_STYLE_DISABLED)
		text_col = wz_scale_color(thm->color2, 0.5);
	else
		text_col = thm->color2;
	wz_draw_multi_text(x, y, w, h, halign, valign,text_col, thm->font, text);
}

void wz_def_draw_scroll(struct WZ_THEME* theme, float x, float y, float w, float h, float fraction, int style)
{
	WZ_DEF_THEME* thm = (WZ_DEF_THEME*)theme;
	int vertical = h > w;
	ALLEGRO_COLOR col;
	float xpos;
	float ypos;
	
	if (style & WZ_STYLE_FOCUSED)
	{
		col = wz_scale_color(thm->color2, 1.5);
	}
	else if (style & WZ_STYLE_DISABLED)
	{
		col = wz_scale_color(thm->color2, 0.5);
	}
	else
	{
		col = thm->color2;
	}
	
	if (vertical)
	{
		xpos = x + w / 2;
		ypos = y + fraction * h;
		
		al_draw_line(xpos, y, xpos, y + h, col, 1);
	}
	else
	{
		xpos = x + fraction * w;
		ypos = y + h / 2;
		
		al_draw_line(x, ypos, x + w, ypos, col, 1);
	}
	al_draw_rectangle(xpos - 4, ypos - 4, xpos + 4, ypos + 4, col, 1);
}

void wz_def_draw_editbox(struct WZ_THEME* theme, float x, float y, float w, float h, int cursor_pos, ALLEGRO_USTR* text, int style)
{
	WZ_DEF_THEME* thm = (WZ_DEF_THEME*)theme;
	al_draw_filled_rectangle(x, y, x + w, y + h, wz_scale_color(thm->color1, 0.75));
	int len = wz_get_text_pos(thm->font, text, w);
	int cx,cy,cw,ch;
	int len2 = al_ustr_length(text);
	len = len + 1 > len2 ? len2 : len + 1;
	
	int offset = al_ustr_offset(text, len);
	
	ALLEGRO_USTR_INFO info;
	ALLEGRO_USTR* token = al_ref_ustr(&info, text, 0, offset);
	
	ALLEGRO_COLOR border_col;
	ALLEGRO_COLOR text_col;
	
	if (style & WZ_STYLE_FOCUSED)
	{
		border_col = wz_scale_color(thm->color1, 1.5);
	}
	else if (style & WZ_STYLE_DISABLED)
	{
		border_col = wz_scale_color(thm->color1, 0.5);
	}
	else if (style & WZ_STYLE_DOWN)
	{
		border_col = thm->color2;
	}
	else
	{
		border_col = thm->color1;
	}
	
	if (style & WZ_STYLE_DISABLED)
		text_col = wz_scale_color(thm->color2, 0.5);
	else
		text_col = thm->color2;
	
	al_get_clipping_rectangle(&cx, &cy, &cw, &ch);
	al_set_clipping_rectangle(x, y, w, h);
	wz_draw_single_text(x, y, w, h, WZ_ALIGN_LEFT, WZ_ALIGN_CENTRE, text_col, thm->font, token);
	al_set_clipping_rectangle(cx, cy, cw, ch);
	
	al_draw_rectangle(x, y, x + w, y + h, border_col, 1);
	
	if (style & WZ_STYLE_FOCUSED)
	{
		if (((int)(al_get_time() / 0.5f)) % 2 == 0)
		{
			offset = al_ustr_offset(text, cursor_pos);
			token = al_ref_ustr(&info, text, 0, offset);
			float len = al_get_ustr_width(thm->font, token);
			float halfheight = al_get_font_line_height(thm->font) / 2.0f;
			al_draw_line(x + len, y + h / 2 - halfheight, x + len, y + h / 2 + halfheight, border_col, 1);
		}
	}
}

void wz_def_draw_image(struct WZ_THEME* theme, float x, float y, float w, float h, ALLEGRO_BITMAP* image)
{
	float ix = x + (w - al_get_bitmap_width(image)) / 2;
	float iy = y + (h - al_get_bitmap_height(image)) / 2;
	
	al_draw_bitmap(image, ix, iy, 0);
}

ALLEGRO_FONT* wz_def_get_font(struct WZ_THEME* theme, int font_num)
{
	WZ_DEF_THEME* thm = (WZ_DEF_THEME*)theme;
	return thm->font;
}

/*
Variable: wz_def_theme

The default theme.
*/
const WZ_DEF_THEME wz_def_theme =
{
	{
		wz_def_draw_button,
		wz_def_draw_box,
		wz_def_draw_textbox,
		wz_def_draw_scroll,
		wz_def_draw_editbox,
		wz_def_draw_image,
		wz_def_get_font,
	}
};
