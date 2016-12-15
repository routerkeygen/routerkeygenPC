/*
 * Copyright 2016 Alex Stanev <alex@stanev.org>
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

#include "PldtKeygen.h"

PldtKeygen::PldtKeygen(QString ssid, QString mac, unsigned short int var):
		Keygen(ssid, mac) {
    kgname = "Pldt";
    this->var = var;
}

QVector<QString> & PldtKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.length() != 12 )
        throw ERROR;

    switch (var) {
        case 0: {
            results.append("PLDTWIFI" + mac.right(5).toUpper());
            break;            
        }
        case 1: {
            unsigned long int last = mac.right(6).toInt(0, 16) ^ 0xffffff;
            results.append("wlan" + QString("%1").arg(last, 6, 16, QLatin1Char('0')).right(6).toLower());
            break;
        }
        default:
            throw ERROR;
    }

    return results;
}
