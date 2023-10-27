/******************************************************************************
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
#include "obs-nix-wayland.h"

#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

// X11 only supports 256 scancodes, most keyboards dont have 256 keys so this should be reasonable.
#define MAX_KEYCODES 256
// X11 keymaps only have 4 shift levels, im not sure xkbcommon supports a way to shift the state into a higher level anyway.
#define MAX_SHIFT_LEVELS 4

struct obs_hotkeys_platform {
	struct wl_display *display;
	struct wl_seat *seat;
	struct wl_keyboard *keyboard;
	struct xkb_context *xkb_context;
	struct xkb_keymap *xkb_keymap;
	struct xkb_state *xkb_state;
	xkb_keysym_t key_to_sym[MAX_SHIFT_LEVELS][MAX_KEYCODES];
	uint32_t current_layout;
};

static void platform_keyboard_key(void *data, struct wl_keyboard *keyboard,
				  uint32_t serial, uint32_t time, uint32_t key,
				  uint32_t state)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(keyboard);
	UNUSED_PARAMETER(serial);
	UNUSED_PARAMETER(time);
	UNUSED_PARAMETER(key);
	UNUSED_PARAMETER(state);
	// We have access to the keyboard input here, but behave like other
	// platforms and let Qt inform us of key events through the platform
	// callbacks.
}

static void platform_keyboard_enter(void *data, struct wl_keyboard *keyboard,
				    uint32_t serial, struct wl_surface *surface,
				    struct wl_array *keys)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(keyboard);
	UNUSED_PARAMETER(serial);
	UNUSED_PARAMETER(surface);
	UNUSED_PARAMETER(keys);
	// Nothing to do here.
}

static void platform_keyboard_leave(void *data, struct wl_keyboard *keyboard,
				    uint32_t serial, struct wl_surface *surface)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(keyboard);
	UNUSED_PARAMETER(serial);
	UNUSED_PARAMETER(surface);
	// Nothing to do.
}

static void platform_keyboard_repeat_info(void *data,
					  struct wl_keyboard *keyboard,
					  int32_t rate, int32_t delay)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(keyboard);
	UNUSED_PARAMETER(rate);
	UNUSED_PARAMETER(delay);
	// Nothing to do.
}

static void platform_seat_name(void *data, struct wl_seat *seat,
			       const char *name)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(seat);
	UNUSED_PARAMETER(name);
	// Nothing to do.
}

const struct wl_seat_listener seat_listener = {
	.name = platform_seat_name,
};

static void platform_registry_handler(void *data, struct wl_registry *registry,
				      uint32_t id, const char *interface,
				      uint32_t version)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(registry);
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(interface);
	UNUSED_PARAMETER(version);
}

static void platform_registry_remover(void *data, struct wl_registry *registry,
				      uint32_t id)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(registry);
	UNUSED_PARAMETER(id);
	// Nothing to do.
}

const struct wl_registry_listener registry_listener = {
	.global = platform_registry_handler,
	.global_remove = platform_registry_remover,
};

void obs_nix_wayland_log_info(void)
{
	struct wl_display *display = obs_get_nix_platform_display();
	if (display == NULL) {
		blog(LOG_INFO, "Unable to connect to Wayland server");
		return;
	}
	//TODO: query some information about the wayland server if possible
	blog(LOG_INFO, "Connected to Wayland server");
}
