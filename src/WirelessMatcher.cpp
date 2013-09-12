/*
 * Copyright 2013 Rui Araújo, Luís Fonseca
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
#include "WirelessMatcher.h"
#include "config/AliceMagicInfo.h"
#include "config/TeleTuMagicInfo.h"
#include "config/AliceConfigParser.h"
#include "config/TeleTuConfigParser.h"
#include "config/OTEHuaweiConfigParser.h"
#include "algorithms/Keygen.h"
#include "algorithms/TecomKeygen.h"
#include "algorithms/TeleTuKeygen.h"
#include "algorithms/ThomsonKeygen.h"
#include "algorithms/VerizonKeygen.h"
#include "algorithms/InfostradaKeygen.h"
#include "algorithms/EircomKeygen.h"
#include "algorithms/SkyV1Keygen.h"
#include "algorithms/Wlan2Keygen.h"
#include "algorithms/ComtrendKeygen.h"
#include "algorithms/ZyxelKeygen.h"
#include "algorithms/Wlan6Keygen.h"
#include "algorithms/DiscusKeygen.h"
#include "algorithms/DlinkKeygen.h"
#include "algorithms/PirelliKeygen.h"
#include "algorithms/TelseyKeygen.h"
#include "algorithms/OnoKeygen.h"
#include "algorithms/HuaweiKeygen.h"
#include "algorithms/AliceItalyKeygen.h"
#include "algorithms/AliceGermanyKeygen.h"
#include "algorithms/ConnKeygen.h"
#include "algorithms/AxtelKeygen.h"
#include "algorithms/AndaredKeygen.h"
#include "algorithms/MegaredKeygen.h"
#include "algorithms/MaxcomKeygen.h"
#include "algorithms/InterCableKeygen.h"
#include "algorithms/OteKeygen.h"
#include "algorithms/OteBAUDKeygen.h"
#include "algorithms/OteHuaweiKeygen.h"
#include "algorithms/PBSKeygen.h"
#include "algorithms/PtvKeygen.h"
#include "algorithms/EasyBoxKeygen.h"
#include "algorithms/CabovisaoSagemKeygen.h"
#include "algorithms/Speedport500Keygen.h"
#include "algorithms/WifimediaRKeygen.h"
#include <QRegExp>

WirelessMatcher::WirelessMatcher() {
    supportedAlice = AliceConfigParser::readFile(":/alice.txt");
    supportedTeletu = TeleTuConfigParser::readFile(":/tele2.txt");
    supportedOTE = OTEHuaweiConfigParser::readFile(":/ote_huawei.txt");
}

WirelessMatcher::~WirelessMatcher() {
    QList<QString> keys = supportedAlice->keys();
    for (int i = 0; i < keys.size(); ++i) {
        QVector<AliceMagicInfo *> * supported = supportedAlice->value(
                keys.at(i));
        for (int j = 0; j < supported->size(); ++j)
            delete supported->at(j);
        delete supported;
    }
    supportedAlice->clear();
    delete supportedAlice;
    keys = supportedTeletu->keys();
    for (int i = 0; i < keys.size(); ++i) {
        QVector<TeleTuMagicInfo *> * supported = supportedTeletu->value(
                keys.at(i));
        for (int j = 0; j < supported->size(); ++j)
            delete supported->at(j);
        delete supported;
    }
    supportedTeletu->clear();
    delete supportedTeletu;
    delete supportedOTE;
}

QVector<Keygen *> * WirelessMatcher::getKeygens(QString ssid, QString mac) {
    QVector<Keygen *> * keygens = new QVector<Keygen*>;

    if (ssid.count(QRegExp("^[aA]lice-[0-9]{8}$")) == 1) {

        QVector<AliceMagicInfo *> * supported = supportedAlice->value(
                ssid.mid(6,3));
        if (supported != NULL && supported->size() > 0) {
            if (mac.length() < 6)
                mac = supported->at(0)->mac;
            keygens->append(new AliceItalyKeygen(ssid, mac, supported));
        }
    }

    if (mac.startsWith("00:1E:40") || mac.startsWith("00:25:5E"))
        keygens->append(new AliceGermanyKeygen(ssid, mac));

    if (ssid == "Andared")
        keygens->append(new AndaredKeygen(ssid, mac));

    if (ssid.count(QRegExp("^(AXTEL|AXTEL-XTREMO)-[0-9a-fA-F]{4}$"))==1) {
        QString ssidSubpart = ssid.right(4);
        QString macShort = mac.replace(":", "");
        if (macShort.length() == 12
            && ( ssidSubpart.toLower() == macShort.right(4).toLower()))
            keygens->append(new AxtelKeygen(ssid, mac));
    }

    if (ssid.count(QRegExp("^Cabovisao-[0-9a-fA-F]{4}$")) == 1) {
        if (mac.length() == 0 || mac.startsWith("C0:AC:54"))
            keygens->append(new CabovisaoSagemKeygen(ssid, mac));
    }

    if (ssid == "CONN-X")
        keygens->append(new ConnKeygen(ssid, mac));

    if (ssid.count(QRegExp("Discus--?[0-9a-fA-F]{6}$")) == 1)
        keygens->append(new DiscusKeygen(ssid, mac));

    if (ssid.count(QRegExp("DLink-[0-9a-fA-F]{6}$")) == 1)
        keygens->append(new DlinkKeygen(ssid, mac));


    if (mac.startsWith("00:12:BF") || mac.startsWith("00:1A:2A")
        || mac.startsWith("00:1D:19") || mac.startsWith("00:23:08")
        || mac.startsWith("00:26:4D") || mac.startsWith("50:7E:5D")
        || mac.startsWith("1C:C6:3C") || mac.startsWith("74:31:70")
        || mac.startsWith("7C:4F:B5") || mac.startsWith("88:25:2C"))
        keygens->append(new EasyBoxKeygen(ssid, mac));

    if (ssid.count(QRegExp("^[eE]ircom[0-7]{4} ?[0-7]{4}$")) == 1) {
        if (mac.length() == 0) {
            QString filteredSsid = ssid.replace(" ", "");
            QString end;
            bool status = false;
            int ssidNum = filteredSsid.right(8).toInt(&status, 8) ^ 0x000fcc;
            end.setNum(ssidNum, 16);
            while (end.size() < 6)
                end = "0" + end;
            end = end.toUpper();
            mac = "00:0F:CC:" + end.left(2) + ":" + end.mid(2, 2) + ":"
                  + end.right(2);
        }
        keygens->append(new EircomKeygen(ssid, mac));
    }

    if (ssid.count(QRegExp("^INFINITUM[0-9a-zA-Z]{4}$")) == 1
        || (mac.startsWith("00:0F:E2") || mac.startsWith("00:18:82")
            || mac.startsWith("00:1E:10") || mac.startsWith("00:22:A1")
            || mac.startsWith("00:25:68") || mac.startsWith("00:25:9E")
            || mac.startsWith("00:E0:FC") || mac.startsWith("04:C0:6F")
            || mac.startsWith("08:19:A6") || mac.startsWith("0C:37:DC")
            || mac.startsWith("10:1B:54") || mac.startsWith("10:C6:1F")
            || mac.startsWith("1C:1D:67") || mac.startsWith("20:2B:C1")
            || mac.startsWith("20:F3:A3") || mac.startsWith("24:DB:AC")
            || mac.startsWith("28:3C:E4") || mac.startsWith("28:5F:DB")
            || mac.startsWith("28:6E:D4") || mac.startsWith("30:87:30")
            || mac.startsWith("40:4D:8E") || mac.startsWith("4C:1F:CC")
            || mac.startsWith("4C:54:99") || mac.startsWith("4C:B1:6C")
            || mac.startsWith("54:89:98") || mac.startsWith("54:A5:1B")
            || mac.startsWith("5C:4C:A9") || mac.startsWith("64:16:F0")
            || mac.startsWith("70:7B:E8") || mac.startsWith("78:1D:BA")
            || mac.startsWith("78:1D:BA") || mac.startsWith("80:B6:86")
            || mac.startsWith("80:FB:06") || mac.startsWith("84:A8:E4")
            || mac.startsWith("84:A8:E4") || mac.startsWith("88:53:D4")
            || mac.startsWith("AC:E8:7B") || mac.startsWith("BC:76:70")
            || mac.startsWith("C8:D1:5E") || mac.startsWith("CC:96:A0")
            || mac.startsWith("E0:24:TF") || mac.startsWith("F4:55:9C")
            || mac.startsWith("F4:C7:14") || mac.startsWith("F8:3D:FF")
            || mac.startsWith("FC:48:EF")))
        keygens->append(new HuaweiKeygen(ssid, mac));

    if (ssid.count(QRegExp("^InfostradaWiFi-[0-9a-zA-Z]{6}$")) == 1)
        keygens->append(new InfostradaKeygen(ssid, mac));

    if (ssid.startsWith("InterCable") && mac.startsWith("00:15"))
        keygens->append(new InterCableKeygen(ssid, mac));

    if (ssid.count(QRegExp("^MAXCOM[0-9a-zA-Z]{4}$")) == 1)
        keygens->append(new MaxcomKeygen(ssid, mac));

    if (ssid.count(QRegExp("^Megared[0-9a-fA-F]{4}$")) == 1) {
        // the 4 characters of the SSID should match the final
        if (mac.length() == 0
            || (ssid.right(4) == mac.replace(":", "").right(4)))
            keygens->append(new MegaredKeygen(ssid, mac));
    }

    /* ssid must be of the form P1XXXXXX0000X or p1XXXXXX0000X */
    if (ssid.count(QRegExp("^[Pp]1[0-9]{6}0{4}[0-9]$")) == 1)
        keygens->append(new OnoKeygen(ssid, mac));

    if ((ssid.count(QRegExp("^OTE[0-9a-fA-F]{4}"))==1) && mac.startsWith("00:13:33"))
        keygens->append(new OteBAUDKeygen(ssid, mac));

    if (ssid.count(QRegExp("^OTE[0-9a-fA-F]{6}$")) == 1)
        keygens->append(new OteKeygen(ssid, mac));

    if (ssid.toUpper().startsWith("OTE") && (mac.startsWith("E8:39:DF:F5")
        || mac.startsWith("E8:39:DF:F6") || mac.startsWith("E8:39:DF:FD"))) {
        QString filteredMac = mac.replace(":", "");
        int target = filteredMac.mid(8).toInt(NULL, 16);
        if (filteredMac.length() == 12
            && target > (OteHuaweiKeygen::MAGIC_NUMBER - supportedOTE->length()))
            keygens->append(new OteHuaweiKeygen(ssid, mac,
                                       supportedOTE->at(OteHuaweiKeygen::MAGIC_NUMBER - target)));
    }

    if (ssid.count(QRegExp("^PBS-[0-9a-fA-F]{6}$")) == 1)
        keygens->append(new PBSKeygen(ssid, mac));

    if (ssid.count(
            QRegExp(
                    "^FASTWEB-1-(000827|0013C8|0017C2|00193E|001CA2|001D8B|002233|00238E|002553|00A02F|080018|3039F2|38229D|6487D7)[0-9A-Fa-f]{6}$"))
        == 1) {
        if (mac.length() == 0) {
            QString end = ssid.right(12);
            mac = end.left(2) + ":" + end.mid(2, 2) + ":" + end.mid(4, 2) + ":"
                  + end.mid(6, 2) + ":" + end.mid(8, 2) + ":" + end.right(2);
        }
        keygens->append(new PirelliKeygen(ssid, mac));
    }

    if (ssid.count(QRegExp("^(PTV-|ptv|ptv-)[0-9a-zA-Z]{6}$")) == 1)
        keygens->append(new PtvKeygen(ssid, mac));

    if (ssid.count(QRegExp("^SKY[0-9]{5}$")) == 1
        && (mac.startsWith("C4:3D:C7") || mac.startsWith("E0:46:9A")
            || mac.startsWith("E0:91:F5") || mac.startsWith("00:09:5B")
            || mac.startsWith("00:0F:B5") || mac.startsWith("00:14:6C")
            || mac.startsWith("00:18:4D") || mac.startsWith("00:26:F2")
            || mac.startsWith("C0:3F:0E") || mac.startsWith("30:46:9A")
            || mac.startsWith("00:1B:2F") || mac.startsWith("A0:21:B7")
            || mac.startsWith("00:1E:2A") || mac.startsWith("00:1F:33")
            || mac.startsWith("00:22:3F") || mac.startsWith("00:24:B2")))
        keygens->append(new SkyV1Keygen(ssid, mac));

    if (ssid.count(QRegExp("^WLAN-[0-9a-fA-F]{6}$")) == 1
            && (mac.startsWith("00:12:BF") || mac.startsWith("00:1A:2A") || mac
                    .startsWith("00:1D:19")))
        keygens->append(new Speedport500Keygen(ssid, mac));

    if (ssid.count(QRegExp("^TECOM-AH4(021|222)-[0-9a-zA-Z]{6}$")) == 1)
        keygens->append(new TecomKeygen(ssid, mac));

    if (ssid.toLower().startsWith("teletu")) {
        QString filteredMac = mac.replace(":", "");
        if (filteredMac.length() != 12 &&
            (ssid.count(QRegExp("^TeleTu_[0-9a-fA-F]{12}$")) == 1)){
            mac = filteredMac = ssid.mid(7);
        }
        if (filteredMac.length() == 12) {
            QVector<TeleTuMagicInfo *> *  supported = supportedTeletu
                                                      ->value(filteredMac.left(6));
            if (supported != NULL && supported->size() > 0) {
                int macIntValue = filteredMac.mid(6).toInt(NULL,16);
                for (int i = 0; i < supported->size(); ++i ) {
                    if (macIntValue >= supported->at(i)->range[0]
                        && macIntValue <= supported->at(i)->range[1]) {
                        keygens->append(new TeleTuKeygen(ssid, mac,
                                                supported->at(i)));
                    }
                }
            }
        }
    }

    if (ssid.count(QRegExp("^FASTWEB-(1|2)-(002196|00036F)[0-9A-Fa-f]{6}$"))
        == 1) {
        if (mac.length() == 0) {
            QString end = ssid.right(12);
            mac = end.left(2) + ":" + end.mid(2, 2) + ":" + end.mid(4, 2) + ":"
                  + end.mid(6, 2) + ":" + end.mid(8, 2) + ":" + end.right(2);
        }
        keygens->append(new TelseyKeygen(ssid, mac));
    }

    if (ssid.count(
            QRegExp(
                    "^(Thomson|Blink|SpeedTouch|O2Wireless|O2wireless|Orange-|ORANGE-|INFINITUM|BigPond|Otenet|Bbox-|DMAX|privat|TN_private_|CYTA|Vodafone-|Optimus|OptimusFibra|MEO-)[0-9a-fA-F]{6}$"))
        == 1)
        keygens->append(new ThomsonKeygen(ssid, mac));

    if (ssid.length() == 5
        && (mac.startsWith("00:1F:90") || mac.startsWith("A8:39:44")
            || mac.startsWith("00:18:01") || mac.startsWith("00:20:E0")
            || mac.startsWith("00:0F:B3") || mac.startsWith("00:1E:A7")
            || mac.startsWith("00:15:05") || mac.startsWith("00:24:7B")
            || mac.startsWith("00:26:62") || mac.startsWith("00:26:B8")))
        keygens->append(new VerizonKeygen(ssid, mac));

    if (ssid.count(QRegExp("^wifimedia_R-[0-9a-zA-Z]{4}$")) == 1
            && mac.replace(":", "").length() == 12)
        keygens->append(new WifimediaRKeygen(ssid, mac));

    if (ssid.count(QRegExp("^WLAN_[0-9a-zA-Z]{2}$")) == 1
        && (mac.startsWith("00:01:38") || mac.startsWith("00:16:38")
            || mac.startsWith("00:01:13") || mac.startsWith("00:01:1B")
            || mac.startsWith("00:19:5B")))
        keygens->append(new Wlan2Keygen(ssid, mac));

    if (ssid.count(QRegExp("^(WLAN|WiFi|YaCom)[0-9a-zA-Z]{6}$")) == 1)
        keygens->append(new Wlan6Keygen(ssid, mac));

    if (ssid.count(QRegExp("^(WLAN|JAZZTEL)_[0-9a-fA-F]{4}$")) == 1) {
        if (mac.startsWith("00:1F:A4") || mac.startsWith("F4:3E:61")
            || mac.startsWith("40:4A:03"))
            keygens->append(new ZyxelKeygen(ssid, mac));

        if (mac.startsWith("00:1B:20") || mac.startsWith("64:68:0C")
            || mac.startsWith("00:1D:20") || mac.startsWith("00:23:F8")
            || mac.startsWith("38:72:C0") || mac.startsWith("30:39:F2")
            || mac.startsWith("8C:0C:A3") || mac.startsWith("5C:33:8E")
            || mac.startsWith("C8:6C:87") || mac.startsWith("D0:AE:EC")
            || mac.startsWith("00:19:15") || mac.startsWith("00:1A:2B"))
            keygens->append(new ComtrendKeygen(ssid, mac));
    }
    return keygens;
}
