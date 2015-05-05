/*
 * AxtelKeygen.h
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#ifndef TPLINKKEYGEN_H_
#define TPLINKKEYGEN_H_

#include "Keygen.h"

class TplinkKeygen: public Keygen {
public:
    TplinkKeygen(QString ssid, QString mac);

private:
	QVector<QString> & getKeys() ;
};

#endif /* TPLINKKEYGEN_H_ */
