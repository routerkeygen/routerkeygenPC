/*
 * Copyright 2012 Rui Araújo, Luís Fonseca
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
#include "ArnetPirelliKeygen.h"
#include "AliceItalyKeygen.h"
#include <openssl/sha.h>
ArnetPirelliKeygen::ArnetPirelliKeygen(QString ssid, QString mac) :
        Keygen(ssid, mac) {
}

const QString ArnetPirelliKeygen::LOOKUP = "0123456789abcdefghijklmnopqrstuvwxyz";
const QString ArnetPirelliKeygen::SEED = "1236790";



int ArnetPirelliKeygen::getSupportState() const{
    if (getSsidName().startsWith("WiFi-Arnet-"))
        return SUPPORTED;
    return UNLIKELY;
}

QString ArnetPirelliKeygen::incrementMac(QString mac, int increment) {
    return QString::number(mac.toLong(0, 16) + increment, 16);
}

void ArnetPirelliKeygen::generateKey(QString mac) {
    SHA256_CTX sha;
    unsigned char hash[32];

    char macBytes[6];
    for (int i = 0; i < 12; i += 2) {
        macBytes[i / 2] = (mac.mid(i, 1).toInt(0, 16) << 4)
                + mac.mid(i + 1, 1).toInt(0, 16);
    }

    /* Compute the hash */
    SHA256_Init(&sha);
    SHA256_Update(&sha, (const void *) AliceItalyKeygen::ALICE_SEED, sizeof(AliceItalyKeygen::ALICE_SEED));
    SHA256_Update(&sha, (const void *) SEED.toLatin1().data(), SEED.size());
    SHA256_Update(&sha, (const void *) macBytes, sizeof(macBytes));
    SHA256_Final(hash, &sha);
    QString key = "";
    for (int i = 0; i < 10; ++i) {
        key += LOOKUP.at(hash[i] % LOOKUP.length());
    }
    results.append(key);

}

QVector<QString> & ArnetPirelliKeygen::getKeys() {

    if ( getMacAddress().length() < 12 ) {
        throw ERROR;
    }
    generateKey(incrementMac(getMacAddress(), 1));
    //Computing other possibilities
    for (int i = -2; i < 5; ++i){
        if (i != 1){
            generateKey(incrementMac(getMacAddress(), i));
        }
    }
	return results;

}
