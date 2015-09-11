/*
 * Copyright 2012 Rui Araújo, Luís Fonseca
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
#ifndef SITECOMWLR400xKEYGEN_H
#define SITECOMWLR400xKEYGEN_H
#include "Keygen.h"

//http://blog.emaze.net/2014/04/sitecom-firmware-and-wifi.html
class SitecomWLR400xKeygen : public Keygen
{
    public:
        SitecomWLR400xKeygen(QString ssid, QString mac);
    private:
    	QVector<QString> & getKeys();
        void generateKey(QString mac, const QString charsets[]);
        const static QString CHARSETS_4000[];
        const static QString CHARSETS_4004[];
        const static long MAGIC1;
        const static long MAGIC2;
        const static long MAGIC3;
};

#endif // SITECOMWLR400xKEYGEN_H
