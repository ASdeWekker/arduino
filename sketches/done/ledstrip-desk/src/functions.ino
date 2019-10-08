// Function for changing the ledstrip's color.
void stripColor(int color, bool power) {
	if (power == true) {
		FastLED.showColor(CHSV(color, 255, brightnessInt));
	} else if (power == false) {
		FastLED.show(CRGB::Black);
	}
}


// A function for turning the ledstrip on or off.
void power() {
	// Checks which argument has been passed and turns the strip on or off.
	if (server.arg("power") == "on") {
		stripColor(ccolor, true);
		check = true;
	} else if (server.arg("power") == "off") {
		stripColor(ccolor, false);
		check = false;
		rainbowSet = false;
	} else if (server.arg("power") == "toggle") {
		if (check == true) {
			stripColor(ccolor, false);
			check = false;
			rainbowSet = false;
		} else {
			stripColor(ccolor, true);
			check = true;
		}
	}

	// Send a message back to the client.
	server.send(200, "text/plain", "Processed.\n");
}


// A function for choosing a preprogrammed color.
void color() {
	// Check which color has been passed and act accordingly.
	if (server.arg("color") == "red") {
		ccolor = 0;
	} else if (server.arg("color") == "orange") {
		ccolor = 32;
	} else if (server.arg("color") == "yellow") {
		ccolor = 64;
	} else if (server.arg("color") == "green") {
		ccolor = 96;
	} else if (server.arg("color") == "aqua") {
		ccolor = 128;
	} else if (server.arg("color") == "blue") {
		ccolor = 160;
	} else if (server.arg("color") == "purple") {
		ccolor = 192;
	} else if (server.arg("color") == "pink") {
		ccolor = 224;
	}

	check = true;
	rainbowSet = false;

	// Set the color and send the processed message.
	stripColor(ccolor, true);
	server.send(200, "text/plain", "Set the color: " + server.arg("color") + ".\n");
}


// A function to change the color via an rgb value.
void rgb() {
	// Going to make a function which takes an rgb value and converts it to an hsv value.

	// Send a message back to the client.
	server.send(200, "text/plain", "This function is not yet implemented.\n");
	// server.send(200, "text/plain", "Processed the rgb color and converted it to hsv.\n");
}


// A function to change the color via an hsv value.
void hsv() {
	// This function takes an int and puts it into ccolor.
	ccolor = server.arg("hsv").toInt();
	stripColor(ccolor, true);

	// Send a message back to the client.
	server.send(200, "text/plain", "Processed the hsv color.\n");
}


// A function to emit a rainbow at a certain speed.
void rainbow() {
	rainbowSpeed = server.arg("speed").toInt();
	rainbowSet = true;
	check = true;

	// Send a message back to the client.
	server.send(200, "text/plain", "Turned on the rainbow.\n");
}


// A function to change the brightness.
void brightness() {
	brightnessInt = server.arg("brightness").toInt();
	stripColor(ccolor, true);

	// Send a message back to the client.
	server.send(200, "text/plain", "Changed brightness.\n");
}