/*
 * HG824xKeygen.h
 *
 *  Created on: 5 de Ago de 2012
 *      Author: ruka
 */

#ifndef HG824xKeygen_H_
#define HG824xKeygen_H_
#include "Keygen.h"
class HG824xKeygen : public Keygen{
public:
    HG824xKeygen(QString ssid, QString mac);

private:
	QVector<QString> & getKeys() ;
};

#endif /* HG824xKeygen_H_ */
