#include <typeinfo>
#include <QString>
#include <QVector>
#include <iostream>

#include "WirelessMatcher.h"
#include "wifi/QScanResult.h"
#include "algorithms/Keygen.h"

#include "qcmdlineparser/qcmdlineparser.h"

static void generateCode(WirelessMatcher &wm, QString &ssid, QString &mac)
{
    QScanResult wifi(ssid, mac);

    // Add keygens
    wifi.checkSupport(wm);

    // Calculate!
    QVector<Keygen *> * keygens = wifi.getKeygens();
    if (keygens == nullptr)
        return; /* Out of memory */

    // Print keys
    // Format is MAC<TAB>Password
    for (int i = 0; i < keygens->size(); ++i) {
        try {
            QVector<QString> r = keygens->at(i)->getResults();
            foreach (QString s, r) {
                std::cout<< mac.toUtf8().data() << '\t' << s.toLatin1().data() << std::endl;
            }
        } catch (int e) {
            continue;
        }
    }
}

static QString hexDecode(const QString &in)
{
    auto a = QByteArray::fromHex(in.toUtf8().data());

    return QString::fromStdString(a.toStdString());
}

static inline bool hexValid(const QString &in)
{
    if ((in.size() % 2) != 0) {
        return false;
    }

     foreach (QChar c, in) {
        if (!isxdigit(c.toLatin1())) {
            return false;
        }
    }

    return true;
}

int main(int argc, char * argv[]) {
    QCoreApplication app(argc, argv);
    QCmdLineParser parser;
    parser.addOption("-s", QCmdLineArgument::StoreValue, QObject::tr("SSID"), "--ssid", "network_name");
    parser.addOption("-m", QCmdLineArgument::StoreValue, QObject::tr("MAC address"), "--mac", "mac_address");
    parser.addOption("-k", QCmdLineArgument::StoreTrue, QObject::tr("Print keygen in form kgname:candidate"), "--kg");
    parser.addOption("-q", QCmdLineArgument::StoreTrue, QObject::tr("Print only calculated keys"), "--quiet");
    parser.addOption("-r", QCmdLineArgument::StoreTrue, QObject::tr("Read SSID and MAC from stdin in hex"), "--read");
    QString error = "";
    QVariantMap options = parser.parse(QCoreApplication::arguments(), &error);

    if (options.contains("r")) {
        enum {
            IDX_MAC  = 0,
            IDX_SSID = 1,
            IDX_MAX  = 2,
        };
        std::string line;

 	WirelessMatcher wm;

        // Read line by line
        // Format is: MAC<TAB>SSID
        while (!std::cin.eof()) {

            // Read the string
            std::getline(std::cin, line);

            // We are done if it is empty
            if (line == "") {
                break;
            }

            // Split by `TAB`
            QStringList pair = QString::fromStdString(line).split('\t');

            // We want only two elements
            if (pair.size() != IDX_MAX) {
                std::cerr<<"ignoring: "<<line<<std::endl;
                continue;
            }

            // Do we have valid HEX encoded SSID ?
            if (!hexValid(pair.at(IDX_SSID))) {
                std::cerr<<"ignoring: invalid SSID hex data: "<<line<<std::endl;
                continue;
            }

            // Hex decode
            QString ssid = hexDecode(pair.at(IDX_SSID));
            QString mac = pair.at(IDX_MAC).toUpper();

            // Check that we have valid input
            if (ssid == "" || mac == "") {
                std::cerr<<"ignoring: invalid input: "<<line<<std::endl;
                continue;
            }

            // Generate the code!
            generateCode(wm, ssid, mac);
        }
        return 0;
    }
    if ( !error.isEmpty() || !(options.contains("s") || options.contains("m")) ) {
        std::cout << error.toUtf8().data() << std::endl <<  parser.help().toUtf8().data();
        return -1;
    }
    if ( options.contains("s") || options.contains("m") ) {
        WirelessMatcher m;
        QString mac = options.value("m", "").toString().toUpper();
        if ( mac.length()>0 && mac.count(QRegExp("^([0-9A-F]{2}[:-]){5}([0-9A-F]{2})$")) == 0 ) {
            mac = "";
            if ( !options.value("q", false).toBool() )
                std::cout << QObject::tr("Invalid MAC. It will not be used.").toUtf8().data() << std::endl;
        }
        QString ssid = options.value("s", "").toString();
        QScanResult wifi(ssid, mac);
        wifi.checkSupport(m);
        QVector<Keygen *> * keygens = wifi.getKeygens();
        if (keygens == NULL ) {
            if ( !options.value("q", false).toBool() )
                std::cout << QObject::tr("Out of memory.").toUtf8().data() << std::endl;
            return -100;
        }
        if ( keygens->size() == 0) {
            if ( !options.value("q", false).toBool() )
                std::cout << QObject::tr("Unsupported network. Check the MAC address and the SSID.").toUtf8().data() << std::endl;
            return -2;
        }
        if ( !options.value("q", false).toBool() )
            std::cout << QObject::tr("Calculating keys. This can take a while.").toUtf8().data() << std::endl;
        QVector<QString> results;
        bool kg = options.value("k", false).toBool();
        for ( int i = 0; i < keygens->size(); ++i ){
            try {
                QVector<QString> r = keygens->at(i)->getResults();
                if (kg) {
                    foreach (QString s, r) {
                         results.append(keygens->at(i)->kgname + ":" + s);
                     }
                } else {
                    results +=r;
                }
            } catch (int e) {
                if ( !options.value("q", false).toBool() )
                    std::cout << QObject::tr("Errors while calculating.").toUtf8().data() << std::endl;
                continue;
            }
        }
        if (results.isEmpty()) {
            if ( !options.value("q", false).toBool() )
                std::cout << QObject::tr("No keys were calculated.").toUtf8().data() << std::endl;
            return -1;
        }
        if ( !options.value("q", false).toBool() )
            std::cout << QObject::tr("Calculated Passwords for %1").arg(wifi.getSsidName()).toUtf8().data() << std::endl;
        for (int i = 0; i < results.size(); ++i)
            std::cout <<  results.at(i).toLatin1().data() << std::endl;
    }
    return 0;
}
