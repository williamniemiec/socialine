#pragma once
#include <iostream>

namespace wniemiec::io::consolex
{
	/// <summary>
	///     Responsible for console input.
	/// </summary>
	class InputConsolex
	{
	//-------------------------------------------------------------------------
	//		Methods
	//-------------------------------------------------------------------------
	public:
		std::string read_line();
	};
}
