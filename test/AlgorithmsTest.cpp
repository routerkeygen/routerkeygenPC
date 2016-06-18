#include <QtTest/QtTest>
#include <typeinfo>
#include <QString>
#include <QVector>
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
#include "WirelessMatcher.h"
#include "wifi/QScanResult.h"
#include <QDebug>


class AlgorithmsTest: public QObject
{

    Q_OBJECT
    WirelessMatcher matcher;
private slots:

    void test(){

    }

    void testAliceItaly() {
        QScanResult wifi("Alice-53847953", "00:25:53:35:a7:91");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(AliceItalyKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("7nfyuqlahytaml3bkcjasmtf"));
    }

    void testAliceItaly2() {
        QScanResult wifi("Alice-37588990", "00:23:8e:48:e7:d4");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(AliceItalyKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),4);
        QCOMPARE(results.at(3),QString("9j4hm3ojq4brfdy6wcsuglwu"));
    }

    void testAliceItaly3() {
        QScanResult wifi("Alice-95535232", "00:8c:54:07:de:08");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(AliceItalyKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("e3eudsvbuu2i8zz2yalosd65"));
    }

    void testAliceItaly4() {
        QScanResult wifi("Alice-53023425", "00:25:53:05:e3:50");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(AliceItalyKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),4);
        QCOMPARE(results.at(3),QString("gi0wdaa3crf6wsb53sf7bv5t"));
    }

    void testAliceGermany() {
        QScanResult wifi("ALICE-WLANC3","00:1E:40:A0:84:C4");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(AliceGermanyKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("MGIwMjhjYTYzZmM0"));
    }

    void testArcadyan() {
        QScanResult wifi("Arcor-910B02", "00:12:BF:91:0B:EC");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(ArcadyanKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("F9C8C9DEF"));
    }

    void testArcadyan2() {
        QScanResult wifi("VodafoneGG11", "74:31:70:33:00:11");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(ArcadyanKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),2);
        QCOMPARE(results.at(0),QString("58639029A"));
        QCOMPARE(results.at(1),QString("58639129A"));
    }

    void testArnetPirelli() {
        QScanResult wifi("WiFi-Arnet-0184", "74:88:8B:27:2B:F4");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(ArnetPirelliKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),7);
        QCOMPARE( results.at(0),QString("hcckr5ch38"));
        QCOMPARE( results.at(3),QString("781haylokm"));
    }

    void testArnetPirelli2() {
        QScanResult wifi("ADSLPT-AB65637", "f0:84:2f:83:56:a2");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(ArnetPirelliKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),7);
        QCOMPARE( results.at(0),QString("ds7prly5"));
    }

    void testBelkin() {
        QScanResult wifi("Belkin.c0de","94:44:52:00:C0:DE");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(BelkinKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("040D93B0"));
    }

    void testBelkin2() {
        QScanResult wifi("belkin.ed0","94:44:52:00:ce:d0");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(BelkinKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("d49496b9"));
    }

    void testCONN() {
        QScanResult wifi("CONN-X", "");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(ConnKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("1234567890123"));
    }

    void testDiscus() {
         QScanResult wifi("Discus--DA1CC5", "00:1C:A2:DA:1C:C5");
         wifi.checkSupport(matcher);
         QVector<Keygen *> * keygens = wifi.getKeygens();
         QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
         Keygen * keygen = keygens->at(1);
         QCOMPARE(typeid(*keygen), typeid(DiscusKeygen) );
         QVector<QString> results = keygen->getResults();
         QCOMPARE(results.size(),1);
         QCOMPARE(results.at(0),QString("YW0150565"));
     }

    void testDlink() {
        QScanResult wifi("DLink-123456", "12:34:56:78:9a:bc");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(DlinkKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE( results.at(0),QString("6r8qwaYHSNdpqdYw6aN8"));
    }

    void testEircom() {
        QScanResult wifi("eircom2633 7520", "00:0f:cc:59:b0:9c");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(EircomKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE( results.at(0),QString("29b2e9560b3a83a187ec5f2057"));
    }

    void testHuawei() {
        QScanResult wifi("INFINITUM1be2", "64:16:F0:35:1C:FD");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(HuaweiKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("3432333133"));
    }
    void testHuawei_HG24x() {
        QScanResult wifi("", "00:46:4B:D3:CE:5F");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(1);
        QCOMPARE(typeid(*keygen), typeid(HG824xKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("D3CE560D"));
    }
    void testHuawei_HG24x2() {
        QScanResult wifi("", "E0:24:7F:E5:80:01");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(1);
        QCOMPARE(typeid(*keygen), typeid(HG824xKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("E57FF80C"));
    }

    void testMegared() {
        QScanResult wifi("Megared60EC", "FC:75:16:9F:60:EC");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(MegaredKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0), QString("75169F60EC"));
    }

    void testOTE() {
        QScanResult wifi("OTE37cb4c", "B0:75:D5:37:CB:4C");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(OteKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE( results.at(0),QString("b075d537cb4c"));
    }

    void testOTEBAUD() {
        QScanResult wifi("OTEcb4c", "00:13:33:37:CB:4C");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(OteBAUDKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("000133337cb4c"));
    }

    void testOTEHuawei() {
        QScanResult wifi("OTEcb4c", "E8:39:DF:F5:12:34");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(OteHuaweiKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0),QString("54919345"));
    }

    void testPBS() {
        QScanResult wifi("PBS-11222E", "38:22:9D:11:22:33");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(1);
        QCOMPARE(typeid(*keygen), typeid(PBSKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE( results.at(0),QString("PcL2PgUcX0VhV"));
    }

    void testSitecom() {
        QScanResult wifi("Sitecom", "00:0C:F6:01:23:45");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(SitecomKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),4);
        QCOMPARE( results.at(0),QString("c6aBBBbg"));
        QCOMPARE( results.at(1),QString("q69CB1kg"));
        QCOMPARE( results.at(2),QString("44B5Dmyc"));
        QCOMPARE( results.at(3),QString("N66zHMRc"));
    }


    void testSitecom2() {
        QScanResult wifi("Sitecom", "00:0C:F6:01:2F:F5");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(SitecomKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),4);
        QCOMPARE( results.at(0),QString("zb1cgzvx"));
        QCOMPARE( results.at(1),QString("6sWNS6D6"));
        QCOMPARE( results.at(2),QString("JEmMRJSk"));
        QCOMPARE( results.at(3),QString("qVMKQq7B"));
    }


    void testSitecom3() {
        QScanResult wifi("Sitecom", "00:0C:F6:01:2F:FE");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(SitecomKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),4);
        QCOMPARE( results.at(0),QString("ZB1kqZVQ"));
        QCOMPARE( results.at(1),QString("DhCvzDve"));
        QCOMPARE( results.at(2),QString("S63uySh4"));
        QCOMPARE( results.at(3),QString("tURTXt6t"));
    }


    void testSitecom4() {
        QScanResult wifi("Sitecom", "00:0C:F6:F1:2F:FE");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(SitecomKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),4);
        QCOMPARE( results.at(0),QString("6P2AB6Pb"));
        QCOMPARE( results.at(1),QString("GEBAWGEA"));
        QCOMPARE( results.at(2),QString("GDPK6GDb"));
        QCOMPARE( results.at(3),QString("6sq9v6sp"));
    }

    void testSitecomWRL400X() {
        QScanResult wifi("SitecomF12FFE", "00:0C:F6:F1:2F:FE");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(1);
        QCOMPARE(typeid(*keygen), typeid(SitecomWLR400xKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),9);
        QCOMPARE( results.at(0),QString("E0B0HTVYI8D9"));
        QCOMPARE( results.at(1),QString("G2D2KXZBLAFB"));
        QCOMPARE( results.at(2),QString("6J3J9XZBAS5T"));
    }

    void testSitecomWRL400X2() {
        QScanResult wifi("SitecomF12FFE", "");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(SitecomWLR400xKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),9);
        QCOMPARE( results.at(0),QString("E0B0HTVYI8D9"));
        QCOMPARE( results.at(1),QString("G2D2KXZBLAFB"));
        QCOMPARE( results.at(2),QString("6J3J9XZBAS5T"));
    }

    void testSpeedport500() {
        QScanResult wifi("WLAN-903704", "00:1D:19:90:37:DD");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        QCOMPARE(typeid(*(keygens->at(0))), typeid(ArcadyanKeygen) );
        QVERIFY2(keygens->size() == 2, "2 algorithm should have been detected");
        Keygen * keygen = keygens->at(1);
        QCOMPARE(typeid(*keygen), typeid(Speedport500Keygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1000);
        bool found = false;
        for ( int i = 0; i < results.size() ; ++i ){
            if (  results.at(i) == "SP-0947DD059" ){
                found = true;
                break;
            }
        }
        QVERIFY2(found, "SP-0947DD059 was not found");
    }

    void testTeletu() {
        QScanResult wifi("TeleTu_00238EE528C7", "00:23:8E:E5:28:C7");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(1);
        QCOMPARE(typeid(*keygen), typeid(TeleTuKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0), QString("15301Y0013305"));
    }

    void testThomson() {
        QScanResult wifi("Thomson41518c", "");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(ThomsonKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),3);
        qSort(results);
        QCOMPARE(results.at(0), QString("1C2D56E083"));
        QCOMPARE(results.at(1), QString("69237B667A"));
        QCOMPARE(results.at(2), QString("8F524DED99"));
    }

    void testTplink() {
        QScanResult wifi("tplink","F8:D1:11:1E:28:A5");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(TplinkKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),1);
        QCOMPARE(results.at(0), QString("111E28A5"));
    }

    void testWifimediaR() {
        QScanResult wifi("wifimedia_R-1234", "00:26:5B:1E:28:A5");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen), typeid(WifimediaRKeygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE(results.size(),2);
        QCOMPARE(results.at(0),QString("00265b1e28a0"));
        QCOMPARE(results.at(1),QString("00265B1E28A0"));
    }

    void testWLAN6X() {
        QScanResult wifi("WLAN123456", "11:22:33:44:55:66");
        wifi.checkSupport(matcher);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        QVERIFY2(keygens->size() != 0 , "An algorithm was not detected");
        Keygen * keygen = keygens->at(0);
        QCOMPARE(typeid(*keygen),typeid(Wlan6Keygen) );
        QVector<QString> results = keygen->getResults();
        QCOMPARE( results.size(),10);
        QCOMPARE(results.at(0), QString("5630556304607"));
        QCOMPARE(results.at(1), QString("5730446305616"));
        QCOMPARE(results.at(2), QString("5430776306625"));
        QCOMPARE(results.at(3), QString("5530666307634"));
        QCOMPARE(results.at(4), QString("5230116300643"));
        QCOMPARE(results.at(5), QString("5330006301652"));
        QCOMPARE(results.at(6), QString("5030336302661"));
        QCOMPARE(results.at(7), QString("5130226303670"));
        QCOMPARE(results.at(8), QString("5E30DD630C68F"));
        QCOMPARE(results.at(9), QString("5F30CC630D69E"));
    }

};


QTEST_MAIN(AlgorithmsTest)
#include "AlgorithmsTest.moc"
