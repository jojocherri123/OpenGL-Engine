#include "errorhandler.hpp"

// Common error handling function with caller information
void handleError(const std::string &errorMessage, const char *callerFunc, const char *file, int line)
{
    std::cerr << "Error in function: " << callerFunc
              << ", file: " << file
              << ", line: " << line
              << ". " << errorMessage << std::endl;
    std::exit(EXIT_FAILURE); // Terminates the program with a failure exit code
}
