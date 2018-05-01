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

#include "BssidKeygen.h"

BssidKeygen::BssidKeygen(QString ssid, QString mac, int flags, int offset) :
		Keygen(ssid, mac) {
    kgname = "Bssid";
    this->flags = flags;
    this->offset = offset;
}

QVector<QString> & BssidKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.length() != 12 )
        throw ERROR;

    QString tMac;
    int last;

    /* Check flags and apply offest */
    if (flags & FlagLen8) {
        if (flags & FlagCutLeft) {
            tMac = mac.left(8);
        } else {
            tMac = mac.right(8);
        }
        last = tMac.right(6).toInt(0, 16);
        tMac = tMac.left(2) + QString("%1").arg(last + offset, 6, 16, QLatin1Char('0')).right(6);
        if (flags & FlagUc) {
            results.append(tMac.toUpper());
        }
        if (flags & FlagLc) {
            results.append(tMac.toLower());
        }
    }

    if (flags & FlagLen10) {
        if (flags & FlagCutLeft) {
            tMac = mac.left(10);
        } else {
            tMac = mac.right(10);
        }
        last = tMac.right(6).toInt(0, 16);
        tMac = tMac.left(4) + QString("%1").arg(last + offset, 6, 16, QLatin1Char('0')).right(6);
        if (flags & FlagUc) {
            results.append(tMac.toUpper());
        }
        if (flags & FlagLc) {
            results.append(tMac.toLower());
        }
    }

    if (flags & FlagLen12) {
        last = mac.right(6).toInt(0, 16);
        tMac = mac.left(6) + QString("%1").arg(last + offset, 6, 16, QLatin1Char('0')).right(6);
        if (flags & FlagUc) {
            results.append(tMac.toUpper());
        }
        if (flags & FlagLc) {
            results.append(tMac.toLower());
        }
    }

    if (results.isEmpty())
        throw ERROR;

    return results;
}
