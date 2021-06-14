/*
 * QWifiManager.h
 *
 *  Created on: 3 de Ago de 2012
 *      Author: ruka
 */

#ifndef QWIFIMANAGER_H_
#define QWIFIMANAGER_H_
#include <QObject>
#include <QVector>
#include <QProcess>
#include <QSharedPointer>

class QWifiManagerPrivate;
class QScanResult;
class QWifiManager: public QObject {
Q_OBJECT
public:
	QWifiManager() ;
	virtual ~QWifiManager();
	void startScan();
    QVector<QSharedPointer<QScanResult> > & getScanResults();
	enum SCAN_RESULT_STATE{
		SCAN_OK = 0,
		ERROR_NO_NM,
		ERROR_NO_WIFI,
		ERROR_NO_WIFI_ENABLED,
		ERROR
	};


	// Constants used for different security types
	const static QString PSK;
	const static QString WEP;
	const static QString EAP;
	const static QString OPEN;
	void setForceScan(bool);
Q_SIGNALS:
	void scanFinished(int);
private Q_SLOTS:
	void implScanFinished(int);
private:
	bool forceRefresh;
	QProcess * scan;
	QWifiManagerPrivate * impl;
};

#endif /* QWIFIMANAGER_H_ */
