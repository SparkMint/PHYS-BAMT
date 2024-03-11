#define _CRT_SECURE_NO_WARNINGS // Disables the warnings for the use of unsafe functions. This is bad, but I dont care!
#include "Debug.h"

#include <ctime>
#include <iostream>
#include <sstream>
#include <windows.h>

namespace physBamt
{
    namespace Extra
    {
        int LOG_COLOUR = 15;
        int WARN_COLOUR = 14;
        int ERROR_COLOUR = 12;

        void BamtLog(std::string output, BamtLogType type)
        {
            std::string typeString;
            // Set the colour of our text to whatever severity the log is.
            switch (type)
            {
            case BamtLogType::Info:
                SetColour(LOG_COLOUR);
                typeString = "INFO";
                break;
            case BamtLogType::Warning:
                SetColour(WARN_COLOUR);
                typeString = "WARNING";
                break;
            case BamtLogType::Error:
                SetColour(ERROR_COLOUR);
                typeString = "ERROR";
                break;
            }

            // Print our log out to the console.
            std::cout << "[" << GetTime() << "] " << typeString << " - " << output << '\n';
        }

        std::string GetTime()
        {
            std::ostringstream outputStream;

            const time_t currentTime = time(0);
            struct tm timeStruct;
            char LogTime[80];
            timeStruct = *localtime(&currentTime);
            strftime(LogTime, sizeof(LogTime), "%Y-%m-%d %X", &timeStruct);

            outputStream << LogTime;
            return outputStream.str();
        }

        std::string GetPointerAddress(const void* pointer)
        {
            std::ostringstream address;
            address << pointer;
            return address.str();
        }

        void SetColour(const int colour)
        {
            const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(handle, colour);
        }
    }
}




