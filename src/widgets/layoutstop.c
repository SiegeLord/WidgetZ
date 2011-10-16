#include "widgetz/widgetz_internal.h"

/*
Title: Layout Stop
*/

/*
Function: wz_init_layout_stop
*/
void wz_init_layout_stop(WZ_WIDGET* box, WZ_WIDGET* parent, int id)
{
	WZ_WIDGET* wgt = (WZ_WIDGET*)box;
	wz_init_widget(wgt, parent, 0, 0, 0, 0, id);
	wgt->proc = wz_widget_proc;
	wgt->flags |= WZ_STATE_LAYOUT;
	wgt->flags |= WZ_STATE_NOTWANT_FOCUS;
}

/*
Section: Public

Function: wz_create_layout_stop

Creates a layout stop meta widget. Any widgets after this one will no longer be subject
to any layout control

Inherits From:
<WZ_WIDGET>

See Also:
<WZ_FILL_LAYOUT>

<wz_create_widget>
*/
WZ_WIDGET* wz_create_layout_stop(WZ_WIDGET* parent, int id)
{
	WZ_WIDGET* box = malloc(sizeof(WZ_WIDGET));
	wz_init_layout_stop(box, parent, id);
	wz_ask_parent_to_focus_next(box);
	return box;
}

