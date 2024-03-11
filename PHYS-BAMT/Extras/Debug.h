#pragma once
#include <string>

namespace physBamt
{
    namespace Extra
    {
    enum class BamtLogType
    {
        Info,
        Warning,
        Error,
    };

    /// Logs a message to the console.
    void BamtLog(std::string output = "", BamtLogType type = BamtLogType::Info);

    /// Logs a message to the console, and also provides the address to an object in memory.
    void BamtLog(BamtLogType type, std::string output, void* pointer);
    
    /// Returns the current time as a string. Returns the format [YEAR-MONTH-DAY HOUR:MINUTE:SECOND]
    std::string GetTime();

    /// Returns the address of a pointer as a string.
    std::string GetPointerAddress(const void* pointer);

    /// Changes the colour of the console text.
    void SetColour(int colour);
    }
}




