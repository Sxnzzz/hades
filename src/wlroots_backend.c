#include <wayland-server-core.h>

#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_output.h>

struct wlr_backend *hades_backend_create(struct wl_display *display) {
    struct wl_event_loop *loop = wl_display_get_event_loop(display);

    return wlr_backend_autocreate(loop, NULL);
}

int hades_backend_start(struct wlr_backend *backend) {
    return wlr_backend_start(backend);
}

void hades_backend_destroy(struct wlr_backend *backend) {
    wlr_backend_destroy(backend);
}

void hades_backend_add_output_listener(
    struct wlr_backend *backend,
    struct wl_listener *listener
) {
    wl_signal_add(&backend->events.new_output, listener);
}

struct wlr_renderer *hades_renderer_create(
    struct wlr_backend *backend
) {
    return wlr_renderer_autocreate(backend);
}

struct wlr_allocator *hades_allocator_create(
    struct wlr_backend *backend,
    struct wlr_renderer *renderer
) {
    return wlr_allocator_autocreate(backend, renderer);
}

int hades_output_init_render(
    struct wlr_output *output,
    struct wlr_allocator *allocator,
    struct wlr_renderer *renderer
) {
    return wlr_output_init_render(output, allocator, renderer);
}
