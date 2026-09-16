use calloop::{
    generic::Generic,
    EventLoop,
    Interest,
    Mode,
};

// Our listening socket manages clients connecting to the compositor
use smithay::reexports::wayland_server::{
    Display,
    ListeningSocket,
};

// Smithay
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

	// wrap listening socket as calloop even source
	let source = Generic::new(socket, Interest::READ, Mode::Level,);

	// grab handle that lets us register in the calloop
	let handle = event_loop.handle();
	
	handle
		.insert_source(source, |event, _, _| {
			println!("Socket event: {:?}", event);

			Ok(calloop::PostAction::Continue)
		})
		.unwrap();

	// wait for stuff to happen...
	event_loop
		.run(None, &mut (), |_| {})
		.unwrap();
}
