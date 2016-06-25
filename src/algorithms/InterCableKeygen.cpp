/*
 * InterCableKeygen.cpp
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#include "InterCableKeygen.h"

InterCableKeygen::InterCableKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
}

QVector<QString> & InterCableKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.length() != 12 )
        throw ERROR;

    QString shortMac = mac.left(10);
    int last = mac.right(2).toInt(0, 16);
    mac = shortMac + QString("%1").arg(last + 1, 2, 16, QLatin1Char('0')).right(2);
    results.append("m" + mac.toLower());
    mac = shortMac + QString("%1").arg(last + 2, 2, 16, QLatin1Char('0')).right(2);
    results.append("m" + mac.toLower());

	return results;
}
