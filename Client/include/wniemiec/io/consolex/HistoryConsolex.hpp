#pragma once
#include <iostream>
#include <list>

namespace wniemiec::io::consolex
{
	/// <summary>
    	///     Responsible for managing console history.
    	/// </summary>
	class HistoryConsolex
	{
	//-------------------------------------------------------------------------
	//		Attributes
	//-------------------------------------------------------------------------
	private:
		static const int BUFFER_SIZE;
		std::list<std::string> buffer;


	//-------------------------------------------------------------------------
	//		Constructor
	//-------------------------------------------------------------------------
	public:
		HistoryConsolex();


	//-------------------------------------------------------------------------
	//		Methods
	//-------------------------------------------------------------------------
	public:
		void add(std::string content);
		void free_buffer();
		void clear();
		void dump_to(std::string output);


		//-------------------------------------------------------------------------
		//		Getters
		//-------------------------------------------------------------------------
	public:
		std::list<std::string> get_history();
	};
}
