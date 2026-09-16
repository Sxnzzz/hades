use smithay::reexports::wayland_server::{Display, ListeningSocket};
use calloop::EventLoop;


// Smithay
// Our listening socket manages clients connecting to the compositor

fn main(){
	// wayland display
	let display = Display::<()>::new().unwrap();
	let display_handle = display.handle();

	// iterate to bind our socket
	let socket = ListeningSocket::bind_auto("wayland", 0..10).unwrap();

	// loop
	let mut event_loop: EventLoop<()> = EventLoop::try_new().unwrap();
	
	println!("Wayland display created: {:p}, socket as well: {:p}", &display, &socket);
	println!("\n Socket name: {:?}", socket.socket_name());
	println!("\n Display Handle: {:?}", display_handle);

	// wait for stuff to happen...
	event_loop
		.run(None, &mut (), |_| {})
		.unwrap();
}
