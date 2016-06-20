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
#include "algorithms/ArcadyanKeygen.h"
#include "algorithms/CabovisaoSagemKeygen.h"
#include "algorithms/Speedport500Keygen.h"
#include "algorithms/WifimediaRKeygen.h"
#include "algorithms/BelkinKeygen.h"
#include "algorithms/TplinkKeygen.h"
#include "algorithms/ArnetPirelliKeygen.h"
#include "algorithms/SitecomKeygen.h"
#include "algorithms/HG824xKeygen.h"
#include "algorithms/SitecomWLR400xKeygen.h"
#include "algorithms/SitecomWLR2100Keygen.h"
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
            QString macProcessed = mac.replace(":", "").replace("-", "").toUpper();
            if (macProcessed.length() < 6 || macProcessed.left(6) != supported->at(0)->mac) {
                macProcessed = supported->at(0)->mac;
            } else {
                macProcessed = mac;
            }
            keygens->append(new AliceItalyKeygen(ssid, macProcessed, supported));
        }
    }

    if (mac.startsWith("00:1E:40") || mac.startsWith("00:25:5E"))
        keygens->append(new AliceGermanyKeygen(ssid, mac));

    if (ssid == "Andared")
        keygens->append(new AndaredKeygen(ssid, mac));

    if (mac.startsWith("00:12:BF") || mac.startsWith("00:1A:2A")
        || mac.startsWith("00:1D:19") || mac.startsWith("00:23:08")
        || mac.startsWith("00:26:4D") || mac.startsWith("50:7E:5D")
        || mac.startsWith("1C:C6:3C") || mac.startsWith("74:31:70")
        || mac.startsWith("7C:4F:B5") || mac.startsWith("84:9C:A6")
        || mac.startsWith("88:03:55") || mac.startsWith("88:25:2C"))
        keygens->append(new ArcadyanKeygen(ssid, mac));

    if (mac.startsWith("00:08:27") || mac.startsWith("00:13:C8")
        || mac.startsWith("00:17:C2") || mac.startsWith("00:19:3E")
        || mac.startsWith("00:1C:A2") || mac.startsWith("00:1D:8B")
        || mac.startsWith("00:22:33") || mac.startsWith("00:23:8E")
        || mac.startsWith("00:25:53") || mac.startsWith("00:8C:54")
        || mac.startsWith("30:39:F2") || mac.startsWith("38:22:9D")
        || mac.startsWith("64:87:D7") || mac.startsWith("74:88:8B")
        || mac.startsWith("84:26:15") || mac.startsWith("A4:52:6F")
        || mac.startsWith("A4:5D:A1") || mac.startsWith("D0:D4:12")
        || mac.startsWith("D4:D1:84") || mac.startsWith("DC:0B:1A")
        || mac.startsWith("F0:84:2F")) {
        keygens->append(new ArnetPirelliKeygen(ssid, mac));
    }

    if (ssid.count(QRegExp("^(AXTEL|AXTEL-XTREMO|AXTEL XTREMO)-[0-9a-fA-F]{4}$"))==1) {
        //QString ssidSubpart = ssid.right(4);
        //QString macShort = mac.replace(":", "");
        //if (macShort.length() == 12
        //    && ( ssidSubpart.toLower() == macShort.right(4).toLower()))
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

    if ((ssid.count(QRegExp("DLink-[0-9a-fA-F]{6}$")) == 1)
        || (ssid.count(QRegExp("dlink-[0-9]{2}$")) == 1))
        keygens->append(new DlinkKeygen(ssid, mac));

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
        || (mac.startsWith("00:18:82") || mac.startsWith("00:19:15") || mac.startsWith("00:1E:10")
             || mac.startsWith("00:22:A1") || mac.startsWith("00:25:68") || mac.startsWith("00:25:9E")
             || mac.startsWith("00:34:FE") || mac.startsWith("00:46:4B") || mac.startsWith("00:66:4B")
             || mac.startsWith("00:E0:FC") || mac.startsWith("00:F8:1C") || mac.startsWith("04:02:1F")
             || mac.startsWith("04:BD:70") || mac.startsWith("04:C0:6F") || mac.startsWith("04:F9:38")
             || mac.startsWith("08:19:A6") || mac.startsWith("08:63:61") || mac.startsWith("08:7A:4C")
             || mac.startsWith("08:E8:4F") || mac.startsWith("0C:37:DC") || mac.startsWith("0C:96:BF")
             || mac.startsWith("0C:D6:BD") || mac.startsWith("10:1B:54") || mac.startsWith("10:47:80")
             || mac.startsWith("10:51:72") || mac.startsWith("10:C6:1F") || mac.startsWith("14:B9:68")
             || mac.startsWith("18:C5:8A") || mac.startsWith("1C:1D:67") || mac.startsWith("1C:8E:5C")
             || mac.startsWith("20:08:ED") || mac.startsWith("20:0B:C7") || mac.startsWith("20:2B:C1")
             || mac.startsWith("20:F3:A3") || mac.startsWith("24:09:95") || mac.startsWith("24:1F:A0")
             || mac.startsWith("24:69:A5") || mac.startsWith("24:7F:3C") || mac.startsWith("24:9E:AB")
             || mac.startsWith("24:DB:AC") || mac.startsWith("28:31:52") || mac.startsWith("28:3C:E4")
             || mac.startsWith("28:5F:DB") || mac.startsWith("28:6E:D4") || mac.startsWith("2C:CF:58")
             || mac.startsWith("30:87:30") || mac.startsWith("30:D1:7E") || mac.startsWith("30:F3:35")
             || mac.startsWith("34:00:A3") || mac.startsWith("34:6B:D3") || mac.startsWith("34:CD:BE")
             || mac.startsWith("38:F8:89") || mac.startsWith("3C:47:11") || mac.startsWith("3C:DF:BD")
             || mac.startsWith("3C:F8:08") || mac.startsWith("40:4D:8E") || mac.startsWith("40:CB:A8")
             || mac.startsWith("44:55:B1") || mac.startsWith("48:46:FB") || mac.startsWith("48:62:76")
             || mac.startsWith("4C:1F:CC") || mac.startsWith("4C:54:99") || mac.startsWith("4C:8B:EF")
             || mac.startsWith("4C:B1:6C") || mac.startsWith("50:9F:27") || mac.startsWith("50:A7:2B")
             || mac.startsWith("54:39:DF") || mac.startsWith("54:89:98") || mac.startsWith("54:A5:1B")
             || mac.startsWith("58:1F:28") || mac.startsWith("58:2A:F7") || mac.startsWith("58:7F:66")
             || mac.startsWith("5C:4C:A9") || mac.startsWith("5C:7D:5E") || mac.startsWith("5C:B3:95")
             || mac.startsWith("5C:B4:3E") || mac.startsWith("5C:F9:6A") || mac.startsWith("60:DE:44")
             || mac.startsWith("60:E7:01") || mac.startsWith("64:16:F0") || mac.startsWith("64:3E:8C")
             || mac.startsWith("64:A6:51") || mac.startsWith("68:89:C1") || mac.startsWith("68:8F:84")
             || mac.startsWith("68:A0:F6") || mac.startsWith("68:A8:28") || mac.startsWith("70:54:F5")
             || mac.startsWith("70:72:3C") || mac.startsWith("70:7B:E8") || mac.startsWith("70:A8:E3")
             || mac.startsWith("74:88:2A") || mac.startsWith("74:A0:63") || mac.startsWith("78:1D:BA")
             || mac.startsWith("78:6A:89") || mac.startsWith("78:D7:52") || mac.startsWith("78:F5:FD")
             || mac.startsWith("7C:60:97") || mac.startsWith("7C:A2:3E") || mac.startsWith("80:38:BC")
             || mac.startsWith("80:71:7A") || mac.startsWith("80:B6:86") || mac.startsWith("80:D0:9B")
             || mac.startsWith("80:FB:06") || mac.startsWith("84:5B:12") || mac.startsWith("84:A8:E4")
             || mac.startsWith("84:DB:AC") || mac.startsWith("88:53:D4") || mac.startsWith("88:86:03")
             || mac.startsWith("88:A2:D7") || mac.startsWith("88:CE:FA") || mac.startsWith("88:E3:AB")
             || mac.startsWith("8C:34:FD") || mac.startsWith("90:17:AC") || mac.startsWith("90:4E:2B")
             || mac.startsWith("90:67:1C") || mac.startsWith("94:04:9C") || mac.startsWith("94:77:2B")
             || mac.startsWith("9C:28:EF") || mac.startsWith("9C:37:F4") || mac.startsWith("9C:C1:72")
             || mac.startsWith("A4:99:47") || mac.startsWith("A4:DC:BE") || mac.startsWith("AC:4E:91")
             || mac.startsWith("AC:85:3D") || mac.startsWith("AC:E2:15") || mac.startsWith("AC:E8:7B")
             || mac.startsWith("B0:5B:67") || mac.startsWith("B4:15:13") || mac.startsWith("B4:30:52")
             || mac.startsWith("B8:BC:1B") || mac.startsWith("BC:25:E0") || mac.startsWith("BC:76:70")
             || mac.startsWith("BC:9C:31") || mac.startsWith("C0:70:09") || mac.startsWith("C4:05:28")
             || mac.startsWith("C4:07:2F") || mac.startsWith("C8:51:95") || mac.startsWith("C8:D1:5E")
             || mac.startsWith("CC:53:B5") || mac.startsWith("CC:96:A0") || mac.startsWith("CC:A2:23")
             || mac.startsWith("CC:CC:81") || mac.startsWith("D0:2D:B3") || mac.startsWith("D0:3E:5C")
             || mac.startsWith("D0:7A:B5") || mac.startsWith("D4:40:F0") || mac.startsWith("D4:6A:A8")
             || mac.startsWith("D4:6E:5C") || mac.startsWith("D4:94:E8") || mac.startsWith("D4:B1:10")
             || mac.startsWith("D4:F9:A1") || mac.startsWith("D8:49:0B") || mac.startsWith("DC:D2:FC")
             || mac.startsWith("E0:19:1D") || mac.startsWith("E0:24:7F") || mac.startsWith("E0:36:76")
             || mac.startsWith("E0:97:96") || mac.startsWith("E4:35:C8") || mac.startsWith("E4:68:A3")
             || mac.startsWith("E4:C2:D1") || mac.startsWith("E8:08:8B") || mac.startsWith("E8:BD:D1")
             || mac.startsWith("E8:CD:2D") || mac.startsWith("EC:23:3D") || mac.startsWith("EC:38:8F")
             || mac.startsWith("EC:CB:30") || mac.startsWith("F4:55:9C") || mac.startsWith("F4:8E:92")
             || mac.startsWith("F4:9F:F3") || mac.startsWith("F4:C7:14") || mac.startsWith("F4:DC:F9")
             || mac.startsWith("F4:E3:FB") || mac.startsWith("F8:01:13") || mac.startsWith("F8:3D:FF")
             || mac.startsWith("F8:4A:BF") || mac.startsWith("F8:98:B9") || mac.startsWith("F8:BF:09")
             || mac.startsWith("F8:E8:11") || mac.startsWith("FC:48:EF") || mac.startsWith("FC:E3:3C")))
        keygens->append(new HuaweiKeygen(ssid, mac));

    if (mac.startsWith("00:18:82") || mac.startsWith("00:1E:10") || mac.startsWith("00:22:A1")
                    || mac.startsWith("00:25:68") || mac.startsWith("00:25:9E") || mac.startsWith("00:34:FE")
                    || mac.startsWith("00:46:4B") || mac.startsWith("00:66:4B") || mac.startsWith("00:E0:FC")
                    || mac.startsWith("00:F8:1C") || mac.startsWith("08:19:A6") || mac.startsWith("08:63:61")
                    || mac.startsWith("08:7A:4C") || mac.startsWith("08:E8:4F") || mac.startsWith("10:1B:54")
                    || mac.startsWith("10:47:80") || mac.startsWith("10:51:72") || mac.startsWith("10:C6:1F")
                    || mac.startsWith("20:08:ED") || mac.startsWith("20:0B:C7") || mac.startsWith("20:2B:C1")
                    || mac.startsWith("20:F3:A3") || mac.startsWith("28:31:52") || mac.startsWith("28:3C:E4")
                    || mac.startsWith("28:5F:DB") || mac.startsWith("28:6E:D4") || mac.startsWith("48:46:FB")
                    || mac.startsWith("48:62:76") || mac.startsWith("70:54:F5") || mac.startsWith("70:72:3C")
                    || mac.startsWith("70:7B:E8") || mac.startsWith("70:A8:E3") || mac.startsWith("80:38:BC")
                    || mac.startsWith("80:71:7A") || mac.startsWith("80:B6:86") || mac.startsWith("80:D0:9B")
                    || mac.startsWith("80:FB:06") || mac.startsWith("AC:4E:91") || mac.startsWith("AC:85:3D")
                    || mac.startsWith("AC:E2:15") || mac.startsWith("AC:E8:7B") || mac.startsWith("CC:53:B5")
                    || mac.startsWith("CC:96:A0") || mac.startsWith("CC:A2:23") || mac.startsWith("CC:CC:81")
                    || mac.startsWith("D4:40:F0") || mac.startsWith("D4:6A:A8") || mac.startsWith("D4:6E:5C")
                    || mac.startsWith("D4:94:E8") || mac.startsWith("D4:B1:10") || mac.startsWith("D4:F9:A1")
                    || mac.startsWith("E0:19:1D") || mac.startsWith("E0:24:7F") || mac.startsWith("E0:36:76")
                    || mac.startsWith("E0:97:96") || mac.startsWith("F8:01:13") || mac.startsWith("F8:3D:FF")
                    || mac.startsWith("F8:4A:BF") || mac.startsWith("F8:98:B9") || mac.startsWith("F8:BF:09")
                    || mac.startsWith("F8:E8:11"))
        keygens->append(new HG824xKeygen(ssid, mac));


    if (ssid.count(QRegExp("^InfostradaWiFi-[0-9a-zA-Z]{6}$")) == 1)
        keygens->append(new InfostradaKeygen(ssid, mac));

    if ((ssid.startsWith("InterCable") && mac.startsWith("00:15"))
        || ((ssid.count(QRegExp("^PTV[0-9]{4}$")) == 1) && mac.startsWith("54:E6:FC")))
        keygens->append(new InterCableKeygen(ssid, mac));

    // TODO: This is PSK based on BSSID with no additional changes. Can be implemented as separate keygen with options
    if ( (ssid.count(QRegExp("^MAXCOM[0-9a-zA-Z]{4}$")) == 1)
        || ssid.startsWith("Nemont")
        || ssid.startsWith("TURBONET")
        || ssid.startsWith("300NWLAN")
        || (ssid.count(QRegExp("^DJAWEB_[0-9A-F]{4}$")) == 1)
        || (ssid.count(QRegExp("^Claro-[0-9A-F]{4}$")) == 1)) // TODO: Those have also lowercase PSKs
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

    if ((ssid.count(QRegExp("^(OTE|ote)[0-9a-fA-F]{4}"))==1) && mac.startsWith("00:13:33"))
        keygens->append(new OteBAUDKeygen(ssid, mac));

    // TODO: Merge this with MaxcomKeygen - BSSID lowercase. Have some heuristics with no BSSID
    if ((ssid.count(QRegExp("^OTE[0-9a-f]{6}$")) == 1)
        || (ssid.count(QRegExp("^conn-x[0-9a-f]{6}$")) == 1)
        || (ssid.count(QRegExp("^Claro[0-9A-F]{4}$")) == 1)
        || (ssid.count(QRegExp("^Wind WiFi [0-9a-zA-Z]{6}$")) == 1))
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

    if (mac.startsWith("08:86:3B") || mac.startsWith("94:44:52")
        || mac.startsWith("EC:1A:59"))
        keygens->append(new BelkinKeygen(ssid, mac));

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

    if (mac.startsWith("00:0C:F6") || mac.startsWith("64:D1:A3"))
        keygens->append(new SitecomKeygen(ssid, mac));

    if (ssid.toLower().count(QRegExp("^sitecom[0-9a-f]{6}$")) == 1 ||
            (mac.startsWith("00:0C:F6") || mac.startsWith("64:D1:A3"))) {
        QString filteredMac = mac.replace(":", "");
        if (filteredMac.length() != 12) {
            QString computedMac = "00:0C:F6" + ssid.right(6);
            keygens->append(new SitecomWLR400xKeygen(ssid, computedMac));
            keygens->append(new SitecomWLR2100Keygen(ssid, computedMac));
            computedMac = "64:D1:A3" + ssid.right(6);
            keygens->append(new SitecomWLR400xKeygen(ssid, computedMac));
            keygens->append(new SitecomWLR2100Keygen(ssid, computedMac));
        } else {
            keygens->append(new SitecomWLR400xKeygen(ssid, mac));
            keygens->append(new SitecomWLR2100Keygen(ssid, mac));
        }
    }

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
                    "^(Thomson|Blink|SpeedTouch|O2Wireless|O2wireless|Orange-|ORANGE-|INFINITUM|BigPond|Otenet|Bbox-|DMAX|privat|TN_private_|CYTA|Vodafone-|Optimus|OptimusFibra|MEO-|Forthnet-)[0-9a-fA-F]{6}$"))
        == 1)
        keygens->append(new ThomsonKeygen(ssid, mac));



    if (mac.startsWith("F8:D1:11"))
        keygens->append(new TplinkKeygen(ssid, mac));

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

    if ((ssid.count(QRegExp("^(WLAN|JAZZTEL)_[0-9a-fA-F]{4}$")) == 1)
        || (ssid.count(QRegExp("^OTE[0-9A-F]{6}$")) == 1)) {
        if (mac.startsWith("00:1F:A4") || mac.startsWith("F4:3E:61")
            || mac.startsWith("40:4A:03"))
            keygens->append(new ZyxelKeygen(ssid, mac));

        if (mac.startsWith("00:1B:20") || mac.startsWith("64:68:0C")
            || mac.startsWith("00:1D:20") || mac.startsWith("00:23:F8")
            || mac.startsWith("38:72:C0") || mac.startsWith("30:39:F2")
            || mac.startsWith("8C:0C:A3") || mac.startsWith("5C:33:8E")
            || mac.startsWith("C8:6C:87") || mac.startsWith("D0:AE:EC")
            || mac.startsWith("00:19:15") || mac.startsWith("00:1A:2B")
            || mac.startsWith("F4:3E:61") || mac.startsWith("F8:8E:85"))
            keygens->append(new ComtrendKeygen(ssid, mac));
    }
    return keygens;
}
