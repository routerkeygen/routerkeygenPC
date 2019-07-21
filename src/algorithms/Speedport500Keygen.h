#ifndef SPEEDPORT500KEYGEN_H
#define SPEEDPORT500KEYGEN_H
#include "Keygen.h"

class Speedport500Keygen :public Keygen

{
public:
    Speedport500Keygen(QString ssid, QString mac, QString minus);
private:
    QVector<QString> & getKeys();
    QString minus;
};

#endif // SPEEDPORT500KEYGEN_H
