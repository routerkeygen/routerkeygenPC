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
#include "SitecomWLR2100Keygen.h"
#include <QCryptographicHash>
#include <QDataStream>

SitecomWLR2100Keygen::SitecomWLR2100Keygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
}
const QString SitecomWLR2100Keygen::ALPHABET = "ABCDEFGHJKLMNPQRSTUVWXYZ";

QVector<QString> & SitecomWLR2100Keygen::getKeys() {
	QString mac = getMacAddress().toLower();

	if (mac.size() != 12)
		throw ERROR;

    QByteArray hash = QCryptographicHash::hash(mac.toLatin1(),
            QCryptographicHash::Md5);

    QDataStream halfmd5(hash.right(8));
    quint64 md5int;
    halfmd5 >> md5int;

	QString key = "";
	for (int i = 1; i <= 12; i += 1) {
        key += ALPHABET.at(md5int%24);
        md5int /= 24;
	}
	this->results.append(key);
	return results;
}
