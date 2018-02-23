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
#include "RouterKeygen.h"
#include "ui_routerkeygen.h"
#include <QMessageBox>
#include "mac/macloginitemsmanager.h"
#include <QCompleter>
#include <QStringList>
#include <QMovie>
#include <QListWidgetItem>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QIcon>
#include <QClipboard>
#include <QWidgetAction>
#include <QDesktopServices>
#include <QDateTime>
#include <QFile>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTextStream>
#include <QtScript/QScriptEngine>
#include <QHeaderView>
#include <QScopedPointer>
#include <stdlib.h>
#include "version.h"
#include "algorithms/Keygen.h"
#include "WirelessMatcher.h"
#include "KeygenThread.h"
#include "dialog/AboutDialog.h"
#include "dialog/WelcomeDialog.h"
#include "dialog/UpdateDialog.h"
#include "wifi/QWifiManager.h"
#include "wifi/QScanResult.h"

RouterKeygen::RouterKeygen(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::RouterKeygen), manualWifi(NULL),matcher(new WirelessMatcher()),
    calculator(NULL), loading(NULL), loadingText(NULL), aboutDialog(NULL), welcomeDialog(NULL){
    ui->setupUi(this);
#if !defined(Q_OS_WIN) && !defined(Q_OS_MAC)
    setWindowIcon(QIcon(":/big_icon.png"));
#endif
    connect(ui->calculateButton, SIGNAL( clicked() ), this,
            SLOT( manualCalculation() ));
    connect(ui->refreshScan, SIGNAL( clicked() ), this,
            SLOT( refreshNetworks() ));
    connect(ui->supportedNetworkslist, SIGNAL( cellClicked(int,int) ), this,
            SLOT( supportedNetworkRowSelected(int,int) ));
    connect(ui->unlikelyNetworkslist, SIGNAL( cellClicked(int,int) ), this,
            SLOT( unlikelyNetworkRowSelected(int,int) ));
    connect(ui->unsupportedNetworkslist, SIGNAL( cellClicked(int,int) ), this,
            SLOT( unsupportedNetworkRowSelected(int,int) ));
    connect(ui->copyAllButton, SIGNAL( clicked() ), this,
            SLOT( copyAll() ));

    connect(ui->actionDonate,SIGNAL(triggered()), this, SLOT(donatePaypal()));
    connect(ui->actionDonate_Google_Play, SIGNAL(triggered()),this, SLOT(donateGooglePlay()) );
    connect(ui->actionFeedback, SIGNAL(triggered()), this,SLOT(feedback()));
    connect(ui->actionAbout, SIGNAL(triggered()), this,SLOT(showAboutDialog()) );
    connect(ui->actionCheck_for_Updates, SIGNAL(triggered()), this,SLOT(checkUpdates()));
    ui->actionCheck_for_Updates->setEnabled(false); //It is enabled after the automatic update check
#if defined(Q_OS_MAC)


#endif
#if !defined(Q_OS_WIN) && !defined(Q_OS_MAC)
    connect(ui->forceRefresh, SIGNAL( stateChanged(int) ), this,
            SLOT( forceRefreshToggle(int) ));
#else
    ui->forceRefresh->setVisible(false); // it is not needed in Windows or Mac
#endif
    mNetworkManager = new QNetworkAccessManager(this);
   connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReply(QNetworkReply*)));

    wifiManager = new QWifiManager();
    connect(wifiManager, SIGNAL( scanFinished(int) ), this,
            SLOT( scanFinished(int) ));
    loadingAnim = new QMovie(":/loading.gif");
    loadingAnim->setParent(this);
    //Auto-Complete!
    wordList << "Alice-" <<  "Arcor-" << "AXTEL-" << "AXTEL-XTREMO-" << "Bbox-" << "Belkin" <<
            "BigPond" << "Blink" << "Cabovisao-" << "CONN" << "CYTA" << "Discus--"<<
            "DLink-" << "DMAX" << "EasyBox-" << "eircom" << "FASTWEB-1-" << "INFINITUM" <<
            "InfostradaWiFi-" << "InterCable" << "JAZZTEL_" << "MAXCOM" << "Megared" <<
            "MEO-" << "O2Wireless" << "Optimus" << "OptimusFibra" << "Orange-" << "OTE" <<
            "Otenet" << "PBS" << "privat" << "ptv" << "SKY" << "SpeedTouch" << "TECOM-AH4222-" <<
            "TECOM-AH4021-" << "TeleTu" << "Thomson" << "TN_private_" << "Vodafone-" << "WiFi" <<
            "WiFi-Arnet-" << "wifimedia_R-" << "WLAN_" << "WLAN" << "YaCom";
    completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui->ssidInput->setCompleter(completer);
    ui->passwordsList->installEventFilter(this);

    // build menu
    trayMenu = new QMenu(this);
    trayIcon = new QSystemTrayIcon(this);
    // set up and show the system tray icon
    trayIcon->setIcon(QIcon(":/tray_icon.png"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    
    //Set widget ration
    ui->splitterVertical->setStretchFactor(0, 3);
    ui->splitterVertical->setStretchFactor(1, 2);
    
    settings = new QSettings("Exobel", "RouterKeygen");
    bool forceRefresh = settings->value(FORCE_REFRESH, false).toBool();
    wifiManager->setForceScan(forceRefresh);
    ui->forceRefresh->setChecked(forceRefresh);
    runInBackground = settings->value(RUN_IN_BACKGROUND, false).toBool();
    runOnStartUp = settings->value(RUN_ON_START_UP, false).toBool();
    qApp->setQuitOnLastWindowClosed(!runInBackground);

    startUpAction = ui->menuPreferences->addAction(tr("Run on Start up"));
    startUpAction->setCheckable(true);
    startUpAction->setChecked(runOnStartUp);
    connect(startUpAction, SIGNAL(toggled(bool)), this,
            SLOT(startUpRunToggle(bool)));

    runInBackgroundAction = ui->menuPreferences->addAction(tr("Run in the background"));
    runInBackgroundAction->setCheckable(true);
    runInBackgroundAction->setChecked(runInBackground);
    connect(runInBackgroundAction, SIGNAL(toggled(bool)), this,
            SLOT(backgroundRunToggle(bool)));

    //Set up the tray icon
    connect(trayMenu->addAction(tr("Open")),
            SIGNAL(triggered()), this, SLOT(show()));
    trayMenu->addSeparator();
    connect(trayMenu->addAction(tr("Vulnerable networks")),
            SIGNAL(triggered()), this, SLOT(show()));
    trayMenu->addAction(tr("None were detected"))->setEnabled(false);
    trayMenu->addSeparator();
    trayMenu->addAction(startUpAction);
    trayMenu->addAction(runInBackgroundAction);
    trayMenu->addSeparator();
    connect(trayMenu->addAction(tr("Quit")), SIGNAL(triggered()), qApp, SLOT(quit()));

    //Do an initial scan
    wifiManager->startScan();

    //Check for application updates, it fails silently
    automaticUpdateCheck = true;
    checkUpdates();
}

void RouterKeygen::showWithDialog(){
    show();
    if ( ( QDateTime::currentDateTime().toTime_t()- settings->value(WELCOME_DIALOG, 0).toUInt()) > SECONDS_IN_WEEK ){
        settings->setValue(WELCOME_DIALOG, QDateTime::currentDateTime().toTime_t() );
        if ( welcomeDialog == NULL )
            welcomeDialog = new WelcomeDialog(this);
        welcomeDialog->show();
    }
}

void RouterKeygen::showAboutDialog(){
    if ( aboutDialog == NULL )
        aboutDialog = new AboutDialog(this);
    aboutDialog->show();
}

void  RouterKeygen::checkUpdates(){
    if ( !automaticUpdateCheck ){
        enableUI(false);
        setLoadingAnimation(tr("Checking for updates"));
    }
    mNetworkManager->get(QNetworkRequest(RouterKeygen::UPDATE_URL));
}


void RouterKeygen::onNetworkReply(QNetworkReply* reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        /*
         * Reply is finished!
         * We'll ask for the reply about the Redirection attribute
         * http://doc.trolltech.com/qnetworkrequest.html#Attribute-enum
         */
        QVariant possibleRedirectUrl =
                 reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

        /* We'll deduct if the redirection is valid in the redirectUrl function */
        _urlRedirectedTo = this->redirectUrl(possibleRedirectUrl.toUrl(),
                                             _urlRedirectedTo);

        /* If the URL is not empty, we're being redirected. */
        if(!_urlRedirectedTo.isEmpty()) {
            /* We'll do another request to the redirection url. */
            mNetworkManager->get(QNetworkRequest(_urlRedirectedTo));
        }
        else {
            if ( !automaticUpdateCheck ){
                cleanLoadingAnimation();
                enableUI(true);
            }
            /*
             * We weren't redirected anymore
             * so we arrived to the final destination...
             */
            /* ...so this can be cleared. */
            _urlRedirectedTo.clear();
            if (reply->isReadable()){
                QByteArray result = reply->readAll();
                QScriptValue sc;
                QScriptEngine engine;
                sc = engine.evaluate("(" +QString(result) + ")");
                QString version = sc.property("version").toString();
                if ( version == QApplication::applicationVersion() ){
                    if ( !automaticUpdateCheck ){
                        ui->statusBar->showMessage(tr("The application is already at the latest version."));
                    }
                }
                else{
                    //TODO: when the final website is sc.property("url").toString()
                    UpdateDialog* updateDialog = new UpdateDialog(QString("https://routerkeygen.github.io/"),
                                                                   version, this);
                    updateDialog->show();
                }
            }
            else{
                if ( !automaticUpdateCheck ){
                    ui->statusBar->showMessage(tr("Error while checking for updates"));
                }
            }
            automaticUpdateCheck = false;
        }
    }
    else {
        if ( !automaticUpdateCheck ){
            ui->statusBar->showMessage(tr("Error while checking for updates"));
            cleanLoadingAnimation();
            enableUI(true);
        }
        automaticUpdateCheck = false;
    }
    reply->deleteLater();
}

QUrl RouterKeygen::redirectUrl(const QUrl& possibleRedirectUrl,
                               const QUrl& oldRedirectUrl) const {
    QUrl redirectUrl;
    /*
     * Check if the URL is empty and
     * that we aren't being fooled into a infinite redirect loop.
     * We could also keep track of how many redirects we have been to
     * and set a limit to it, but we'll leave that to you.
     */
    if(!possibleRedirectUrl.isEmpty() &&
       possibleRedirectUrl != oldRedirectUrl) {
        redirectUrl = possibleRedirectUrl;
    }
    return redirectUrl;
}

void RouterKeygen::donateGooglePlay(){
    QDesktopServices::openUrl(QUrl("https://play.google.com/store/apps/details?id=org.doublecheck.wifiscanner"));
}

void  RouterKeygen::donatePaypal(){
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=V3FFBTRTTV5DN"));
}


void  RouterKeygen::feedback(){
    QDesktopServices::openUrl(QUrl("mailto:"+ QString(PROJECT_CONTACT) +"?subject=RouterKeygen Feedback "+ QString(SYSNAME) +"&body=I love you! ;)"));
}


RouterKeygen::~RouterKeygen() {
    delete ui;
    delete loadingAnim;
    delete wifiManager;
    if (calculator != NULL) {
        if (calculator->isRunning()) {//TODO:stop router
            calculator->wait();
        }
        delete calculator;
    }
    if ( manualWifi != NULL )
        delete manualWifi;
    delete settings;
    trayMenu->clear();
    delete trayMenu;
    delete trayIcon;
    delete matcher;
    delete aboutDialog;
    delete welcomeDialog;
    mNetworkManager->deleteLater();
}
void RouterKeygen::manualCalculation() {
    QString mac = ui->macInput->text();
    if ( mac.length()>0 && mac.count(QRegExp("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$")) == 0 ) {
        mac = "";
        ui->statusBar->showMessage(tr("Invalid MAC. It will not be used."));
    }
    if (ui->ssidInput->text().trimmed() == "" && mac == "")
        return;
    if ( manualWifi != NULL )
        delete manualWifi;
    manualWifi = new QScanResult(ui->ssidInput->text().trimmed(), mac.toUpper());
    manualWifi->checkSupport(*matcher);
    calc(manualWifi);
}

void RouterKeygen::copyAll() {
    int cnt;
    QClipboard *clipboard = QApplication::clipboard();
    QString passwords;
    for(cnt = 0; cnt < ui->passwordsList->count(); cnt++)
      passwords += ui->passwordsList->item(cnt)->text() + QString("\n");
    clipboard->setText(passwords, QClipboard::Clipboard);
    ui->statusBar->showMessage(tr("%1 passwords copied").arg(cnt));
}

void RouterKeygen::calc(QScanResult * wifi) {
    if (calculator != NULL) {
        return; //ignore while a calculator is still running
    }
    if (wifi->getSupportState() == Keygen::UNSUPPORTED) {
        ui->statusBar->showMessage(
                tr("Unsupported network. Check the MAC address and the SSID."));
        return;
    }
    ui->passwordsList->clear();
    setLoadingAnimation(tr("Calculating keys. This can take a while."));
    this->calculator = new KeygenThread(wifi->getKeygens());
    connect(this->calculator, SIGNAL( finished() ), this, SLOT( getResults() ));
    enableUI(false);
    this->calculator->start();
}

void RouterKeygen::supportedNetworkRowSelected(int row, int) {
    QString selectedSSID = ui->supportedNetworkslist->item(row, 0)->text();
    QString selectedMac = ui->supportedNetworkslist->item(row, 1)->text();
    for ( int i  = 0; i < wifiNetworks.size(); ++i ){
        if ( wifiNetworks.at(i)->getSsidName() == selectedSSID &&  wifiNetworks.at(i)->getMacAddress() == selectedMac){
            calc(wifiNetworks.at(i).data());
            return;
        }
    }
}


void RouterKeygen::unlikelyNetworkRowSelected(int row, int) {
    QString selectedSSID = ui->unlikelyNetworkslist->item(row, 0)->text();
    QString selectedMac = ui->unlikelyNetworkslist->item(row, 1)->text();
    for ( int i  = 0; i < wifiNetworks.size(); ++i ){
        if ( wifiNetworks.at(i)->getSsidName() == selectedSSID &&  wifiNetworks.at(i)->getMacAddress() == selectedMac){
            calc(wifiNetworks.at(i).data());
            return;
        }
    }
}

void RouterKeygen::unsupportedNetworkRowSelected(int, int) {
    ui->passwordsList->clear();
    ui->statusBar->showMessage(
            tr("Unsupported network."));
}

void RouterKeygen::refreshNetworks() {
    enableUI(false);
    setLoadingAnimation(tr("Scanning the network"));
    wifiManager->startScan();
}

void RouterKeygen::scanFinished(int code) {
    cleanLoadingAnimation();
    enableUI(true);
    switch (code) {
    case QWifiManager::SCAN_OK: {
            foreach ( QSharedPointer<QScanResult> scanResult, wifiNetworks )
                scanResult.clear();
            bool setTabPosition = false;
            if ( wifiNetworks.size() == 0 )
                setTabPosition = true;
            wifiNetworks = wifiManager->getScanResults();
            //Stting the row count to the maximum possible value
            ui->unsupportedNetworkslist->setRowCount(wifiNetworks.size());
            ui->supportedNetworkslist->setRowCount(wifiNetworks.size());
            ui->unlikelyNetworkslist->setRowCount(wifiNetworks.size());
            trayMenu->clear();
            connect(trayMenu->addAction(tr("Open")),
                    SIGNAL(triggered()), this, SLOT(show()));
            trayMenu->addSeparator();
            connect(trayMenu->addAction(tr("Vulnerable networks")),
                    SIGNAL(triggered()), this, SLOT(show()));
            bool foundVulnerable = false;
            int unsupportedPos = 0, supportedPos = 0, unlikelyPos= 0;
           for (int i = 0; i < wifiNetworks.size(); ++i) {
                wifiNetworks.at(i)->checkSupport(*matcher);
                if ( wifiNetworks.at(i)->getSupportState() == Keygen::UNSUPPORTED ){
                    ui->unsupportedNetworkslist->setItem(unsupportedPos, 0,
                                              new QTableWidgetItem(wifiNetworks.at(i)->getSsidName()));
                    ui->unsupportedNetworkslist->setItem(unsupportedPos, 1,
                                              new QTableWidgetItem(wifiNetworks.at(i)->getMacAddress()));
                    QString level;
                    level.setNum(wifiNetworks.at(i)->getLevel(), 10);
                    ui->unsupportedNetworkslist->setItem(unsupportedPos, 2, new QTableWidgetItem(level));
                    ++unsupportedPos;
                }
                else{
                    if ( wifiNetworks.at(i)->getSupportState() == Keygen::SUPPORTED ){
                        ui->supportedNetworkslist->setItem(supportedPos, 0,
                                                   new QTableWidgetItem(wifiNetworks.at(i)->getSsidName()));
                         ui->supportedNetworkslist->setItem(supportedPos, 1,
                                                   new QTableWidgetItem(wifiNetworks.at(i)->getMacAddress()));
                         QString level;
                         level.setNum(wifiNetworks.at(i)->getLevel(), 10);
                         ui->supportedNetworkslist->setItem(supportedPos, 2, new QTableWidgetItem(level));
                         ++supportedPos;
                    }
                    else {//if ( networks.at(i)->getSupportState() == Keygen::MAYBE )
                         ui->unlikelyNetworkslist->setItem(unlikelyPos, 0,
                                                   new QTableWidgetItem(wifiNetworks.at(i)->getSsidName()));
                         ui->unlikelyNetworkslist->setItem(unlikelyPos, 1,
                                                   new QTableWidgetItem(wifiNetworks.at(i)->getMacAddress()));
                         QString level;
                         level.setNum(wifiNetworks.at(i)->getLevel(), 10);
                         ui->unlikelyNetworkslist->setItem(unlikelyPos, 2, new QTableWidgetItem(level));
                         ++unlikelyPos;
                    }
                    addNetworkToTray(wifiNetworks.at(i)->getSsidName(), wifiNetworks.at(i)->getLevel(), wifiNetworks.at(i)->isLocked());
                    foundVulnerable = true;
                }
            }
            //Stting the row count to the correct value
            ui->unsupportedNetworkslist->setRowCount(unsupportedPos);
            ui->supportedNetworkslist->setRowCount(supportedPos);
            ui->unlikelyNetworkslist->setRowCount(unlikelyPos);
            if ( setTabPosition ){
                if ( supportedPos > 0 )
                    ui->networkSplitter->setCurrentIndex(0);
                else if ( unlikelyPos > 0 )
                    ui->networkSplitter->setCurrentIndex(1);
                else
                    ui->networkSplitter->setCurrentIndex(2);
            }
            if (!foundVulnerable) {
                trayMenu->addAction(tr("None were detected"))->setEnabled(false);
            }

            trayMenu->addSeparator();
            trayMenu->addAction(startUpAction);
            trayMenu->addAction(runInBackgroundAction);
            trayMenu->addSeparator();
            connect(trayMenu->addAction(tr("Quit")), SIGNAL(triggered()), qApp, SLOT(quit()));
            if (wifiNetworks.size() == 0) {
                 ui->statusBar->showMessage(tr("None were detected"));
            }
            QStringList headers;
            headers << tr("Name") << tr("MAC") << tr("Strength");
            ui->supportedNetworkslist->setHorizontalHeaderLabels(headers);
            ui->supportedNetworkslist->resizeColumnsToContents();
            ui->supportedNetworkslist->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
            ui->supportedNetworkslist->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
            ui->supportedNetworkslist->sortByColumn(2); //Order by Strength
            ui->unsupportedNetworkslist->setHorizontalHeaderLabels(headers);
            ui->unsupportedNetworkslist->resizeColumnsToContents();
            ui->unsupportedNetworkslist->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
            ui->unsupportedNetworkslist->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
            ui->unsupportedNetworkslist->sortByColumn(2); //Order by Strength
            ui->unlikelyNetworkslist->setHorizontalHeaderLabels(headers);
            ui->unlikelyNetworkslist->resizeColumnsToContents();
            ui->unlikelyNetworkslist->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
            ui->unlikelyNetworkslist->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
            ui->unlikelyNetworkslist->sortByColumn(2); //Order by Strength
            break;
	}
    case QWifiManager::ERROR_NO_NM:
        ui->statusBar->showMessage(tr("Network Manager was not detected"));
        break;

    case QWifiManager::ERROR_NO_WIFI:
        ui->statusBar->showMessage(tr("No Wifi device detected"));
        break;

    case QWifiManager::ERROR_NO_WIFI_ENABLED:
        ui->statusBar->showMessage(tr("The wifi device is not enabled"));
        break;
    }

}

void RouterKeygen::addNetworkToTray(const QString & ssid, int level, bool locked ) {
    QIcon icon;
    if (level >= 75)
        icon = locked?QIcon::fromTheme("nm-signal-100-secure"):QIcon::fromTheme("nm-signal-100");
    else if (level >= 50)
        icon = locked?QIcon::fromTheme("nm-signal-75-secure"):QIcon::fromTheme("nm-signal-75");
    else if (level >= 25)
        icon = locked?QIcon::fromTheme("nm-signal-50-secure"):QIcon::fromTheme("nm-signal-50");
    else
        icon = locked?QIcon::fromTheme("nm-signal-25-secure"):QIcon::fromTheme("nm-signal-25");
    QAction * net = trayMenu->addAction(icon, ssid);
    connect(net, SIGNAL(triggered()), this, SLOT(show()));
}

void RouterKeygen::getResults() {
    cleanLoadingAnimation();
    enableUI(true);
    if (calculator->hadError()) {
        ui->statusBar->showMessage(tr("Error while calculating."));
        delete calculator;
        calculator = NULL;
        return;
    }
    listKeys = this->calculator->getResults();
    if (listKeys.isEmpty()) {
        ui->statusBar->showMessage(tr("No keys were calculated."));
        delete calculator;
        calculator = NULL;
        return;
    }
    ui->passwordsList->clear();
    for (int i = 0; i < listKeys.size(); ++i)
        ui->passwordsList->insertItem(0, listKeys.at(i));
    ui->statusBar->showMessage(tr("Calculation finished"));
    delete calculator;
    calculator = NULL;
}

bool RouterKeygen::eventFilter(QObject *obj, QEvent *event) {
    // you may handle multiple objects checking "obj" parameter
    if (event->type() == QEvent::ContextMenu) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (obj == ui->passwordsList) {
            rightButtonClicked(obj, mouseEvent->globalPos());
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void RouterKeygen::rightButtonClicked(QObject *obj, const QPoint &pos) {
    if (obj == ui->passwordsList) {
        if (ui->passwordsList->count() == 0)
            return;
        if (ui->passwordsList->selectedItems().size() == 0)
            return;
        QMenu *menu = new QMenu();
        QAction * copy = new QAction(tr("Copy"), this);
        copy->setShortcut(QKeySequence::Copy);
        copy->setStatusTip(tr("Copy this key"));
        connect(copy, SIGNAL(triggered()), this, SLOT(copyKey()));
        menu->addAction(copy);
        menu->exec(pos);
    }
}

void RouterKeygen::copyKey() {
    QList<QListWidgetItem*> selectedItems = ui->passwordsList->selectedItems();
    if (selectedItems.size() == 0)
        return;
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(selectedItems.at(0)->text(), QClipboard::Clipboard);
    ui->statusBar->showMessage(tr("%1 copied").arg(selectedItems.at(0)->text()));
}

void RouterKeygen::forceRefreshToggle(int state) {
    wifiManager->setForceScan(state == Qt::Checked);
    settings->setValue(FORCE_REFRESH, state == Qt::Checked);
}

void RouterKeygen::backgroundRunToggle(bool state) {
    runInBackground = state;
    qApp->setQuitOnLastWindowClosed(!state);
    settings->setValue(RUN_IN_BACKGROUND, runInBackground);
}

void RouterKeygen::startUpRunToggle(bool state) {
    runOnStartUp = state;
    settings->setValue(RUN_ON_START_UP, runOnStartUp);
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
    MacLoginItemsManager loginManager;
    if ( runOnStartUp ){
        if ( !loginManager.containsRunningApplication() ){
           loginManager.appendRunningApplication();
        }
    }
    else{
        if ( loginManager.containsRunningApplication() ){
            loginManager.removeRunningApplication();
        }
    }
#else
    QString newFile = "/home/" + QString(getenv("USER"))
                      + "/.config/autostart/routerkeygen.desktop";
    if (runOnStartUp) {
        QFile file(newFile);
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << "[Desktop Entry]" << endl;
            stream << "Type=Application" << endl;
            stream << "Terminal=false" << endl;
            stream << "Exec=" << QCoreApplication::applicationFilePath() << " --no-gui" << endl;
            stream << "Name=" <<  QCoreApplication::applicationName() << endl;
            stream.flush();
            QFile::setPermissions(newFile,
                                  QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser
                                  | QFile::WriteUser | QFile::ReadGroup | QFile::ReadGroup
                                  | QFile::ReadOther);

        }
    } else {
        if (QFile::exists(newFile))
            QFile::remove(newFile);
    }
#endif
#endif
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if (runOnStartUp) {
        settings.setValue("RouterKeygen", QCoreApplication::applicationFilePath().replace('/','\\')+ " --no-gui");
    } else {
        settings.remove("RouterKeygen");
    }
#endif
}

void RouterKeygen::setLoadingAnimation(const QString& text) {
    loadingAnim->start();
    loading = new QLabel(ui->statusBar);
    loading->setMovie(loadingAnim);
    loadingText = new QLabel(text, ui->statusBar);
    ui->statusBar->clearMessage();
    ui->statusBar->addWidget(loading);
    ui->statusBar->addWidget(loadingText);
}
void RouterKeygen::cleanLoadingAnimation() {
    if (loading == NULL || loadingText == NULL)
        return;
    loadingAnim->stop();
    ui->statusBar->removeWidget(loading);
    ui->statusBar->removeWidget(loadingText);
    loading = NULL;
    loadingText = NULL;
}


void RouterKeygen::enableUI(bool enable){
    ui->actionCheck_for_Updates->setEnabled(enable);
    ui->refreshScan->setEnabled(enable);
    ui->calculateButton->setEnabled(enable);
    ui->supportedNetworkslist->setEnabled(enable);
    ui->unlikelyNetworkslist->setEnabled(enable);
    ui->unsupportedNetworkslist->setEnabled(enable);
}
//https://raw.githubusercontent.com/routerkeygen/routerkeygenPC/master/routerkeygen_version.json
const QUrl RouterKeygen::UPDATE_URL = QUrl("https://raw.github.com/routerkeygen/routerkeygenPC/master/routerkeygen_version.json");
const QString RouterKeygen::RUN_ON_START_UP = "RUN_ON_START_UP";
const QString RouterKeygen::RUN_IN_BACKGROUND = "RUN_IN_BACKGROUND";
const QString RouterKeygen::FORCE_REFRESH = "FORCE_REFRESH";
const QString RouterKeygen::WELCOME_DIALOG = "WELCOME_DIALOG_TIME";
const unsigned int RouterKeygen::SECONDS_IN_WEEK = 7 * 24 * 3600;
