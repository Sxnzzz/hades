#include <cstdio>

extern "C" {
#include <wayland-server-core.h>
#include <wlr/backend.h>
}

int main() {
wl_display* display = wl_display_create();

```
if (!display) {
    std::fprintf(stderr, "Failed to create Wayland display\n");
    return 1;
}

wlr_backend* backend = wlr_backend_autocreate(display, nullptr);

if (!backend) {
    std::fprintf(stderr, "Failed to create wlroots backend\n");
    wl_display_destroy(display);
    return 1;
}

if (!wlr_backend_start(backend)) {
    std::fprintf(stderr, "Failed to start wlroots backend\n");
    wlr_backend_destroy(backend);
    wl_display_destroy(display);
    return 1;
}

const char* socket = wl_display_add_socket_auto(display);

if (!socket) {
    std::fprintf(stderr, "Failed to create Wayland socket\n");
    wlr_backend_destroy(backend);
    wl_display_destroy(display);
    return 1;
}

std::printf("Hades started on WAYLAND_DISPLAY=%s\n", socket);

wl_display_run(display);

wlr_backend_destroy(backend);
wl_display_destroy(display);

return 0;
}
