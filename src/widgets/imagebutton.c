#include "widgetz/widgetz_internal.h"

/*
Title: Image Button

Section: Internal

Function: wz_button_proc

See also:
<wz_widget_proc>
*/
int wz_image_button_proc(WZ_WIDGET* wgt, ALLEGRO_EVENT* event)
{
	int ret = 1;
	WZ_IMAGE_BUTTON* but = (WZ_IMAGE_BUTTON*)wgt;
	switch (event->type)
	{
		case WZ_DRAW:
		{
			ALLEGRO_BITMAP* image = 0;
			if (wgt->flags & WZ_STATE_HIDDEN)
				break;
			else if (wgt->flags & WZ_STATE_DISABLED)
				image = but->disabled;
			else
			{
				WZ_BUTTON* button = (WZ_BUTTON*)but;
				if (button->down)
					image = but->down;
				else if (wgt->flags & WZ_STATE_HAS_FOCUS)
					image = but->focused;
				else
					image = but->normal;
			}
			wgt->theme->draw_image(wgt->theme, wgt->local_x, wgt->local_y, wgt->w, wgt->h, image);
			break;
		}
		default:
			ret = 0;
	}
	if (ret == 0)
		ret = wz_button_proc(wgt, event);
	return ret;
}

/*
Function: wz_init_image_button
*/
void wz_init_image_button(WZ_IMAGE_BUTTON* but, WZ_WIDGET* parent, float x, float y, float w, float h, ALLEGRO_BITMAP* normal, ALLEGRO_BITMAP* down, ALLEGRO_BITMAP* focused, ALLEGRO_BITMAP* disabled, int id)
{
	WZ_WIDGET* wgt = (WZ_WIDGET*)but;
	wz_init_button((WZ_BUTTON*)wgt, parent, x, y, w, h, 0, 0, id);
	but->normal = normal;
	but->down = down;
	but->focused = focused;
	but->disabled = disabled;
	wgt->proc = wz_image_button_proc;
}

/*
Section: Public

Function: wz_create_image_button

Creates an image button.

Parameters:

normal - Image drawn when the button is in its default state
down - Image drawn when the button is pressed down
focused - Image drawn when the button is focused
disabled - Image drawn when the button is disabled

Inherits From:
<WZ_BUTTON>

See Also:
<WZ_IMAGE_BUTTON>

<wz_create_button>
*/
WZ_IMAGE_BUTTON* wz_create_image_button(WZ_WIDGET* parent, float x, float y, float w, float h, ALLEGRO_BITMAP* normal, ALLEGRO_BITMAP* down, ALLEGRO_BITMAP* focused, ALLEGRO_BITMAP* disabled, int id)
{
	WZ_IMAGE_BUTTON* but = malloc(sizeof(WZ_IMAGE_BUTTON));
	wz_init_image_button(but, parent, x, y, w, h, normal, down, focused, disabled, id);
	return but;
}
