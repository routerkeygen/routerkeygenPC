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
#include "ComtrendKeygen.h"

ComtrendKeygen::ComtrendKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
	this->hash = new QCryptographicHash(QCryptographicHash::Md5);
}
ComtrendKeygen::~ComtrendKeygen(){
	delete hash;
}

const QString ComtrendKeygen::magic       = "bcgbghgg";
const QString ComtrendKeygen::lowermagic  = "64680C";
const QString ComtrendKeygen::highermagic = "3872C0";
const QString ComtrendKeygen::mac001a2b   = "001A2B";

QVector<QString> & ComtrendKeygen::getKeys() {
	QString mac   = getMacAddress();
    QString ssid4 = getSsidName().right(4);
    QString XX;
	if (mac.size() != 12)
		throw ERROR;

    if (mac.left(6) == mac001a2b) {
        for (int i=0; i<512; i++) {
            this->hash->reset();
            this->hash->addData(magic.toLatin1());
            
            if (i<256)  {
                this->hash->addData(lowermagic.toLatin1());
                XX = QString("%1").arg(i, 2, 16, QChar('0')).toUpper();
                this->hash->addData(XX.toLatin1());
            } else {
                this->hash->addData(highermagic.toLatin1());
                XX = QString("%1").arg(i-256, 2, 16, QChar('0')).toUpper();
                this->hash->addData(XX.toLatin1());                
            }
            
            this->hash->addData(ssid4.toLatin1());
            this->hash->addData(mac.toLatin1());

            QString result = QString::fromLatin1(this->hash->result().toHex().data());
            result.truncate(20);
            this->results.append(result);
        }
        return results;
    } else { 
    	this->hash->reset();
        this->hash->addData(magic.toLatin1());
    	QString macMod = mac.left(8) + ssid4.toLatin1();
        this->hash->addData(macMod.toUpper().toLatin1());
        this->hash->addData(mac.toLatin1());
        QString result = QString::fromLatin1(this->hash->result().toHex().data());
    	result.truncate(20);
    	this->results.append(result);
    	return results;
    }
}
