/*
 * AxtelKeygen.cpp
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#include "AxtelKeygen.h"

AxtelKeygen::AxtelKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
}



QVector<QString> & AxtelKeygen::getKeys() {
    QString mac = getMacAddress();
    QString ssidIdentifier  = getSsidName();
    
    if ( mac.length() != 12 )
        throw ERROR;
        
        unsigned char M1 = mac.mid(2,2).toInt(0, 16) + 1;
        unsigned char M2 = mac.mid(7,2);
        unsigned char S1 = ssidIdentifier.left(4);
        
        QString wpaKey = M1 + M2 + S1;
        
        results.append(wpaKey);
        results.append(mac.mid(2).toUpper());
	return results;
}
