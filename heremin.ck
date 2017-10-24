// launch with hereminDebug

// create our OSC receiver
OscIn oin;
// create our OSC message
OscMsg msg;
// use port 12345
12345 => oin.port;
// create an address in the receiver
oin.addAddress( "/heremin, f f" );
float volume;
float freq;

// infinite event loop
while ( true )
{
	// wait for event to arrive
	oin => now;
	
	// grab the next message from the queue. 
	while ( oin.recv(msg) != 0 )
	{ 
		// getFloat fetches the expected float (as indicated by "f")
		msg.getFloat(0) => volume;
		msg.getFloat(1) => freq;
		// print
		<<< "got (via OSC) volume: ", volume, " pitch: ", freq >>>;
	}
}