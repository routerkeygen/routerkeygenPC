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
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QVector>
#include <iostream>
#include "RouterKeygen.h"
#include "wifi/QScanResult.h"
#include "WirelessMatcher.h"
#include "algorithms/Keygen.h"
#include "qcmdlineparser/qcmdlineparser.h"
#include "version.h"

int main(int argc, char * argv[]) {
    /* Run main application */
    QApplication app(argc, argv);
    QTranslator translator;
    QApplication::setOrganizationName("com.doublecheck");
    QApplication::setApplicationName(PROJECT_NAME);
    QApplication::setApplicationVersion(PROJECT_VERSION);
    QString qmFile = app.applicationName().toLower() + "_" + QLocale::system().name();
    if ( translator.load(qmFile,":/lang") )
        app.installTranslator(&translator);

    RouterKeygen w;
    w.showWithDialog();
    return app.exec();
}
