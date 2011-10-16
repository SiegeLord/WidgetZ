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

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <math.h>
#include "widgetz/widgetz.h"

int main(int argc, char *argv[])
{
	// Initialize Allegro 5 and the font routines
	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_DISPLAY *display = al_create_display(1000, 600);
	al_install_keyboard();
	// Load a font
	ALLEGRO_FONT *font = al_load_font("DejaVuSans.ttf", 18, 0);
	
	al_install_mouse();
	
	// Start the event queue to handle keyboard input and our timer
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	
	int refresh_rate = 60;
	double fixed_dt = 1.0f / refresh_rate;
	double old_time = al_current_time();
	double game_time = al_current_time();
	
	/*
	Define custom theme
	wz_def_theme is a global vtable defined by the header
	*/
	WZ_DEF_THEME theme = wz_def_theme;
	theme.font = font;
	theme.color1 = al_map_rgba_f(0, 0.3, 0, 1);
	theme.color2 = al_map_rgba_f(1, 1, 0, 1);
	
	/*
	Define root gui element
	*/
	WZ_WIDGET* gui = wz_create_widget(0, 0, 0, -1);
	wz_set_theme(gui, (WZ_THEME*)&theme);
	
	/*
	Define all other gui elements, fill_layout is an automatic layout container
	*/
	WZ_WIDGET* wgt;
	wz_create_fill_layout(gui, 50, 50, 300, 450, 50, 20, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, -1);
	
	wz_create_textbox(gui, 0, 0, 200, 50, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, al_ustr_new("Welcome to WidgetZ!"), 1, -1);
	wz_create_toggle_button(gui, 0, 0, 200, 50, al_ustr_new("Toggle 1"), 1, 1, 5);
	wz_create_toggle_button(gui, 0, 0, 200, 50, al_ustr_new("Toggle 2"), 1, 1, 6);
	wz_create_toggle_button(gui, 0, 0, 200, 50, al_ustr_new("Toggle 3"), 1, 1, 7);
	
	wgt = (WZ_WIDGET*)wz_create_button(gui, 0, 0, 200, 50, al_ustr_new("Quit"), 1, 1);
	wz_set_shortcut(wgt, ALLEGRO_KEY_Q, ALLEGRO_KEYMOD_CTRL);
	
	wz_create_fill_layout(gui, 350, 50, 300, 450, 50, 20, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, -1);
	
	wz_create_textbox(gui, 0, 0, 200, 50, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, al_ustr_new("Scroll Bars:"), 1, -1);
	wz_create_scroll(gui, 0, 0, 200, 50, 20, 9);
	wz_create_scroll(gui, 0, 0, 50, 200, 20, 9);
	wz_create_scroll(gui, 0, 0, 50, 200, 20, 9);
	
	wz_create_fill_layout(gui, 650, 50, 300, 450, 20, 20, WZ_ALIGN_CENTRE, WZ_ALIGN_TOP, -1);
	wz_create_textbox(gui, 0, 0, 200, 50, WZ_ALIGN_CENTRE, WZ_ALIGN_CENTRE, al_ustr_new("Edit Box:"), 1, -1);
	wgt = (WZ_WIDGET*)wz_create_editbox(gui, 0, 0, 200, 50, al_ustr_new("Type here..."), 1, -1);
	wz_create_textbox(gui, 0, 0, 200, 50, WZ_ALIGN_LEFT, WZ_ALIGN_TOP, al_ustr_new("A textbox with a lot of text"
					  " in it. Also supports new lines:\n\nNew paragraph.\n"
					  "Also supports unicode:\n\n"
					  "Привет"), 1, -1);
	                  
	/*
	Register the gui with the event queue
	*/
	wz_register_sources(gui, queue);
	
	bool done = false;
	while (!done)
	{
		double dt = al_current_time() - old_time;
		al_rest(fixed_dt - dt); //rest at least fixed_dt
		dt = al_current_time() - old_time;
		old_time = al_current_time();
		
		if (old_time - game_time > dt)   //eliminate excess overflow
		{
			game_time += fixed_dt * floor((old_time - game_time) / fixed_dt);
		}
		
		double start_time = al_current_time();
		while (old_time - game_time >= 0)
		{
			game_time += fixed_dt;
			
			/*
			Update gui
			*/
			wz_update(gui, fixed_dt);
			
			ALLEGRO_EVENT event;
			while (al_get_next_event(queue, &event))
			{
				/*
				Give the gui the event, in case it wants it
				*/
				wz_send_event(gui, &event);
				switch (event.type)
				{
				case WZ_BUTTON_PRESSED:
					{
						switch ((int)event.user.data1)
						{
						case 1:
							{
								done = true;
								break;
							}
							
						}
						break;
					}
				}
			}
			
			if (al_current_time() - start_time > fixed_dt)//break if we start taking too long
				break;
		}
		
		al_clear_to_color(al_map_rgba_f(0, 0, 0, 1));
		
		/*
		Draw the gui
		*/
		wz_draw(gui);
		
		al_wait_for_vsync();
		al_flip_display();
	}
	
	/*
	Destroy the gui
	*/
	wz_destroy(gui);
	al_destroy_display(display);
	
	return 0;
}

