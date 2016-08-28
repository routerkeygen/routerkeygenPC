/*
 * Copyright 2016 Alex Stanev <alex@stanev.org>
 *
 * Based on https://github.com/yolosec/upcgen, ubee_keys.c
 *
 * This file is part of Router Keygen.
 *
 * Router Keygen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Router Keygen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Router Keygen.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UPC07UBEEKEYGEN_H
#define UPC07UBEEKEYGEN_H
#include "Keygen.h"

class Upc07UbeeKeygen: public Keygen {
private:
	const static char*   UBEE_NONINSULTING_ALPHABET;
	const static quint16 PROFANITY_COUNT;
	const static char*   profanities[];

	// Generates default SSID from source MAC
	qint8 ubee_generate_ssid(unsigned const char * mac, unsigned char * ssid, size_t * len);
	// Generates default passphrase from source MAC, with profanity checking
	qint8 ubee_generate_pass(unsigned const char * mac, unsigned char * passwd, size_t * len);
	// Generates default passphrase from source MAC, may contain profanity
	qint8 ubee_generate_pass_raw(unsigned const char * mac, unsigned char * hash_buff, unsigned char * passwd);
	// Generates a new default passphrase, if the original had profanity as a substring
	qint8 ubee_enerate_profanity_free_pass(unsigned char * hash_buff, unsigned char const * new_pass);
	// Math ADD operation on MAC address
	void incmac(unsigned char * mac, unsigned char * newmac, qint8 delta);
	// Read MAC from hex string
	qint8 readmac(char const * machex, unsigned char * mac);

	QVector<QString> & getKeys();
public:
	Upc07UbeeKeygen(QString ssid, QString mac);
	int getSupportState() const;
};

#endif //  UPC07UBEEKEYGEN_H
