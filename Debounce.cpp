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
#include "Debounce.h"
#include "PinDebouncer.h"

#ifndef NO_DEBOUNCE_TIMER
/* This function gets called every 1ms from timer1 */
ISR(TIMER1_OVF_vect)
{
	Debounce::instance().update();
}
#endif

Debounce &Debounce::instance()
{
	if (!self) {
		self = new Debounce();
	}
	return *self;
}

void Debounce::update()
{
	Debounce::instance().do_update();
}

void Debounce::do_update()
{
	for (unsigned char i = 0; i < pin_count; i++) {
		pin_list[i]->update();
	}
}

void Debounce::register_pin(PinDebouncer *db)
{
	if (pin_list_len == 0) {
		/* Initialize an array for 16 inputs */
		pin_list_len = 16;
		pin_list = new PinDebouncer* [pin_list_len];
	}
	if (pin_count >= pin_list_len) {
		/* If array is too small, double its size */
		PinDebouncer **new_list;
		pin_list_len = 2 * pin_list_len;
		new_list = new PinDebouncer* [pin_list_len];
		for (unsigned char i = 0; i < pin_count; i++) {
			new_list[i] = pin_list[i];
		}
		delete [] pin_list;
		pin_list = new_list;
	}
	pin_list[pin_count++] = db;
}

Debounce::Debounce()
{
#ifndef NO_DEBOUNCE_TIMER
	/* Calculate time for 1ms - CS11 equals prescaler of 8, so divide
	 * MCU frequency by 8 before dividing to 1000 to get ticks per 1ms */
	OCR1A = (F_CPU / 8 / 1000) + 1;
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS11); /* Set CTC mode and /8 scaler */
	TIMSK1 |= (1 << OCIE1A);
	sei();
#endif
}
