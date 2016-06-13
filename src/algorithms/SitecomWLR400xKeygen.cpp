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
#include "SitecomWLR400xKeygen.h"

SitecomWLR400xKeygen::SitecomWLR400xKeygen(QString ssid, QString mac) :
        Keygen(ssid, mac) {
}

const QString SitecomWLR400xKeygen::CHARSETS_341[]  = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", "W0X1CDYNJU8VOZA0BKL46PQ7RS9T2E5HI3MFG"};
const QString SitecomWLR400xKeygen::CHARSETS_4000[] = {"23456789ABCDEFGHJKLMNPQRSTUVWXYZ38BZ", "WXCDYNJU8VZABKL46PQ7RS9T2E5H3MFGPWR2"};
const QString SitecomWLR400xKeygen::CHARSETS_4004[] = {"JKLMNPQRST23456789ABCDEFGHUVWXYZ38BK", "E5MFJUWXCDKL46PQHAB3YNJ8VZ7RS9TR2GPW"};
const long SitecomWLR400xKeygen::MAGIC1 =  0x98124557;
const long SitecomWLR400xKeygen::MAGIC2 =  0x0004321a;
const long SitecomWLR400xKeygen::MAGIC3 =  0x80000000;

void SitecomWLR400xKeygen::generateKey(QString mac, const QString charsets[]) {

    long val = mac.mid(4).toLong(0, 16);
    int offsets[12];

    for (int i = 0; i < 12; ++i) {
        if ((val & 0x1) == 0) {
            val = val ^ MAGIC2;
            val = val >> 1;
        }
        else{
            val = val ^ MAGIC1;
            val = val >> 1;
            val = val | MAGIC3;
        }
        int offset = val % charsets[0].size();
        offsets[i] = offset;
    }
    QString wpakey;
    wpakey.append(charsets[0].at(offsets[0]));
    for (int i = 0; i < 11; ++i) {
        if (offsets[i] != offsets[i+1]) {
            wpakey.append(charsets[0].at(offsets[i+1]));
        }
        else {
            int newOffset = (offsets[i] + i) % charsets[0].size();
            wpakey.append(charsets[1].at(newOffset));
        }
   }
    results.append(wpakey);
}

QVector<QString> & SitecomWLR400xKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.length() < 12 ) {
        throw ERROR;
    }
    generateKey(mac, CHARSETS_341);
    generateKey(mac, CHARSETS_4000);
    generateKey(mac, CHARSETS_4004);
    
    QString shortMac = mac.left(11);
    int lastChar = mac.right(1).toInt(0, 16);
    lastChar = (lastChar + 1) % 0x10;
    generateKey(shortMac + QString::number(lastChar, 16), CHARSETS_341);
    generateKey(shortMac + QString::number(lastChar, 16), CHARSETS_4000);
    generateKey(shortMac + QString::number(lastChar, 16), CHARSETS_4004);
    lastChar = (lastChar + 3) % 0x10;
    generateKey(shortMac + QString::number(lastChar, 16), CHARSETS_341);
    generateKey(shortMac + QString::number(lastChar, 16), CHARSETS_4000);
    generateKey(shortMac + QString::number(lastChar, 16), CHARSETS_4004);

    return results;

}
