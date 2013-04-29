/**
   @file   wireless.h
   @brief  wireless tools


   @author Faizal Zakaria
   @date   2012-07-20
*/

#ifndef WIRELESS_H
#define WIRELESS_H

#include <QVariant>
#include <QStringList>

class Wireless
{
public:
	Wireless();
	~Wireless();
	int iw( QString dev, QString cmd, QString params );
	QStringList *getNetworkList();
	int scanNetworks( const QString dev );
	int joinOpenNetwork(const QString ssid);
	int udhcpc( QString dev, QString params );
	int joinWPANetwork(const QString ssid, const QString password);

private:
	QStringList *stringList;
	int killall( QString process );
	int closeNetwork();
	int wpa_passphrase( QString ssid, QString password );
	int wpa_supplicant( QString dev );
	int ifconfig( QString dev, QString cmd, QString param );

	
};

#endif // WIRELESS_H
