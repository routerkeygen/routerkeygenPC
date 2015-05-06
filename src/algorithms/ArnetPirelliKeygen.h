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
#ifndef ARNETPIRELLIKEYGEN_H
#define ARNETPIRELLIKEYGEN_H
#include "Keygen.h"

class ArnetPirelliKeygen : public Keygen
{
    public:
        ArnetPirelliKeygen(QString ssid, QString mac);
        int getSupportState() const;
    private:
    	QVector<QString> & getKeys();
        QString incrementMac(QString mac, int increment);
        void generateKey(QString mac);
        const static QString LOOKUP;
        const static QString SEED;
};

#endif // ARNETPIRELLIKEYGEN_H
