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
#include "KeygenThread.h"
#include "algorithms/Keygen.h"

KeygenThread::KeygenThread( QVector<Keygen *>  * routers): routers(routers) , error(false){}

KeygenThread::~KeygenThread() {
}

void KeygenThread::run(){
    for ( int i = 0; i < routers->size(); ++i ){
        try{
            QVector<QString> r = routers->at(i)->getResults();
            Q_FOREACH (QString s, r) {
                results.append(s);
            }
        } catch (int e){
            error = true;
        }
    }
}

QVector<QString> KeygenThread::getResults() const {
	return results;
}

bool KeygenThread::hadError() const{
	return error;
}



