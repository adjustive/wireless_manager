/**
   @file   wireless.cpp
   @brief  wireless tools


   @author Faizal Zakaria
   @date   2012-07-20
*/

/* TODO - 
 * add wireless_node 
 * password should be mesh for security
*/

#include "wireless.h"
#include <stdio.h>
#include <stdlib.h>
#include <QSet>
#include <QList>

#define WPA_CONF_FILE "/etc/wpa_supplicant.conf"
#define WLAN_DEV "wlan0"

Wireless::Wireless()
{
	stringList = new QStringList();
}

Wireless::~Wireless()
{
	delete stringList;
}

int Wireless::killall( QString process )
{
	char buf[256];
	int len;
	/* TODO, should not killall -9, try use wpa_cli in the future */
	len = snprintf(buf, sizeof(buf) - 1, "killall -9 %s 2> /dev/null", 
	               process.toAscii().data());
	buf[len] = '\0';

	printf("Wireless Command: \"%s\"\n", buf);
	return system( buf );
}

int Wireless::ifconfig( QString dev, QString cmd, QString param )
{
	char buf[256];
	int len;
	len = snprintf(buf, sizeof(buf) - 1, "ifconfig %s %s %s 2> /dev/null", 
	               dev.toAscii().data(), cmd.toAscii().data(), param.toAscii().data());
	buf[len] = '\0';

	printf("Wireless Command: \"%s\"\n", buf);
	return system( buf );
}

int Wireless::iw( QString dev, QString cmd, QString param )
{
	char buf[256];
	int len;
	len = snprintf(buf, sizeof(buf) - 1, "iw dev %s %s %s 2> /dev/null", 
	               dev.toAscii().data(), cmd.toAscii().data(), param.toAscii().data());
	buf[len] = '\0';

	printf("Wireless Command: \"%s\"\n", buf);
	return system( buf );
}

int Wireless::udhcpc( QString dev, QString params )
{
	char buf[256];
	int len = 0;
	len = snprintf( buf, sizeof(buf) - 1, "udhcpc -i %s %s 2> /dev/null", dev.toAscii().data(), params.toAscii().data() );
	buf[len] = '\0';

	printf("Wireless Command: \"%s\"\n", buf);
	return system( buf );
}

int Wireless::wpa_passphrase( QString ssid, QString password )
{
	char buf[256];
	int len = 0;
	len = snprintf( buf, sizeof(buf) - 1, "wpa_passphrase %s %s > %s", ssid.toAscii().data(), password.toAscii().data(), WPA_CONF_FILE );
	buf[len] = '\0';

	printf("Wireless Command: \"%s\"\n", buf);
	return system( buf );
}

int Wireless::wpa_supplicant( QString dev )
{
	char buf[256];
	int len = 0;
	len = snprintf( buf, sizeof(buf) - 1, "wpa_supplicant -Dnl80211 -i%s -c %s &", dev.toAscii().data(), WPA_CONF_FILE );
	buf[len] = '\0';

	printf("Wireless Command: \"%s\"\n", buf);
	return system( buf );
}

int Wireless::scanNetworks( const QString dev )
{
	static char buf[409600];
	static char parseData[4096];
	FILE* fp = NULL;
	int len = 0;
	int status = 0;
	
	status = ifconfig( WLAN_DEV, "up", "" );

	stringList->clear();

	len = snprintf(buf, sizeof(buf) - 1, "iw dev %s scan 2> /dev/null", dev.toAscii().data());

	buf[len] = '\0';

	printf("Wireless Command: \"%s\"\n", buf);

	fp = popen(buf, "r");

	if( !fp )
	{
		perror("File to Open:");
		return -1;
	}

	while ( fgets( parseData, 4096, fp ) != NULL )
	{
		/* SSID */
		char *pch;
		if ((pch = strstr( parseData, "SSID: " )) != NULL)
		{
			char *ssid = strtok( pch, ":" );
			ssid = strtok( NULL, ":" );
			ssid[strlen(ssid) - 1] = '\0';
			while( isspace(*ssid) ) ssid++;
			if ( strcmp( "", ssid) )
			{
				stringList->append( ssid );
				printf("Read2 : ++%s++\n", ssid );
			}
		}
		
		/* WPA */
        /*
		char *wpaTmp = strstr( parseData, "WPA: " );
		if (wpaTmp != NULL)
		{
			wpa = strtok( wpaTmp, ":");
			wpa = strtok( NULL, ":");
			while( isspace(*wpa) ) wpa++;
			printf("ReadWPA : ++%s++\n", wpa );
		}
		*/
	}

	//stringList->removeDuplicate();
	for (int i = 0; i< stringList->count(); ++i)
	{
		forever { // remove all occurrences at greater indexes
			int p = stringList->lastIndexOf( stringList->at(i) );
			if (p == i) break;
			stringList->removeAt(p);
		}
	}

	buf[len] = '\0';

	printf("Read %d bytes:\n", len);

	printf("%s", buf);

	len = pclose( fp );

	printf("Exit Status: %d\n", len);

	if( 0 != len )
	{
		return -1;
	}

	return status;
}

/* temp function, only for open network */
int Wireless::joinOpenNetwork(const QString ssid)
{
	int status = 0;

	closeNetwork();
	sleep(1);

	// it has to be "ssid"
	QString ssidNew = "\"" + ssid + "\"";
	status = iw( WLAN_DEV, "connect", ssidNew );
	printf( "status : %d\n", status);
	if ( status )
		return -1;

	status = udhcpc( WLAN_DEV, "-n -q" );
	printf( "status : %d\n", status);
	if ( status )
		return -1;

	printf( "status : %d\n", status);
	return status;
}

/* temp function, only for open network */
int Wireless::joinWPANetwork(const QString ssid, const QString password)
{
	int status = 0;

	closeNetwork();
	sleep(1);

	// it has to be "ssid"
	QString ssidNew = "\"" + ssid + "\"";
	status = iw( WLAN_DEV, "connect", ssidNew );
	printf( "status : %d\n", status);
	if ( status )
		return -1;

	status = wpa_passphrase( ssid, password );
	if ( status )
		return -1;

	status = wpa_supplicant( WLAN_DEV );
	if ( status )
		return -1;

	status = udhcpc( WLAN_DEV, "-n -q" );
	printf( "status : %d\n", status);
	if ( status )
		return -1;

	printf( "status : %d\n", status);
	return status;
}

int Wireless::closeNetwork()
{
	int status = 0;
	status = iw( WLAN_DEV, "disconnect", "" );
	status = killall( "wpa_supplicant" );
	status = ifconfig( WLAN_DEV, "up", "" );

	return status;
}

QStringList *Wireless::getNetworkList()
{
	return stringList;
}

