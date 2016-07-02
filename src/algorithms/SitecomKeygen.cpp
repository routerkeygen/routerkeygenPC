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
#include "SitecomKeygen.h"
#include <QRegExp>
#include <QStringList>

SitecomKeygen::SitecomKeygen(QString ssid, QString mac) :
        Keygen(ssid, mac) {
    kgname = "Sitecom";
}

const QString SitecomKeygen::CHARSET = "123456789abcdefghjkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ";

void SitecomKeygen::generateKey(QString mac) {
    QString key = "";
    int numericMac = mac.mid(6).split(QRegExp("[A-Fa-f]")).at(0).toInt();
    key += CHARSET.at(((numericMac + mac.at(11).unicode() + mac.at(5).unicode()) * (mac
          .at(9).unicode() + mac.at(3).unicode() + mac.at(11).unicode())) % CHARSET.length());
    key += CHARSET.at(((numericMac + mac.at(11).unicode() + mac.at(6).unicode()) * (mac
          .at(8).unicode() + mac.at(10).unicode() + mac.at(11).unicode())) % CHARSET.length());
    key += CHARSET.at(((numericMac + mac.at(3).unicode() + mac.at(5).unicode()) * (mac
          .at(7).unicode() + mac.at(9).unicode() + mac.at(11).unicode())) % CHARSET.length());
    key += CHARSET.at(((numericMac + mac.at(7).unicode() + mac.at(6).unicode()) * (mac
          .at(5).unicode() + mac.at(4).unicode() + mac.at(11).unicode()) % CHARSET.length()));
    key += CHARSET.at(((numericMac + mac.at(7).unicode() + mac.at(6).unicode()) * (mac
          .at(8).unicode() + mac.at(9).unicode() + mac.at(11).unicode())) % CHARSET.length());
    key += CHARSET.at(((numericMac + mac.at(11).unicode() + mac.at(5).unicode()) * (mac
          .at(3).unicode() + mac.at(4).unicode() + mac.at(11).unicode())) % CHARSET.length());
    key += CHARSET.at(((numericMac + mac.at(11).unicode() + mac.at(4).unicode()) * (mac
          .at(6).unicode() + mac.at(8).unicode() + mac.at(11).unicode())) % CHARSET.length());
    key += CHARSET.at(((numericMac + mac.at(10).unicode() + mac
          .at(11).unicode()) * (mac.at(7).unicode() + mac.at(8).unicode() + mac.at(11).unicode())) % CHARSET.length());
    results.append(key);

}

QVector<QString> & SitecomKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.length() < 12 ) {
        throw ERROR;
    }
    generateKey(mac.toLower());
    generateKey(mac.toUpper());
    QString shortMac = mac.left(11);
    int lastChar = mac.right(1).toInt(0, 16);
    lastChar = (lastChar + 1) % 0x10;
    generateKey(shortMac + QString::number(lastChar, 16).toUpper());
    lastChar = (lastChar + 1) % 0x10;
    generateKey(shortMac + QString::number(lastChar, 16).toUpper());
	return results;

}
