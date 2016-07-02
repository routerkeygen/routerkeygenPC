/*
 * Copyright 2016 Alex Stanev <alex@stanev.org>
 *
 * Based on http://haxx.in/upc_keys.c
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

#ifndef UPC07KEYGEN_H
#define UPC07KEYGEN_H
#include "Keygen.h"

class Upc07Keygen: public Keygen {
private:
	const static quint32 magic24;
	const static quint32 magic5;
	const static quint64 magic0;
	const static quint64 magic1;
	const static quint64 magic2;
	
	const static unsigned int max0;
	const static unsigned int max1;
	const static unsigned int max2;
	const static unsigned int max3;
	const static char* prefixes[];

    QString hash2pass(quint8 *in_hash);
    quint32 mangle(quint32 *pp);
    quint32 upc_generate_ssid(quint32* data, quint32 magic);

	QVector<QString> & getKeys();
public:
	Upc07Keygen(QString ssid, QString mac);
	int getSupportState() const;
};

#endif //  UPC07KEYGEN_H
