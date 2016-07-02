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

#ifndef BSSIDKEYGEN_H_
#define BSSIDKEYGEN_H_

#include "Keygen.h"

enum BssidKeygenFlags {
    FlagLen12 = 1u << 0,
    FlagLen10 = 1u << 1,
    FlagLen8  = 1u << 2,
    FlagUc    = 1u << 3,
    FlagLc    = 1u << 4
};

class BssidKeygen: public Keygen {
public:
    BssidKeygen(QString ssid, QString mac, int flags, int offset);

private:
	QVector<QString> & getKeys();
	unsigned int flags;
	int offset;
};

#endif /* BSSIDKEYGEN_H_ */
