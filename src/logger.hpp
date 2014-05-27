/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef LOGGING_HPP
#define LOGGING_HPP
#include <iostream>
#include <string>

namespace logger {
	class Logger {
	public:
		Logger(std::ostream& stream, const std::string& prefix)
		: stream(stream)
		, prefix{prefix}{};

		template<typename T>
		std::ostream& operator << (const T& object) {
			stream << prefix << object;
			return stream;
		}
	private:
		std::ostream& stream;
		std::string prefix;
	};

	static Logger warn{std::cerr, "WARNING: "};
	static Logger info{std::cerr, "INFO: "};
	static Logger error{std::cerr, "ERROR: "};
}

#endif // DEBUG_HPP