#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <functional>
#include <cstdlib>

// Common error handling function with caller information
void handleError(const std::string &errorMessage, const char *callerFunc, const char *file, int line);

// Template function to handle integer return codes
template <typename Func, typename... Args>
void executeWithHandlerImpl(Func func, const char *callerFunc, const char *file, int line, Args... args)
{
    int returnCode = func(args...);
    if (returnCode != 0)
    {
        handleError("Function returned an error code: " + std::to_string(returnCode), callerFunc, file, line);
    }
    else
    {
        std::cout << "Function executed successfully with code 0." << std::endl;
    }
}

// Template function to handle nullptr returns and return the result
template <typename Func, typename... Args>
auto executeWithNullHandlerImpl(Func func, const char *callerFunc, const char *file, int line, Args... args) -> decltype(func(args...))
{
    auto result = func(args...);
    if (result == nullptr)
    {
        handleError("Function returned a nullptr.", callerFunc, file, line);
    }
    std::cout << "Function executed successfully and returned a valid pointer." << std::endl;
    return result; // Return the valid result
}

// Macros to capture caller information
#define executeWithHandler(func, ...) executeWithHandlerImpl(func, __func__, __FILE__, __LINE__, __VA_ARGS__)
#define executeWithNullHandler(func, ...) executeWithNullHandlerImpl(func, __func__, __FILE__, __LINE__, __VA_ARGS__)
