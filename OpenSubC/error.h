#ifndef OPENSUBC_ERROR_H
#define OPENSUBC_ERROR_H

#include <cstring>
#include <string>
#include <sstream>

#include "capi.h"

#ifdef __GNUC__
#define UNREACHABLE() __builtin_unreachable()
#else
#define UNREACHABLE() (void)0
#endif

namespace opensubc {

	inline void
		set_errmsg(const char* message)
	{
		std::strcpy(opensubc_err_msg, message);
	}

	inline void
		set_errmsg(const std::string& message)
	{
		std::strcpy(opensubc_err_msg, message.c_str());
	}

	inline void
		set_errmsg(const std::stringstream& message)
	{
		std::strcpy(opensubc_err_msg, message.str().c_str());
	}

	[[noreturn]] void fatal_error(const std::string& message, int err = -1);

	[[noreturn]] inline
		void fatal_error(const std::stringstream& message)
	{
		fatal_error(message.str());
	}

	[[noreturn]] inline
		void fatal_error(const char* message)
	{
		fatal_error(std::string{ message, std::strlen(message) });
	}

	void warning(const std::string& message);

	inline
		void warning(const std::stringstream& message)
	{
		warning(message.str());
	}

	void write_message(const std::string& message, int level = 0);

	inline
		void write_message(const std::stringstream& message, int level)
	{
		write_message(message.str(), level);
	}

#ifdef OPENSUBC_MPI
	extern "C" void abort_mpi(int code);
#endif

} // namespace opensubc
#endif // OPENSUBC_ERROR_H
