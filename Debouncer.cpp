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

#include "Debouncer.h"

Debouncer::Debouncer(int pin, uint16_t ms)
	: pin(pin)
	, wanted_dur(ms)
	, debounced_state(-1)
	, last_pin_state(-1)
	, state_dur(0)
{}

char Debouncer::read()
{
	return debounced_state;
}

void Debouncer::update()
{
	char current_state = 0; /* TODO: DigitalRead */

	if (current_state != last_pin_state) {
		last_pin_state = current_state;
		state_dur = 0;
	} else {
		if (state_dur < wanted_dur) {
			state_dur++;
		} else {
			debounced_state = current_state;
		}
	}
}
