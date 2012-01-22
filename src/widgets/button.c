/*
Copyright (c) 2011 Pavel Sountsov

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "widgetz/widgetz_internal.h"

/*
Title: Button

Section: Internal

Function: wz_button_proc

See also:
<wz_widget_proc>
*/
int wz_button_proc(WZ_WIDGET* wgt, ALLEGRO_EVENT* event)
{
	int ret = 1;
	WZ_BUTTON* but = (WZ_BUTTON*)wgt;
	switch (event->type)
	{
		case WZ_LOSE_FOCUS:
		{
			but->down = 0;
			ret = 0;
			break;
		}
		case WZ_DRAW:
		{
			if (wgt->flags & WZ_STATE_HIDDEN)
			{
				ret = 0;
			}
			else if (wgt->flags & WZ_STATE_DISABLED)
			{
				wgt->theme->draw_button(wgt->theme, wgt->local_x, wgt->local_y, wgt->w, wgt->h, but->text, WZ_STYLE_DISABLED);
			}
			else
			{
				int flags = 0;
				if (but->down)
					flags |= WZ_STYLE_DOWN;
				if (wgt->flags & WZ_STATE_HAS_FOCUS)
					flags |= WZ_STYLE_FOCUSED;
				
				wgt->theme->draw_button(wgt->theme, wgt->local_x, wgt->local_y, wgt->w, wgt->h, but->text, flags);
			}
			break;
		}
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		{
			if (wgt->flags & WZ_STATE_DISABLED)
			{
				ret = 0;
			}
			else if (event->mouse.button == 1 && wz_widget_rect_test(wgt, event->mouse.x, event->mouse.y))
			{
				wz_ask_parent_for_focus(wgt);
				but->down = 1;
			}
			else
				ret = 0;
			break;
		}
		case ALLEGRO_EVENT_KEY_DOWN:
		{
			switch (event->keyboard.keycode)
			{
				case ALLEGRO_KEY_ENTER:
				{
					if (wgt->flags & WZ_STATE_DISABLED)
					{
						ret = 0;
					}
					else if (wgt->flags & WZ_STATE_HAS_FOCUS)
					{
						but->down = 1;
					}
					else
						ret = 0;
					break;
				}
				default:
					ret = 0;
			}
			break;
		}
		case ALLEGRO_EVENT_KEY_UP:
		{
			switch (event->keyboard.keycode)
			{
			case ALLEGRO_KEY_ENTER:
				{
					if (wgt->flags & WZ_STATE_DISABLED)
					{
						ret = 0;
					}
					else if (wgt->flags & WZ_STATE_HAS_FOCUS)
					{
						wz_trigger(wgt);
					}
					else
						ret = 0;
					break;
				}
			default:
				ret = 0;
			}
			break;
		}
		case WZ_HANDLE_SHORTCUT:
		{
			if (wgt->flags & WZ_STATE_DISABLED)
			{
				ret = 0;
			}
			else
			{
				if (!(wgt->flags & WZ_STATE_HAS_FOCUS))
				{
					wz_ask_parent_for_focus(wgt);
				}
				wz_trigger(wgt);
			}
			break;
		}
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
			if (wgt->flags & WZ_STATE_DISABLED)
			{
				ret = 0;
			}
			else if (event->mouse.button == 1 && wz_widget_rect_test(wgt, event->mouse.x, event->mouse.y))
			{
				wz_trigger(wgt);
			}
			else
				ret = 0;
			break;
		}
		case ALLEGRO_EVENT_MOUSE_AXES:
		{
			if (wgt->flags & WZ_STATE_DISABLED)
			{
				ret = 0;
			}
			else if ((event->mouse.dx != 0 || event->mouse.dy != 0) && wz_widget_rect_test(wgt, event->mouse.x, event->mouse.y))
			{
				ALLEGRO_MOUSE_STATE state;
				al_get_mouse_state(&state);
				if (state.buttons & 1)
				{
					but->down = 1;
				}
				else
					but->down = 0;
				wz_ask_parent_for_focus(wgt);
			}
			else
			{
				but->down = 0;
				ret = 0;
			}
			break;
		}
		case WZ_DESTROY:
		{
			if(but->own)
				al_ustr_free(but->text);
			ret = 0;
			break;
		}
		case WZ_SET_TEXT:
		{
			if(but->own)
			{
				al_ustr_free(but->text);
				but->text = al_ustr_dup((ALLEGRO_USTR*)event->user.data3);
			}
			else
			{
				but->text = (ALLEGRO_USTR*)event->user.data3;
			}
			break;
		}
		case WZ_TRIGGER:
		{
			but->down = 0;
			{
			ALLEGRO_EVENT ev;
			wz_craft_event(&ev, WZ_BUTTON_PRESSED, wgt, 0);
			al_emit_user_event(wgt->source,	&ev, 0);
			}
			break;
		}
		default:
			ret = 0;
	}
	if (ret == 0)
		ret = wz_widget_proc(wgt, event);
	return ret;
}

/*
Function: wz_init_button
*/
void wz_init_button(WZ_BUTTON* but, WZ_WIDGET* parent, float x, float y, float w, float h, ALLEGRO_USTR* text, int own, int id)
{
	WZ_WIDGET* wgt = (WZ_WIDGET*)but;
	wz_init_widget(wgt, parent, x, y, w, h, id);
	but->down = 0;
	but->own = own;
	but->text = text;
	wgt->proc = wz_button_proc;
}

/*
Section: Public

Function: wz_create_button

Creates a button.

Parameters:

own - Set to 1 if you want the widget to own the text

Inherits From:
<WZ_WIDGET>

See Also:
<WZ_BUTTON>

<wz_create_widget>
*/
WZ_BUTTON* wz_create_button(WZ_WIDGET* parent, float x, float y, float w, float h, ALLEGRO_USTR* text, int own, int id)
{
	WZ_BUTTON* but = malloc(sizeof(WZ_BUTTON));
	wz_init_button(but, parent, x, y, w, h, text, own, id);
	return but;
}
