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
#include "MeoPirelliKeygen.h"
#include <openssl/sha.h>
MeoPirelliKeygen::MeoPirelliKeygen(QString ssid, QString mac) :
        ArnetPirelliKeygen(ssid, mac) {
}


int MeoPirelliKeygen::getSupportState() const{
    if (getSsidName().startsWith("WiFi-Arnet-"))
        return SUPPORTED;
    return UNLIKELY;
}

QVector<QString> & MeoPirelliKeygen::getKeys() {

    if ( getMacAddress().length() < 12 ) {
        throw ERROR;
    }
    generateKey(incrementMac(getMacAddress(), -1), 8);
	return results;

}
