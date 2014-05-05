/**
 *  @file   PandoraPFANew/Framework/include/Helpers/XmlHelper.h
 * 
 *  @brief  Header file for the xml helper class.
 * 
 *  $Log: $
 */
#ifndef XML_HELPER_H
#define XML_HELPER_H 1

#include "tinyxml.h"

#include "Exceptions.h"

#include "CartesianVector.h"

#include <sstream>
#include <vector>
#include <string>

namespace sdhcal
{

template <class T>
inline bool StringToType(const std::string &s, T &t)
{
    std::istringstream iss(s);
    return !(iss >> t).fail();
}

template <class T>
inline std::string TypeToString(const T &t)
{
    std::ostringstream oss;

    if ((oss << t).fail())
        throw;

    return oss.str();
}


/**
 *  @brief  XmlHelper class
 */
class XmlHelper
{
public:
    /**
     *  @brief  Read a value from an xml element
     * 
     *  @param  xmlHandle the relevant xml handle
     *  @param  xmlElementName the name of the xml element to examine
     *  @param  t to receive the value
     */
    template <typename T>
    static void ReadValue(const TiXmlHandle &xmlHandle, const std::string &xmlElementName, T &t);

    /**
     *  @brief  Read a vector of values from a (space separated) list in an xml element
     * 
     *  @param  xmlHandle the relevant xml handle
     *  @param  xmlElementName the name of the xml element to examine
     *  @param  vector to receive the vector of values
     */
    template <typename T>
    static void ReadVectorOfValues(const TiXmlHandle &xmlHandle, const std::string &xmlElementName, std::vector<T> &vector);

    /**
     *  @brief  Read a two-dimensional array of values into a vector of vectors. Each row of values must be contained
     *          within <rowname></rowname> xml tags, whilst the values in the row must be space separated
     * 
     *  @param  xmlHandle the relevant xml handle
     *  @param  xmlElementName the name of the xml element to examine
     *  @param  rowName the row name
     *  @param  vector to receive the 2d vector of values
     */
    template <typename T>
    static void Read2DVectorOfValues(const TiXmlHandle &xmlHandle, const std::string &xmlElementName, const std::string &rowName,
        std::vector< std::vector<T> > &vector);

    /**
     *  @brief  Tokenize a string
     * 
     *  @param  inputString the input string
     *  @param  tokens to receive the resulting tokens
     *  @param  delimiter the specified delimeter
     */
    static void TokenizeString(const std::string &inputString, std::vector<std::string> &tokens, const std::string &delimiter = " ");
};

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline void XmlHelper::ReadValue(const TiXmlHandle &xmlHandle, const std::string &xmlElementName, T &t)
{
	const TiXmlElement *const pXmlElement = xmlHandle.FirstChild(xmlElementName).Element();

	if (NULL == pXmlElement)
	{
		std::stringstream ss;
		ss << "Xml element " << xmlElementName << " not found !";
		throw EVENT::Exception(ss.str());
	}

	if (!StringToType(pXmlElement->GetText(), t))
	{
		std::stringstream ss;
		ss << "Bad value conversion fo xml element " << xmlElementName;
		throw EVENT::Exception(ss.str());
	}
}

template <>
inline void XmlHelper::ReadValue<CartesianVector>(const TiXmlHandle &xmlHandle, const std::string &xmlElementName, CartesianVector &t)
{
	const TiXmlElement *const pXmlElement = xmlHandle.FirstChild(xmlElementName).Element();

	if (NULL == pXmlElement)
	{
		std::stringstream ss;
		ss << "Xml element " << xmlElementName << " not found !";
		throw EVENT::Exception(ss.str());
	}

	std::vector<std::string> tokens;
	TokenizeString(pXmlElement->GetText(), tokens);

	if (tokens.size() != 3)
	{
		std::stringstream ss;
		ss << "Bad type conversion for xml element " << xmlElementName << ". CartesianVector type expected !";
		throw EVENT::Exception(ss.str());
	}

	float x(0.f), y(0.f), z(0.f);

	if (!StringToType(tokens[0], x) || !StringToType(tokens[1], y) || !StringToType(tokens[2], z))
	{
		std::stringstream ss;
		ss << "Bad value conversion fo xml element " << xmlElementName;
		throw EVENT::Exception(ss.str());
	}

	t = CartesianVector(x, y, z);
}

template <>
inline void XmlHelper::ReadValue<bool>(const TiXmlHandle &xmlHandle, const std::string &xmlElementName, bool &t)
{
	const TiXmlElement *const pXmlElement = xmlHandle.FirstChild(xmlElementName).Element();

	if (NULL == pXmlElement)
	{
		std::stringstream ss;
		ss << "Xml element " << xmlElementName << " not found !";
		throw EVENT::Exception(ss.str());
	}

	const std::string xmlElementString = pXmlElement->GetText();

	if ((xmlElementString == "1") || (xmlElementString == "true"))
	{
	t = true;
	}
	else if ((xmlElementString == "0") || (xmlElementString == "false"))
	{
	t = false;
	}
	else
	{
		std::stringstream ss;
		ss << "Bad conversion for boolean type for xml element " << xmlElementName;
		throw EVENT::Exception(ss.str());
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline void XmlHelper::ReadVectorOfValues(const TiXmlHandle &xmlHandle, const std::string &xmlElementName, std::vector<T> &vector)
{
	const TiXmlElement *const pXmlElement = xmlHandle.FirstChild(xmlElementName).Element();

	if (NULL == pXmlElement)
	{
		std::stringstream ss;
		ss << "Xml element " << xmlElementName << " not found !";
		throw EVENT::Exception(ss.str());
	}

	std::vector<std::string> tokens;
	TokenizeString(pXmlElement->GetText(), tokens);

	for (std::vector<std::string>::const_iterator iter = tokens.begin(), iterEnd = tokens.end(); iter != iterEnd; ++iter)
	{
		T t;

		if (!StringToType(*iter, t))
		{
			std::stringstream ss;
			ss << "Bad value conversion fo xml element " << xmlElementName;
			throw EVENT::Exception(ss.str());
		}

		vector.push_back(t);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline void XmlHelper::Read2DVectorOfValues(const TiXmlHandle &xmlHandle, const std::string &xmlElementName, const std::string &rowName,
    std::vector< std::vector<T> > &vector)
{
	TiXmlElement *pXmlElement = xmlHandle.FirstChild(xmlElementName).Element();

	if (NULL == pXmlElement)
	{
		std::stringstream ss;
		ss << "Xml element " << xmlElementName << " not found !";
		throw EVENT::Exception(ss.str());
	}

	TiXmlElement *pXmlRowElement = TiXmlHandle(pXmlElement).FirstChild(rowName).Element();

	if (NULL == pXmlRowElement)
	{
		std::stringstream ss;
		ss << "Xml row element " << rowName << " not found !";
		throw EVENT::Exception(ss.str());
	}

	for ( ; NULL != pXmlRowElement; pXmlRowElement = pXmlRowElement->NextSiblingElement(rowName))
	{
		std::vector<T> rowVector;

		std::vector<std::string> tokens;
		TokenizeString(pXmlRowElement->GetText(), tokens);

		for (std::vector<std::string>::const_iterator iter = tokens.begin(), iterEnd = tokens.end(); iter != iterEnd; ++iter)
		{
			T t;

			if (!StringToType(*iter, t))
			{
				std::stringstream ss;
				ss << "Bad value conversion fo xml element " << xmlElementName;
				throw EVENT::Exception(ss.str());
			}

			rowVector.push_back(t);
		}

		vector.push_back(rowVector);
	}
}

}

#endif // #ifndef XML_HELPER_H
