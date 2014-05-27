/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef PROBLEMATIC_CONNECTION_EXCEPTION_HPP
#define PROBLEMATIC_CONNECTION_EXCEPTION_HPP
#include <exception>

class ProblematicConnectionException : public std::exception {
	const char* what() const noexcept { return "The connection is problematic. Retrying..."; }
};

#endif // PROBLEMATIC_CONNECTION_EXCEPTION_HPP