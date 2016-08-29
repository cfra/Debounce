/*
 * Copyright (C) 2016 Christian Franke
 *
 * This file is part of the debounce arduino library.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef PinDebouncer_h
#define PinDebouncer_h

#include "Arduino.h"
#include <inttypes.h>

class Debounce;

enum DebounceEdgeType {
	RisingEdge,
	FallingEdge
};

class PinDebouncer
{
	friend class Debounce;
public:
	/**
	 * Creates a new PinDebouncer for a pin.
	 * @param pin The input pin for which the debounce should be created.
	 * @param ms The time for which the pin needs to be stable to change state
	 */
	PinDebouncer(int pin, uint16_t ms);

	/**
	 * Returns the curent state of the pin associated with the debouncer.
	 * @return -1 if pin has not been stable yet, 0 or 1 otherwise.
	 */
	char read();

	/**
	 * Sets a callback to call when input state changes.
	 * @param cb Function to call. The function has one parameter which describes
	 * the direction of the edge.
	 */
	void setHandler(void (*cb)(enum DebounceEdgeType));
private:
	/**
	 * Samples the pin state and updates internal state accordingly.
	 * Should be called every milisecond
	 */
	void update();

	int pin; /* Stores the pin which we should sample */
	uint16_t wantedDur; /* The time the pin needs to be stable */
	char debouncedState; /* The state after debounce */
	char lastPinState; /* The last state of the pin */
	uint16_t stateDur; /* The time for which the pin has been stable */
	void (*edgeHandler)(enum DebounceEdgeType); /* Callback for changes */
};

#endif
