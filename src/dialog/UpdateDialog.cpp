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
#include "UpdateDialog.h"
#include "ui_UpdateDialog.h"
#include <QDesktopServices>
#include <QUrl>

UpdateDialog::UpdateDialog(QString url, QString version, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog), url(url)
{
    ui->setupUi(this);
    connect(ui->openWebsite,SIGNAL(clicked()), this, SLOT(openWebsite()));
    connect(ui->close,SIGNAL(clicked()), this, SLOT(closeWindow()));
    ui->textBrowser->setHtml(ui->textBrowser->toHtml().arg(version));
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void  UpdateDialog::openWebsite(){
    QDesktopServices::openUrl(QUrl(url));
    this->closeWindow();
}


void  UpdateDialog::closeWindow(){
    close();
    this->deleteLater();
}
