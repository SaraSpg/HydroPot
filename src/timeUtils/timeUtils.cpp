#include "timeUtils.h"

// ----------- Function to format elapsed time ----------- //
// This function takes an elapsed time in seconds and formats it into a human-readable string.


String formatElapsedTime(unsigned long seconds)
{
    unsigned long days = seconds / 86400;
    unsigned long hours = (seconds % 86400) / 3600;
    unsigned long minutes = (seconds % 3600) / 60;
    unsigned long secs = seconds % 60;

    String result = "";

    if (days > 0)
    {
        result += String(days);
        result += (days == 1) ? " day" : " days";
    }

    if (hours > 0)
    {
        if (result.length() > 0)
            result += ", ";

        result += String(hours);
        result += (hours == 1) ? " hour" : " hours";
    }

    if (minutes > 0)
    {
        if (result.length() > 0)
            result += ", ";

        result += String(minutes);
        result += (minutes == 1) ? " minute" : " minutes";
    }

    if (secs > 0 || result.length() == 0)
    {
        if (result.length() > 0)
            result += ", ";

        result += String(secs);
        result += (secs == 1) ? " second" : " seconds";
    }

    return result;
}