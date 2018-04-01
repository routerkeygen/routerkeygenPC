/*
 * Copyright 2018 Alex Stanev <alex@stanev.org>
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

#include "BaseXKeygen.h"

BaseXKeygen::BaseXKeygen(QString ssid, QString mac, int flags, int offset, int base, int nibbles) :
		Keygen(ssid, mac) {
    kgname = "BaseX";
    this->flags = flags;
    this->offset = offset;
    this->base = base;
    this->nibbles = nibbles;
}

QVector<QString> & BaseXKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.length() != 12 )
        throw ERROR;

    /* Cut nibbles */
    QString shortMac = mac.right(nibbles);
    
    /* Apply offset value and base */
    int ishortMac = shortMac.toInt(0, 16);
    QString res = QString("%1").arg(ishortMac + offset, 0, base, QLatin1Char('0'));
    
    /* Check flags */
    if (flags & xFlagLen8) {
        if (flags & xFlagUc)
            results.append(res.rightJustified(8, QLatin1Char('0'), true).toUpper());
        if (flags & xFlagLc)
            results.append(res.rightJustified(8, QLatin1Char('0'), true).toLower());
    }
    if (flags & xFlagLen10) {
        if (flags & xFlagUc)
            results.append(res.rightJustified(10, QLatin1Char('0'), true).toUpper());
        if (flags & xFlagLc)
            results.append(res.rightJustified(10, QLatin1Char('0'), true).toLower());
    }
    if (flags & xFlagLen12) {
        if (flags & xFlagUc)
            results.append(res.rightJustified(12, QLatin1Char('0'), true).toUpper());
        if (flags & xFlagLc)
            results.append(res.rightJustified(12, QLatin1Char('0'), true).toLower());
    }

    if (results.isEmpty())
        throw ERROR;

    return results;
}
