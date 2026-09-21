#include <cstdio>

extern "C" {
#include <wayland-server-core.h>

struct wlr_backend;
struct wlr_renderer;
struct wlr_allocator;
struct wlr_output;

wlr_backend* hades_backend_create(wl_display* display);
int hades_backend_start(wlr_backend* backend);
void hades_backend_destroy(wlr_backend* backend);

void hades_backend_add_output_listener(
    wlr_backend* backend,
    wl_listener* listener
);

wlr_renderer* hades_renderer_create(wlr_backend* backend);

wlr_allocator* hades_allocator_create(
    wlr_backend* backend,
    wlr_renderer* renderer
);

int hades_output_init_render(
    wlr_output* output,
    wlr_allocator* allocator,
    wlr_renderer* renderer
);
}

struct Server {
    wl_display* display;
    wlr_backend* backend;
    wlr_renderer* renderer;
    wlr_allocator* allocator;

    wl_listener new_output;
};

static Server* g_server = nullptr;

static void handle_new_output(wl_listener* listener, void* data) {
    (void)listener;

    auto* output = static_cast<wlr_output*>(data);

    if (!hades_output_init_render(
            output,
            g_server->allocator,
            g_server->renderer)) {
        std::fprintf(
            stderr,
            "Failed to initialize output rendering\n"
        );
        return;
    }

    std::printf("Output rendering initialized\n");
}

int main() {
    Server server{};
    g_server = &server;

    server.display = wl_display_create();

    if (!server.display) {
        std::fprintf(stderr, "Failed to create Wayland display\n");
        return 1;
    }

    server.backend = hades_backend_create(server.display);

    if (!server.backend) {
        std::fprintf(stderr, "Failed to create wlroots backend\n");
        wl_display_destroy(server.display);
        return 1;
    }

    server.renderer = hades_renderer_create(server.backend);

    if (!server.renderer) {
        std::fprintf(stderr, "Failed to create renderer\n");
        hades_backend_destroy(server.backend);
        wl_display_destroy(server.display);
        return 1;
    }

    server.allocator =
        hades_allocator_create(server.backend, server.renderer);

    if (!server.allocator) {
        std::fprintf(stderr, "Failed to create allocator\n");
        hades_backend_destroy(server.backend);
        wl_display_destroy(server.display);
        return 1;
    }

    server.new_output.notify = handle_new_output;

    hades_backend_add_output_listener(
        server.backend,
        &server.new_output
    );

    const char* socket = wl_display_add_socket_auto(server.display);

    if (!socket) {
        std::fprintf(stderr, "Failed to create Wayland socket\n");
        hades_backend_destroy(server.backend);
        wl_display_destroy(server.display);
        return 1;
    }

    if (!hades_backend_start(server.backend)) {
        std::fprintf(stderr, "Failed to start wlroots backend\n");
        hades_backend_destroy(server.backend);
        wl_display_destroy(server.display);
        return 1;
    }

    std::printf(
        "Hades started on WAYLAND_DISPLAY=%s\n",
        socket
    );

    wl_display_run(server.display);

    hades_backend_destroy(server.backend);
    wl_display_destroy(server.display);

    return 0;
}
