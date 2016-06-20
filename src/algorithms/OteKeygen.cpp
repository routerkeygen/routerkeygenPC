/*
 * OteKeygen.cpp
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#include "OteKeygen.h"

OteKeygen::OteKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
}


QVector<QString> & OteKeygen::getKeys() {
    if (getMacAddress().length() == 12) {
        results.append(getMacAddress().toLower());
    } else {
        QString ssid = getSsidName();
        QString ssidIdentifier = ssid.right(6);
        if (ssid.startsWith("OTE")) {
            results.append("c87b5b" + ssidIdentifier);
            results.append("fcc897" + ssidIdentifier);
            results.append("681ab2" + ssidIdentifier);
            results.append("b075d5" + ssidIdentifier);
            results.append("384608" + ssidIdentifier);
        } else if (ssid.startsWith("conn-x")) {
            results.append("146080" + ssidIdentifier);
            results.append("2c26c5" + ssidIdentifier);
            results.append("4c09b4" + ssidIdentifier);
            results.append("5422f8" + ssidIdentifier);
            results.append("a0ec80" + ssidIdentifier);
            results.append("cc1afa" + ssidIdentifier);
            results.append("dc028e" + ssidIdentifier);
        } else {
            throw ERROR;
        }
    }
	return results;
}



