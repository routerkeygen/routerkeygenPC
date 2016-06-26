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

#include "Upc07Keygen.h"
#include <stdio.h>
#include <openssl/md5.h>

Upc07Keygen::Upc07Keygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
    kgname = "Upc07";
}

int Upc07Keygen::getSupportState() const{
    if ( getSsidName().contains(QRegExp("^UPC[0-9]{7}$")) )
        return SUPPORTED;
    return UNLIKELY;
}

const quint32 Upc07Keygen::magic24 = 0xff8d8f20;
const quint32 Upc07Keygen::magic5  = 0xffd9da60;
const quint64 Upc07Keygen::magic0  = 0xb21642c9ll;
const quint64 Upc07Keygen::magic1  = 0x68de3afll;
const quint64 Upc07Keygen::magic2  = 0x6b5fca6bll;

const unsigned int Upc07Keygen::max0 = 9;
const unsigned int Upc07Keygen::max1 = 99;
const unsigned int Upc07Keygen::max2 = 9;
const unsigned int Upc07Keygen::max3 = 9999;

QString Upc07Keygen::hash2pass(quint8 *in_hash) {
	quint32 i, a;
	QString pass = "";

	for (i = 0; i < 8; i++) {
		a = in_hash[i] & 0x1f;
		a -= ((a * magic0) >> 36) * 23;

		a = (a & 0xff) + 0x41;

		if (a >= 'I') a++;
		if (a >= 'L') a++;
		if (a >= 'O') a++;

		pass += a;
	}
	
	return pass;
}

quint32 Upc07Keygen::mangle(quint32 *pp) {
	quint32 a, b;

	a = ((pp[3] * magic1) >> 40) - (pp[3] >> 31);
	b = (pp[3] - a * 9999 + 1) * 11ll;

	return b * (pp[1] * 100 + pp[2] * 10 + pp[0]);
}

quint32 Upc07Keygen::upc_generate_ssid(quint32* data, quint32 magic) {
	quint32 a, b;

	a = data[1] * 10 + data[2];
	b = data[0] * 2500000 + a * 6800 + data[3] + magic;

	return b - (((b * magic2) >> 54) - (b >> 31)) * 10000000;
}

QVector<QString> & Upc07Keygen::getKeys() {
    quint32 buf[4], target;
    char serial[64];
    char serial_input[64];
    char tmpstr[17];
    quint8 h1[16], h2[16];
    quint32 hv[4], w1, w2, i;
    int mode;
    QString pass;
    bool ok;

    QString ssid7 = getSsidName().right(7);
    target = ssid7.toInt(&ok, 10);
    if (!ok)
        throw ERROR;

    MD5_CTX ctx;

    for (buf[0] = 0; buf[0] <= max0; buf[0]++)
    for (buf[1] = 0; buf[1] <= max1; buf[1]++)
    for (buf[2] = 0; buf[2] <= max2; buf[2]++)
    for (buf[3] = 0; buf[3] <= max3; buf[3]++)
    for (mode = 1; mode <= 2; mode++) {
		if (mode == 1 && upc_generate_ssid(buf, magic24) != target)
			continue;
		if (mode == 2 && upc_generate_ssid(buf, magic5) != target)
			continue;

        sprintf(serial, "SAAP%d%02d%d%04d", buf[0], buf[1], buf[2], buf[3]);
        memset(serial_input, 0, 64);

        if (mode == 2) {
            for(i=0; i<strlen(serial); i++) {
                serial_input[strlen(serial)-1-i] = serial[i];
            }
        } else {
            memcpy(serial_input, serial, strlen(serial));
        }

        MD5_Init(&ctx);
        MD5_Update(&ctx, serial_input, strlen(serial_input));
        MD5_Final(h1, &ctx);

        for (i = 0; i < 4; i++) {
            hv[i] = *(quint16 *)(h1 + i*2);
        }

        w1 = mangle(hv);

        for (i = 0; i < 4; i++) {
            hv[i] = *(quint16 *)(h1 + 8 + i*2);
        }

        w2 = mangle(hv);

        sprintf(tmpstr, "%08X%08X", w1, w2);

        MD5_Init(&ctx);
        MD5_Update(&ctx, tmpstr, strlen(tmpstr));
        MD5_Final(h2, &ctx);

        results.append(hash2pass(h2));
    }

    return results;   
}
