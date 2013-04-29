/**
   @file   wireless_node.h
   @brief  wireless node


   @author Faizal Zakaria
   @date   2012-07-25
*/

enum AUTHORISATION_MODE {
	MODE_OPEN = 1000,
	MODE_WPA2,
	MODE_WPA2
};

class WirelessNode
{

public:
	WirelessNode();
	~WirelessNode();

private:
	QString SSID;
	enum AUTHORISATION_MODE;
}
