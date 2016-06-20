/*
 * AxtelKeygen.cpp
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#include "TplinkKeygen.h"

TplinkKeygen::TplinkKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
}



QVector<QString> & TplinkKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.length() != 12 )
        throw ERROR;
    if (getSsidName().startsWith("MGTS")) {
        results.append(mac.mid(4).toLower());
    } else {
        results.append(mac.mid(4).toUpper());
    }
	return results;
}
