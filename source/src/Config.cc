  /// \file Config.cc
/*
 *
 * Config.cc source template automatically generated by a class generator
 * Creation date : lun. avr. 28 2014
 *
 * This file is part of SDHCALEventDisplay libraries.
 * 
 * SDHCALEventDisplay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * SDHCALEventDisplay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SDHCALEventDisplay.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#include "Config.h"

// sdhcal
#include "tinyxml.h"

// gear
#include "gear/GearMgr.h"
#include "gearxml/GearXML.h"

// lcio
#include "Exceptions.h"

// streamlog
#include "streamlog/streamlog.h"

namespace sdhcal
{

Config *Config::m_pInstance = NULL;

//-------------------------------------------------------------------------------------------

Config::Config()
 : m_pGearMgr(NULL)
{

}

//-------------------------------------------------------------------------------------------

Config::~Config() 
{

}

//-------------------------------------------------------------------------------------------

Config *Config::getInstance()
{
	if(NULL == m_pInstance)
		m_pInstance = new Config();

	return m_pInstance;
}

//-------------------------------------------------------------------------------------------

void Config::kill()
{
	if(NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

//-------------------------------------------------------------------------------------------

void Config::parseXmlConfig(const std::string &xmlFileName)
{
	TiXmlDocument document;

	bool ok = document.LoadFile(xmlFileName);

	if(!ok)
	{
		std::stringstream ss;

  ss << "Config::parseXmlConfig error in file [" << xmlFileName
       << ", row: " << document.ErrorRow() << ", col: " << document.ErrorCol() << "] : "
       << document.ErrorDesc();

		throw EVENT::Exception(ss.str());
	}

	TiXmlElement *pRootElement = document.RootElement();

	if(0 == pRootElement)
	{
		std::stringstream ss;

  ss << "Config::parseXmlConfig : no root tag found in " << xmlFileName;

		throw EVENT::Exception(ss.str());
	}
}

//-------------------------------------------------------------------------------------------

void Config::parseGearFile(const std::string &gearFileName)
{
	if(NULL != m_pGearMgr)
		throw EVENT::Exception("Gear file parsed more than once !");

	gear::GearXML gearXmlFile(gearFileName);

	try
	{
		m_pGearMgr = gearXmlFile.createGearMgr();
	}
	catch(gear::Exception &e)
	{
		streamlog_out(WARNING) << "Exception thrown while reading gear file. Message : " << e.what() << std::endl;
		throw e;
	}
}


} 

