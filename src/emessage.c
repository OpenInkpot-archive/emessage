/*
 * Copyright © 2009 Mikhail Gusarov <dottedmag@dottedmag.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */
#include <string.h>
#include <err.h>
#include <stdbool.h>

#include <Edje.h>
#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>

void exit_all(void* param)
{
    ecore_main_loop_quit();
}

static void main_win_resize_handler(Ecore_Evas* main_win)
{
    int w, h;
    Evas* canvas = ecore_evas_get(main_win);
    evas_output_size_get(canvas, &w, &h);

    Evas_Object* edje = evas_object_name_find(canvas, "edje");
    evas_object_resize(edje, w, h);
}

static void main_win_key_handler(void* data, Evas* evas,
                                 Evas_Object* obj, void* event_info)
{
    Evas_Event_Key_Up* ev = event_info;
    if(!strcmp(ev->keyname, "Return"))
        ecore_main_loop_quit();
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: emessage <message>\n");
        return 1;
    }

    if(!evas_init())
        errx(1, "Unable to initialize Evas\n");
    if(!ecore_init())
        errx(1, "Unable to initialize Ecore\n");
    if(!ecore_evas_init())
        errx(1, "Unable to initialize Ecore_Evas\n");
    if(!edje_init())
        errx(1, "Unable to initialize Edje\n");

    ecore_x_io_error_handler_set(exit_all, NULL);

    Ecore_Evas* main_win = ecore_evas_software_x11_new(0, 0, 0, 0, 600, 800);
    ecore_evas_borderless_set(main_win, 0);
    ecore_evas_shaped_set(main_win, 0);
    ecore_evas_title_set(main_win, "emessage");
    ecore_evas_name_class_set(main_win, "emessage", "emessage");

    ecore_evas_callback_resize_set(main_win, main_win_resize_handler);

    Evas* main_canvas = ecore_evas_get(main_win);

    Evas_Object* edje = edje_object_add(main_canvas);
    evas_object_name_set(edje, "edje");
    edje_object_file_set(edje, DATADIR "/emessage/themes/emessage.edj", "emessage");
    evas_object_move(edje, 0, 0);
    evas_object_resize(edje, 600, 800);
    evas_object_show(edje);
    evas_object_focus_set(edje, true);

    evas_object_event_callback_add(edje, EVAS_CALLBACK_KEY_UP,
                                   &main_win_key_handler, NULL);

    edje_object_part_text_set(edje, "message", argv[1]);

    ecore_evas_show(main_win);

    ecore_main_loop_begin();

    edje_shutdown();
    ecore_evas_shutdown();
    ecore_shutdown();
    evas_shutdown();

    return 0;
}

