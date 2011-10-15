#include "../widgetz_internal.h"

/*
Title: Box

Section: Internal

Function: wz_box_proc

See also:
<wz_widget_proc>
*/
int wz_box_proc(WZ_WIDGET* wgt, ALLEGRO_EVENT* event)
{
	int ret = 1;
	switch (event->type)
	{
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		{
			if (event->mouse.button == 1 && wz_widget_rect_test(wgt, event->mouse.x, event->mouse.y))
			{
				wz_ask_parent_for_focus(wgt);
			}
			ret = 0;
			break;
		}
		case WZ_DRAW:
		{
			if (wgt->flags & WZ_STATE_HIDDEN)
			{
				ret = 0;
			}
			else
			{
				int flags = (wgt->flags & WZ_STATE_HAS_FOCUS) ? WZ_STYLE_FOCUSED : 0;
				wgt->theme->draw_box(wgt->theme, wgt->local_x, wgt->local_y, wgt->w, wgt->h, flags);
			}
		}
		default:
			ret = 0;
	}
	if (ret == 0)
		ret = wz_widget_proc(wgt, event);
	return ret;
}

/*
Function: wz_init_box
*/
void wz_init_box(WZ_WIDGET* wgt, WZ_WIDGET* parent, float x, float y, float w, float h, int id)
{
	wz_init_widget(wgt, parent, x, y, w, h, id);
	wgt->proc = wz_box_proc;
}

/*
Section: Public

Function: wz_create_box

Creates a box.

See Also:
<WZ_WIDGET>

<wz_create_widget>
*/
WZ_WIDGET* wz_create_box(WZ_WIDGET* parent, float x, float y, float w, float h, int id)
{
	WZ_WIDGET* wgt = malloc(sizeof(WZ_WIDGET));
	wz_init_box(wgt, parent, x, y, w, h, id);
	return wgt;
}
