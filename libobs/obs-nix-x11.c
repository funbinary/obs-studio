/******************************************************************************
    Copyright (C) 2023 by Lain Bailey <lain@obsproject.com>
    Copyright (C) 2014 by Zachary Lund <admin@computerquip.com>
    Copyright (C) 2019 by Jason Francis <cycl0ps@tuta.io>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "obs-internal.h"
#include "obs-nix-platform.h"
#include "obs-nix-x11.h"

#include <xcb/xcb.h>
#if defined(XCB_XINPUT_FOUND)
#include <xcb/xinput.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib-xcb.h>
#include <X11/XF86keysym.h>
#include <X11/Sunkeysym.h>

void obs_nix_x11_log_info(void)
{
	Display *dpy = obs_get_nix_platform_display();
	if (!dpy) {
		blog(LOG_INFO, "Unable to open X display");
		return;
	}

	int protocol_version = ProtocolVersion(dpy);
	int protocol_revision = ProtocolRevision(dpy);
	int vendor_release = VendorRelease(dpy);
	const char *vendor_name = ServerVendor(dpy);

	if (strstr(vendor_name, "X.Org")) {
		blog(LOG_INFO,
		     "Window System: X%d.%d, Vendor: %s, Version: %d"
		     ".%d.%d",
		     protocol_version, protocol_revision, vendor_name,
		     vendor_release / 10000000, (vendor_release / 100000) % 100,
		     (vendor_release / 1000) % 100);
	} else {
		blog(LOG_INFO,
		     "Window System: X%d.%d - vendor string: %s - "
		     "vendor release: %d",
		     protocol_version, protocol_revision, vendor_name,
		     vendor_release);
	}
}

/* So here's how linux works with key mapping:
 *
 * First, there's a global key symbol enum (xcb_keysym_t) which has unique
 * values for all possible symbols keys can have (e.g., '1' and '!' are
 * different values).
 *
 * Then there's a key code (xcb_keycode_t), which is basically an index to the
 * actual key itself on the keyboard (e.g., '1' and '!' will share the same
 * value).
 *
 * xcb_keysym_t values should be given to libobs, and libobs will translate it
 * to an obs_key_t, and although xcb_keysym_t can differ ('!' vs '1'), it will
 * get the obs_key_t value that represents the actual key pressed; in other
 * words it will be based on the key code rather than the key symbol.  The same
 * applies to checking key press states.
 */

struct keycode_list {
	DARRAY(xcb_keycode_t) list;
};

struct obs_hotkeys_platform {
	Display *display;
	xcb_keycode_t min_keycode;
	xcb_keycode_t super_l_code;
	xcb_keycode_t super_r_code;

	/* stores a copy of the keysym map for keycodes */
	xcb_keysym_t *keysyms;
	int num_keysyms;
	int syms_per_code;

#if defined(XCB_XINPUT_FOUND)
	bool pressed[XINPUT_MOUSE_LEN];
	bool update[XINPUT_MOUSE_LEN];
	bool button_pressed[XINPUT_MOUSE_LEN];
#endif
};
