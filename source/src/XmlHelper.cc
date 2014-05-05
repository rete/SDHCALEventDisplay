/**
 *  @file   PandoraPFANew/Framework/src/Helpers/XmlHelper.cc
 * 
 *  @brief  Implementation of the xml helper class.
 * 
 *  $Log: $
 */

#include "XmlHelper.h"

namespace sdhcal
{

void XmlHelper::TokenizeString(const std::string &inputString, std::vector<std::string> &tokens, const std::string &delimiter)
{
    std::string::size_type lastPos = inputString.find_first_not_of(delimiter, 0);
    std::string::size_type pos     = inputString.find_first_of(delimiter, lastPos);

    while ((std::string::npos != pos) || (std::string::npos != lastPos))
    {
        tokens.push_back(inputString.substr(lastPos, pos - lastPos));
        lastPos = inputString.find_first_not_of(delimiter, pos);
        pos = inputString.find_first_of(delimiter, lastPos);
    }
}

}
