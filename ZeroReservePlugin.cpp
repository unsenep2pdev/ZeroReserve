/*
    This file is part of the Zero Reserve Plugin for Retroshare.

    Zero Reserve is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Zero Reserve is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Zero Reserve.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <retroshare/rsplugin.h>
#include <util/rsversion.h>
#include <QTranslator>

#include "ZeroReservePlugin.h"
#include "ZeroReserveDialog.h"
#include "OrderBook.h"
#include "p3ZeroReserverRS.h"



extern "C" {
	void *RETROSHARE_PLUGIN_provide()
	{
		static ZeroReservePlugin *p = new ZeroReservePlugin() ;

		return (void*)p ;
	}
	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_revision = SVN_REVISION_NUMBER ;

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_api = RS_PLUGIN_API_VERSION ;
}

#define IMAGE_LINKS ":/images/bitcoin.png"

void ZeroReservePlugin::getPluginVersion(int& major,int& minor,int& svn_rev) const
{
	major = 5 ;
	minor = 4 ;
	svn_rev = SVN_REVISION_NUMBER ;
}

ZeroReservePlugin::ZeroReservePlugin()
{
	mainpage = NULL ;
        mIcon = NULL ;
        mPlugInHandler = NULL;
        m_ZeroReserve = NULL;
//        mPeers = NULL;
//        mFiles = NULL;

        m_asks = new OrderBook();
        m_bids = new OrderBook();
}

void ZeroReservePlugin::setInterfaces(RsPlugInInterfaces &interfaces)
{
//    mPeers = interfaces.mPeers;
//    mFiles = interfaces.mFiles;
}

MainPage *ZeroReservePlugin::qt_page() const
{
    if(mainpage == NULL){
        mainpage = new ZeroReserveDialog(m_bids, m_asks);
    }

    return mainpage ;
}


void ZeroReservePlugin::setPlugInHandler(RsPluginHandler *pgHandler)
{
    mPlugInHandler = pgHandler;
}

QIcon *ZeroReservePlugin::qt_icon() const
{
	if(mIcon == NULL)
	{
		Q_INIT_RESOURCE(ZeroReserve_images) ;

		mIcon = new QIcon(IMAGE_LINKS) ;
	}

	return mIcon ;
}

RsPQIService * ZeroReservePlugin::rs_pqi_service() const
{
        if(m_ZeroReserve == NULL){
                m_ZeroReserve = new p3ZeroReserveRS(mPlugInHandler) ;
        }

        return m_ZeroReserve ;
}

std::string ZeroReservePlugin::getShortPluginDescription() const
{
        return QApplication::translate("ZeroReservePlugin", "This plugin implements a distributed Bitcoin exchange.").toUtf8().constData();
}

std::string ZeroReservePlugin::getPluginName() const
{
	return QApplication::translate("ZeroReservePlugin", "Zero Reserve").toUtf8().constData();
}

QTranslator* ZeroReservePlugin::qt_translator(QApplication */*app*/, const QString& languageCode, const QString& externalDir) const
{
	if (languageCode == "en") {
		return NULL;
	}

	QTranslator* translator = new QTranslator();

	if (translator->load(externalDir + "/ZeroReserve_" + languageCode + ".qm")) {
		return translator;
	} else if (translator->load(":/lang/ZeroReserve_" + languageCode + ".qm")) {
		return translator;
	}

	delete(translator);
	return NULL;
}
