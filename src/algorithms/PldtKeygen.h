/*
 * Copyright 2016 Alex Stanev <alex@stanev.org>
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

#ifndef PLDTKEYGEN_H_
#define PLDTKEYGEN_H_

#include "Keygen.h"

class PldtKeygen: public Keygen {
public:
    PldtKeygen(QString ssid, QString mac, unsigned short int var);

private:
    QVector<QString> & getKeys();
    unsigned short int var;
};

#endif /* PLDTKEYGEN_H_ */
