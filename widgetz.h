#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

/*
Title: Misc
*/

/*
Struct: WZ_THEME

A struct that defines a bunch of rendering functions to render various types of widgets
*/
typedef struct WZ_THEME
{
	void (*draw_button)(struct WZ_THEME* theme, float x, float y, float w, float h, ALLEGRO_USTR* text, int style);
	void (*draw_box)(struct WZ_THEME* theme, float x, float y, float w, float h, int style);
	void (*draw_textbox)(struct WZ_THEME* theme, float x, float y, float w, float h, int halign, int valign, ALLEGRO_USTR* text, int style);
	void (*draw_scroll)(struct WZ_THEME* theme, float x, float y, float w, float h, float fraction, int style);
	void (*draw_editbox)(struct WZ_THEME* theme, float x, float y, float w, float h, int cursor_pos, ALLEGRO_USTR* text, int style);
	void (*draw_image)(struct WZ_THEME* theme, float x, float y, float w, float h, ALLEGRO_BITMAP* image);
	ALLEGRO_FONT*(*get_font)(struct WZ_THEME* theme, int font_num);
} WZ_THEME;

/*
Struct: WZ_DEF_THEME

The default theme struct


Inherits From:
<WZ_THEME>
*/
typedef struct WZ_DEF_THEME
{
	WZ_THEME theme;
	
	/*
	Variable: font
	Font to use for this GUI
	*/
	ALLEGRO_FONT* font;
	
	/*
	Variable: color1
	The background color
	*/
	ALLEGRO_COLOR color1;
	/*
	Variable: color2
	The foreground color
	*/
	ALLEGRO_COLOR color2;
} WZ_DEF_THEME;

typedef struct WZ_SHORTCUT
{
	int modifiers;
	int keycode;
} WZ_SHORTCUT;

/*
Struct: WZ_WIDGET

A base widget struct.

See Also:
<wz_create_widget>
*/
typedef struct WZ_WIDGET
{
	/*
	Variable: x
	x coordinate of the widget
	*/
	float x;
	/*
	Variable: y
	y coordinate of the widget
	*/
	float y;
	
	float local_x;
	float local_y;
	
	/*
	Variable: w
	Width of the widget
	*/
	float w;
	/*
	Variable: h
	Height of the widget
	*/
	float h;
	
	struct WZ_WIDGET* prev_sib;
	struct WZ_WIDGET* next_sib;
	struct WZ_WIDGET* parent;
	
	struct WZ_WIDGET* last_child;
	struct WZ_WIDGET* first_child;
	
	WZ_THEME* theme;
	ALLEGRO_EVENT_SOURCE* source;
	
	WZ_SHORTCUT shortcut;
	
	/*
	Variable: flags
	Flags of the widget. A combination of some sort of <Widget States>
	*/
	int flags;
	/*
	Variable: id
	id of the widget
	*/
	int id;
	
	int (*proc)(struct WZ_WIDGET*, ALLEGRO_EVENT*);
} WZ_WIDGET;

/*
Struct: WZ_BUTTON

A button. Sends <WZ_BUTTON_PRESSED> when pressed.

Inherits From:
<WZ_WIDGET>

See Also:
<wz_create_button>
*/
typedef struct WZ_BUTTON
{
	WZ_WIDGET wgt;
	ALLEGRO_USTR* text;
	int own;
	int down;
} WZ_BUTTON;

/*
Struct: WZ_IMAGE_BUTTON

An image button. Draws itself using four images, one for each state. The text is
not drawn.

Inherits From:
<WZ_BUTTON>

See Also:
<wz_create_image_button>
*/
typedef struct WZ_IMAGE_BUTTON
{
	WZ_BUTTON button;
	ALLEGRO_BITMAP* normal;
	ALLEGRO_BITMAP* down;
	ALLEGRO_BITMAP* focused;
	ALLEGRO_BITMAP* disabled;
} WZ_IMAGE_BUTTON;

/*
Struct: WZ_FILL_LAYOUT

A meta-widget that controls the layout of its sibling widgets. The algorithm is simple, it takes all widgets
that go after it (stopping as soon as it hits another layout widget), and arranges them in order, in a grid,
respecting the horizontal and vertical spacings. It wraps the widgets to try to fit in this layout widget's
width.

Inherits From:
<WZ_WIDGET>

See Also:
<wz_create_fill_layout>
*/
typedef struct WZ_FILL_LAYOUT
{
	WZ_WIDGET wgt;
	float h_spacing;
	float v_spacing;
	int h_align;
	int v_align;
} WZ_FILL_LAYOUT;

/*
Struct: WZ_TEXTBOX

A texbox that will wrap the text inside if needed.

Inherits From:
<WZ_WIDGET>

See Also:
<wz_create_textbox>
*/
typedef struct WZ_TEXTBOX
{
	WZ_WIDGET wgt;
	ALLEGRO_USTR* text;
	int own;
	int h_align;
	int v_align;
} WZ_TEXTBOX;

/*
Struct: WZ_SCROLL

A scroll bar. If it's higher than it is wide, it is a vertical scroll bar. Otherwise it is a horizontal scroll bar.
Sends <WZ_SCROLL_POS_CHANGED> when it's slider is moved.

Inherits From:
<WZ_WIDGET>

See Also:
<wz_create_scroll>
*/
typedef struct WZ_SCROLL
{
	WZ_WIDGET wgt;
	int max_pos;
	int cur_pos;
} WZ_SCROLL;

/*
Struct: WZ_TOGGLE

A toggle button. Creates a toggle button. A toggle button stays pressed when you press it, requiring you to click a second time
to depress it. If group does not equal to -1, then all toggle button siblings sharing the same group number will
force only one button in this group to be pressed. Sends the same events as <WZ_BUTTON>.

Inherits From:
<WZ_BUTTON>

See Also:
<wz_create_toggle_button>
*/
typedef struct WZ_TOGGLE
{
	WZ_BUTTON but;
	int group;
} WZ_TOGGLE;

/*
Struct: WZ_EDITBOX

An single line edit box. You can type text in it, until the buffer inside of it is filled.
Sends the <WZ_TEXT_CHANGED> event.

Inherits From:
<WZ_WIDGET>

See Also:
<wz_create_editbox>
*/
typedef struct WZ_EDITBOX
{
	WZ_WIDGET wgt;
	/*
	Variable: text
	Holds the current text that is entered inside the textbox.
	*/
	ALLEGRO_USTR* text;
	int own;
	int cursor_pos;
	int scroll_pos;
} WZ_EDITBOX;

/*
Section: Constants

Various constants defined by WidgetZ

Constants: Widget States

WZ_STATE_HIDDEN           - Widget is hidden
WZ_STATE_DISABLED         - Widget is disabled
WZ_STATE_HAS_FOCUS        - Widget has focus
WZ_STATE_NOTWANT_FOCUS    - Widget does not want focus
WZ_STATE_LAYOUT           - Widget is a layout widget
*/
#define WZ_STATE_HIDDEN             02
#define WZ_STATE_DISABLED           04
#define WZ_STATE_HAS_FOCUS         010
#define WZ_STATE_NOTWANT_FOCUS     020
#define WZ_STATE_LAYOUT            040

/*
Constants: Widget Styles

WZ_STYLE_DISABLED          - Widget is disabled
WZ_STYLE_FOCUSED           - Widget has focus
WZ_STYLE_DOWN              - Widget is down
*/
#define WZ_STYLE_DISABLED           02
#define WZ_STYLE_FOCUSED            04
#define WZ_STYLE_DOWN              020

/*
Enum: Events

For events, data1 is the issuer widget id and data2 points to the
actual widget, data3 varies per event

WZ_DRAW - Draw the widget
WZ_DESTROY - Destroy widget
WZ_UPDATE - Update widget. data3 is the time delta
WZ_UPDATE_POSITION - Tell widget to update its local position
WZ_HIDE - Hide the widget
WZ_SHOW - Show the widget
WZ_DISABLE - Disable the widget
WZ_ENABLE - Enabled the widget
WZ_TRIGGER - Trigger the widget
WZ_BUTTON_PRESSED - Indicates that a button was pressed
WZ_TEXT_CHANGED - Indicates that text was changed
WZ_SET_TEXT - Tells the widget to set its text. data3 is the new text.
WZ_WANT_FOCUS - Tells the widget that a widget wants focus
WZ_TAKE_FOCUS - Tells the widget that it has obtained focus
WZ_LOSE_FOCUS - Tells the widget that it has lost focus
WZ_HANDLE_SHORTCUT - Called by self when own shortcut is pressed
WZ_SET_SCROLL_POS - Tells the widget to move its scroll position somewhere. data3 is the new scroll position.
WZ_SET_SCROLL_MAX_POS - Tells the widget to alter its max position. data3 is the new max scroll position.
WZ_SCROLL_POS_CHANGED - Indicates that the scroll position changed. data3 is the new scroll position.
WZ_TOGGLE_GROUP - Tells something to a toggle group. data3 is the group number
WZ_SET_CURSOR_POS - Sets the cursor position. data3 is the new cursor position
WZ_EVENT_COUNT - A dummy event, useful if you need to add more events without conflicts
*/
enum
{
	WZ_DRAW = 9000,
	WZ_DESTROY,
	WZ_UPDATE,//3rd pos is time delta
	WZ_UPDATE_POSITION,
	WZ_HIDE,
	WZ_SHOW,
	WZ_DISABLE,
	WZ_ENABLE,
	WZ_TRIGGER,
	WZ_BUTTON_PRESSED,
	WZ_TEXT_CHANGED,
	WZ_SET_TEXT,//3rd pos is new text
	WZ_WANT_FOCUS,
	WZ_TAKE_FOCUS,
	WZ_LOSE_FOCUS,
	WZ_HANDLE_SHORTCUT,//called by self when own shortcut is pressed
	WZ_SET_SCROLL_POS,//3rd pos is the new position for the scroll
	WZ_SET_SCROLL_MAX_POS,//3rd pos is the new maximum position for the scroll
	WZ_SCROLL_POS_CHANGED,//3rd pos is the new scroll position
	WZ_TOGGLE_GROUP,//3rd pos is the group number
	WZ_SET_CURSOR_POS,//sets the new cursor position
	WZ_EVENT_COUNT
};

/*
Enum: Alignments

WZ_ALIGN_CENTRE - Centre alignment
WZ_ALIGN_LEFT - Left alignment
WZ_ALIGN_RIGHT - Right alignment
WZ_ALIGN_TOP - Top alignment
WZ_ALIGN_BOTTOM - Buttom alignment
*/
enum
{
	WZ_ALIGN_CENTRE = 0,
	WZ_ALIGN_LEFT,
	WZ_ALIGN_RIGHT,
	WZ_ALIGN_TOP,
	WZ_ALIGN_BOTTOM
};

extern const WZ_DEF_THEME wz_def_theme;

void wz_register_sources(WZ_WIDGET* wgt, ALLEGRO_EVENT_QUEUE* queue);
void wz_set_theme(WZ_WIDGET* wgt, WZ_THEME* theme);
void wz_detach(WZ_WIDGET* wgt);
void wz_attach(WZ_WIDGET* wgt, WZ_WIDGET* parent);
void wz_attach_after(WZ_WIDGET* wgt, WZ_WIDGET* sib);
void wz_attach_before(WZ_WIDGET* wgt, WZ_WIDGET* sib);

int wz_send_event(WZ_WIDGET* wgt, ALLEGRO_EVENT* event);
int wz_broadcast_event(WZ_WIDGET* wgt, ALLEGRO_EVENT* event);

void wz_update(WZ_WIDGET* wgt, double dt);
void wz_draw(WZ_WIDGET* wgt);
void wz_destroy(WZ_WIDGET* wgt);
void wz_trigger(WZ_WIDGET* wgt);

void wz_set_text(WZ_WIDGET* wgt, ALLEGRO_USTR* text);
void wz_set_scroll_pos(WZ_WIDGET* wgt, int pos, int max);
void wz_set_cursor_pos(WZ_WIDGET* wgt, int pos);
void wz_focus(WZ_WIDGET* wgt, int focus);
void wz_show(WZ_WIDGET* wgt, int show);
void wz_enable(WZ_WIDGET* wgt, int enable);
void wz_set_shortcut(WZ_WIDGET* wgt, int keycode, int modifiers);

ALLEGRO_COLOR wz_blend_colors(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, float frac);
ALLEGRO_COLOR wz_scale_color(ALLEGRO_COLOR c, float scale);

WZ_WIDGET* wz_create_widget(WZ_WIDGET* parent, float x, float y, int id);
WZ_BUTTON* wz_create_button(WZ_WIDGET* parent, float x, float y, float w, float h, ALLEGRO_USTR* text, int own, int id);
WZ_TEXTBOX* wz_create_textbox(WZ_WIDGET* parent, float x, float y, float w, float h, int halign, int valign, ALLEGRO_USTR* text, int own, int id);
WZ_TOGGLE* wz_create_toggle_button(WZ_WIDGET* parent, float x, float y, float w, float h, ALLEGRO_USTR* text, int own, int group, int id);
WZ_WIDGET* wz_create_box(WZ_WIDGET* parent, float x, float y, float w, float h, int id);
WZ_FILL_LAYOUT* wz_create_fill_layout(WZ_WIDGET* parent, float x, float y, float w, float h, float hspace, float vspace, int halign, int valign, int id);
WZ_SCROLL* wz_create_scroll(WZ_WIDGET* parent, float x, float y, float w, float h, int max_pos, int id);
WZ_EDITBOX* wz_create_editbox(WZ_WIDGET* parent, float x, float y, float w, float h, ALLEGRO_USTR* text, int own, int id);
WZ_WIDGET* wz_create_layout_stop(WZ_WIDGET* parent, int id);
WZ_IMAGE_BUTTON* wz_create_image_button(WZ_WIDGET* parent, float x, float y, float w, float h, ALLEGRO_BITMAP* normal, ALLEGRO_BITMAP* down, ALLEGRO_BITMAP* focused, ALLEGRO_BITMAP* disabled, int id);

void wz_draw_multiline_text(float x, float y, float w, float h, int halign, int valign, ALLEGRO_COLOR color, ALLEGRO_FONT* font, ALLEGRO_USTR* text);
void wz_draw_single_text(float x, float y, float w, float h, int halign, int valign, ALLEGRO_COLOR color, ALLEGRO_FONT* font, ALLEGRO_USTR* text);

int wz_widget_rect_test(WZ_WIDGET* wgt, float x, float y);
int wz_widget_rect_test_all(WZ_WIDGET* wgt, float x, float y);
