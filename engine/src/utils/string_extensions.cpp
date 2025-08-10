#include "utils/string_extensions.h"

#include <algorithm>

namespace tool
{
	namespace string_extensions
	{
		//-----------------------------------------------------------------------------------------------------
		std::string StringToUpper(std::string a_sStrToConvert)
		{
			std::transform(a_sStrToConvert.begin(), a_sStrToConvert.end(), a_sStrToConvert.begin(), ::toupper);

			return a_sStrToConvert;
		}

		//-----------------------------------------------------------------------------------------------------
		std::string StringToLower(std::string a_sStrToConvert)
		{
			std::transform(a_sStrToConvert.begin(), a_sStrToConvert.end(), a_sStrToConvert.begin(), ::tolower);

			return a_sStrToConvert;
		}
	}
}