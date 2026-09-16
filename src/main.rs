use smithay::reexports::wayland_server::Display;
use smithay::reexports::wayland_server::ListeningSocket;
use calloop::EventLoop;


// Smithay

fn main(){
	// wayland display
	let display = Display::<()>::new().unwrap();

	// iterate to bind our socket
	let socket = ListeningSocket::bind_auto("wayland", 0..10).unwrap();

	// loop
	let mut event_loop: EventLoop<()> = EventLoop::try_new().unwrap();
	
	println!("Wayland display created: {:p}, socket as well: {:p}", &display, &socket);
	println!("\n Socket name: {:?}", socket.socket_name());

	event_loop
		.run(None, &mut (), |_| {})
		.unwrap();
}
