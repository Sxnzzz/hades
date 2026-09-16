use std::sync::Arc;
use std::os::fd::AsRawFd;

use calloop::{
generic::Generic,
EventLoop,
Interest,
Mode,
};

use smithay::{
reexports::wayland_server::{
backend::{ClientData, ClientId, DisconnectReason},
Display,
},
wayland::socket::ListeningSocketSource,
};

struct ClientState;

impl ClientData for ClientState {
fn initialized(&self, client_id: ClientId) {
println!("Client initialized: {:?}", client_id);
}

fn disconnected(
    &self,
    client_id: ClientId,
    reason: DisconnectReason,
) {
    println!(
        "Client disconnected: {:?}, reason: {:?}",
        client_id, reason
    );
}

}

struct State {
display: Display<()>,
}

fn main() {
let display = Display::<()>::new().unwrap();

let mut state = State {
    display,
};

let listening_socket = ListeningSocketSource::new_auto().unwrap();

println!(
    "Socket name: {:?}",
    listening_socket.socket_name()
);

let mut event_loop: EventLoop<State> =
    EventLoop::try_new().unwrap();

let handle = event_loop.handle();

// get the wayland display FD as a raw FD.
let display_fd = state.display.backend().poll_fd().as_raw_fd();

// wayland display events.
handle
    .insert_source(
        Generic::new(
            unsafe {
                std::os::fd::BorrowedFd::borrow_raw(display_fd)
            },
            Interest::READ,
            Mode::Level,
        ),
        |_, _, state| {
            state.display.dispatch_clients(&mut ()).unwrap();

            Ok(calloop::PostAction::Continue)
        },
    )
    .unwrap();

// new wayland clients.
handle
    .insert_source(
        listening_socket,
        |client_stream, _, state| {
            println!("Client connected!");

            state
                .display
                .handle()
                .insert_client(
                    client_stream,
                    Arc::new(ClientState),
                )
                .unwrap();
        },
    )
    .unwrap();

event_loop
    .run(None, &mut state, |_| {})
    .unwrap();

}
