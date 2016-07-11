/*
 * AxtelKeygen.cpp
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#include "AxtelKeygen.h"

AxtelKeygen::AxtelKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
    kgname = "Axtel";
}

QVector<QString> & AxtelKeygen::getKeys() {
    QString mac = getMacAddress();
    QString macShort = mac.replace(":", "");
    bool gotResult = false;
    
    if ( macShort.length() != 12 )
        throw ERROR;
        
    QString ssidSubpart = getSsidName().right(4);
    if ( ssidSubpart.toLower() == macShort.right(4).toLower()) {
        results.append(mac.mid(2).toUpper());
        gotResult = true;
    }

    //WPA = (Second octet + 1) + (Fourth octect) + (Fifth octect) + (Sixth octect - 1)
    if (macShort.right(2).toInt(0, 16)-1 == ssidSubpart.right(2).toInt(0, 16)) {
        QString M1 = QString("%1").arg(mac.mid(2, 2).toInt(0, 16) + 1, 2, 16, QChar('0'));
        QString M2 = mac.mid(6, 2);
       
        QString wpaKey = M1 + M2 + ssidSubpart;

        results.append(wpaKey.toUpper());
	    gotResult = true;
	}
	
	if (gotResult) {
	    return results;
	} else {
	    throw ERROR;
	}
}
