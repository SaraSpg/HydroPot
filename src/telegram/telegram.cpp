
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <time.h>

#include "../globals/globals.h"
#include "../checkSensors/checkSensors.h"
#include "../timeUtils/timeUtils.h"

#include "telegram.h"
#include "secrets.h"

// This function will be implemented in the FSM module.
// It returns true if the manual watering request is accepted.
extern bool requestManualWatering(uint8_t plantNumber);


// =====================================================
// TELEGRAM
// =====================================================

// Secure Wi-Fi client.
WiFiClientSecure client;

// Telegram bot instance.
UniversalTelegramBot bot(BOT_TOKEN, client);


// =====================================================
// SCHEDULED REPORT SETTINGS
// =====================================================

// Scheduled reports are enabled by default.
bool scheduledReportsEnabled = true;

// Prevent multiple reports on the same day.
int lastMorningReportDay = -1;
int lastEveningReportDay = -1;

// =====================================================
// PLANT ERROR NOTIFICATION SETTINGS
// =====================================================

// Prevent repeated notifications for the same error.
bool plant1ErrorNotified = false;
bool plant2ErrorNotified = false;


// =====================================================
// AUTHORIZATION
// =====================================================

// Check whether the message comes from the authorized user.
bool isAuthorizedUser(String chat_id)
{
    return chat_id == String(CHAT_ID);
}


// =====================================================
// TELEGRAM MENU
// =====================================================

void sendTelegramMenu(String chat_id)
{
    String reportButton = scheduledReportsEnabled
        ? "🔔 Scheduled Reports: ON"
        : "🔕 Scheduled Reports: OFF";

    String keyboard =
        "["
        "[{\"text\":\"🪴 Check Your Plants\"}],"
        "[{\"text\":\"💧 Water Plant 1\"},"
        "{\"text\":\"💧 Water Plant 2\"}],"
        "[{\"text\":\"" + reportButton + "\"}],"
        "[{\"text\":\"ℹ️ Info\"}]"
        "]";

    bot.sendMessageWithReplyKeyboard(
        chat_id,
        "🌱 *Hydropot*\n\n"
        "Welcome! 👋\n"
        "What would you like to do?",
        "Markdown",
        keyboard,
        true
    );
}


// =====================================================
// TELEGRAM SETUP
// =====================================================

void telegramSetup()
{
    Serial.println();
    Serial.println("Starting Telegram...");

    // Connect to Wi-Fi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Disable certificate verification.
    // Kept consistent with the original implementation.
    client.setInsecure();

    // Configure Italian time zone (CET/CEST).
    configTzTime(
        "CET-1CEST,M3.5.0,M10.5.0/3",
        "pool.ntp.org",
        "time.google.com"
    );

    Serial.println("Synchronizing time...");

    struct tm timeInfo;

    if (getLocalTime(&timeInfo, 10000))
    {
        Serial.println("Time synchronized!");

        Serial.print("Current time: ");
        Serial.println(&timeInfo, "%d/%m/%Y %H:%M:%S");
    }
    else
    {
        Serial.println("WARNING: Time synchronization failed.");
    }

    Serial.println("Telegram bot ready!");
}


// =====================================================
// PLANT STATUS
// =====================================================

// Generates and sends the same plant status message
// both for manual requests and scheduled reports.

void sendPlantStatus(String chat_id)
{
    // Refresh sensor values without modifying the FSM state.
    updateSensorReadings();

    // Calculate humidity percentages.
    int humidityPercent1 = constrain(
        map(
            valueSensor1,
            HUMIDITY_THRESHOLD_1,
            HUMIDITY_THRESHOLD_1 - 1000,
            0,
            100
        ),
        0,
        100
    );

    int humidityPercent2 = constrain(
        map(
            valueSensor2,
            HUMIDITY_THRESHOLD_2,
            HUMIDITY_THRESHOLD_2 - 1000,
            0,
            100
        ),
        0,
        100
    );


    // -------------------------------------------------
    // PLANT 1
    // -------------------------------------------------

    String message =
        "💧 *Plant Status*\n\n"

        "🌿 *Plant 1 — English Ivy*\n"
        "💧 Sensor value: " +
        String(valueSensor1) +
        "\n"
        "💦 Humidity: " +
        String(humidityPercent1) +
        "%\n";

    if (lastWateringTime1 == 0)
    {
        message +=
            "⏱️ No watering recorded yet.\n\n";
    }
    else
    {
        unsigned long elapsed1 =
            (millis() - lastWateringTime1) / 1000;

        message += "⏱️ Last watered: ";
        message += formatElapsedTime(elapsed1);
        message += " ago\n\n";
    }


    // -------------------------------------------------
    // PLANT 2
    // -------------------------------------------------

    message +=
        "🌿 *Plant 2 — Anthurium polyschistum*\n"
        "💧 Sensor value: " +
        String(valueSensor2) +
        "\n"
        "💦 Humidity: " +
        String(humidityPercent2) +
        "%\n";

    if (lastWateringTime2 == 0)
    {
        message +=
            "⏱️ No watering recorded yet.";
    }
    else
    {
        unsigned long elapsed2 =
            (millis() - lastWateringTime2) / 1000;

        message += "⏱️ Last watered: ";
        message += formatElapsedTime(elapsed2);
        message += " ago";
    }


    // Send the status message.
    bot.sendMessage(
        chat_id,
        message,
        "Markdown"
    );
}


// =====================================================
// SCHEDULED TELEGRAM REPORTS
// =====================================================

// Sends plant status automatically at 08:00 and 20:00.

void telegramScheduledReports()
{
    // Do nothing if reports are disabled.
    if (!scheduledReportsEnabled)
        return;

    // Get current local time.
    struct tm timeInfo;

    if (!getLocalTime(&timeInfo, 0))
        return;

    int currentDay = timeInfo.tm_yday;


    // -------------------------------------------------
    // MORNING REPORT — 08:00
    // -------------------------------------------------

    if (
        timeInfo.tm_hour == 8 &&
        timeInfo.tm_min == 0 &&
        lastMorningReportDay != currentDay
    )
    {
        Serial.println("Sending morning plant report...");

        sendPlantStatus(String(CHAT_ID));

        lastMorningReportDay = currentDay;
    }


    // -------------------------------------------------
    // EVENING REPORT — 20:00
    // -------------------------------------------------

    if (
        timeInfo.tm_hour == 20 &&
        timeInfo.tm_min == 0 &&
        lastEveningReportDay != currentDay
    )
    {
        Serial.println("Sending evening plant report...");

        sendPlantStatus(String(CHAT_ID));

        lastEveningReportDay = currentDay;
    }
}


// =====================================================
// TELEGRAM MESSAGE HANDLING
// =====================================================

void telegramHandleMessages()
{
    // Check for new Telegram messages.
    int numNewMessages = bot.getUpdates(
        bot.last_message_received + 1
    );

    for (int i = 0; i < numNewMessages; i++)
    {
        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;

        Serial.println();
        Serial.println("Telegram message received:");
        Serial.println(text);

        Serial.print("Chat ID: ");
        Serial.println(chat_id);


        // -------------------------------------------------
        // AUTHORIZATION
        // -------------------------------------------------

        if (!isAuthorizedUser(chat_id))
        {
            bot.sendMessage(
                chat_id,
                "⛔ Unauthorized user.",
                ""
            );

            continue;
        }


        // -------------------------------------------------
        // START
        // -------------------------------------------------

        if (text == "/start")
        {
            sendTelegramMenu(chat_id);
        }


        // -------------------------------------------------
        // CHECK YOUR PLANTS
        // -------------------------------------------------

        else if (text == "🪴 Check Your Plants")
        {
            sendPlantStatus(chat_id);
        }


        // -------------------------------------------------
        // SCHEDULED REPORTS ON/OFF
        // -------------------------------------------------

        else if (
            text == "🔔 Scheduled Reports: ON" ||
            text == "🔕 Scheduled Reports: OFF"
        )
        {
            scheduledReportsEnabled =
                !scheduledReportsEnabled;

            String message;

            if (scheduledReportsEnabled)
            {
                message =
                    "🔔 *Scheduled reports enabled!*\n\n"
                    "You will receive plant status updates "
                    "every day at 08:00 and 20:00.";
            }
            else
            {
                message =
                    "🔕 *Scheduled reports disabled.*\n\n"
                    "Automatic plant status updates are now OFF.";
            }

            bot.sendMessage(
                chat_id,
                message,
                "Markdown"
            );

            // Refresh the keyboard.
            sendTelegramMenu(chat_id);
        }


        // -------------------------------------------------
        // MANUAL WATERING — PLANT 1
        // -------------------------------------------------

        else if (text == "💧 Water Plant 1")
        {
            bot.sendMessage(
                chat_id,
                "🌿 *English Ivy*\n"
                "Checking irrigation system...",
                "Markdown"
            );

            if (requestManualWatering(1))
            {
                bot.sendMessage(
                    chat_id,
                    "💧 *Manual watering started!*\n\n"
                    "🌿 English Ivy is being watered.",
                    "Markdown"
                );
            }
            else
            {
                bot.sendMessage(
                    chat_id,
                    "⚠️ *Watering request rejected.*\n\n"
                    "The irrigation system is busy or "
                    "currently unavailable.",
                    "Markdown"
                );
            }
        }


        // -------------------------------------------------
        // MANUAL WATERING — PLANT 2
        // -------------------------------------------------

        else if (text == "💧 Water Plant 2")
        {
            bot.sendMessage(
                chat_id,
                "🌿 *Anthurium polyschistum*\n"
                "Checking irrigation system...",
                "Markdown"
            );

            if (requestManualWatering(2))
            {
                bot.sendMessage(
                    chat_id,
                    "💧 *Manual watering started!*\n\n"
                    "🌿 Anthurium polyschistum is being watered.",
                    "Markdown"
                );
            }
            else
            {
                bot.sendMessage(
                    chat_id,
                    "⚠️ *Watering request rejected.*\n\n"
                    "The irrigation system is busy or "
                    "currently unavailable.",
                    "Markdown"
                );
            }
        }


        // -------------------------------------------------
        // INFO
        // -------------------------------------------------

        else if (text == "ℹ️ Info")
        {
            String message =
                "🌱 *Meet Your Plants!*\n\n"

                "🌿 *1. English Ivy — Hedera helix*\n\n"

                "English Ivy is an evergreen climbing "
                "plant known for its characteristic "
                "lobed leaves and adaptability.\n\n"

                "☀️ *Light:* Prefers bright, indirect "
                "light but can tolerate partial shade.\n\n"

                "💧 *Water:* Allow the upper layer of "
                "soil to dry slightly between waterings. "
                "Avoid waterlogging, as it can cause "
                "root rot.\n\n"

                "🌡️ *Environment:* Prefers moderate "
                "temperatures and good air circulation.\n\n"

                "⚠️ Toxic if ingested by pets.\n\n"

                "━━━━━━━━━━━━━━\n\n"

                "🌿 *2. Anthurium polyschistum*\n\n"

                "A tropical member of the Araceae family, "
                "Anthurium polyschistum is appreciated "
                "for its distinctive, deeply divided "
                "foliage.\n\n"

                "☀️ *Light:* Prefers bright, indirect "
                "light. Avoid prolonged direct sunlight.\n\n"

                "💧 *Water:* Enjoys consistently slightly "
                "moist soil. Allow excess water to drain "
                "and avoid waterlogging.\n\n"

                "🌡️ *Environment:* Prefers warm conditions "
                "and relatively high humidity.\n\n"

                "🌱 *Growing tip:* An airy, well-draining "
                "substrate helps maintain healthy roots.\n\n"

                "━━━━━━━━━━━━━━\n\n"

                "💧 *Hydropot*\n"
                "Your plants are monitored automatically "
                "to help maintain suitable soil moisture.";

            bot.sendMessage(
                chat_id,
                message,
                "Markdown"
            );
        }
    }
}


// =====================================================
// WATERING NOTIFICATION
// =====================================================

// Sends a notification when a plant is watered.

void sendWateringNotification(
    int plantNumber,
    unsigned long elapsedTime,
    bool firstWatering
)
{
    String chat_id = String(CHAT_ID);

    String plantName = (plantNumber == 1)
        ? "English Ivy"
        : "Anthurium polyschistum";

    String message =
        "💧 *" + plantName + " was watered!*\n\n";

    if (firstWatering)
    {
        message +=
            "🌱 First watering recorded.";
    }
    else
    {
        message += "⏱️ After ";
        message += formatElapsedTime(elapsedTime);
        message += ", the plant was watered. 🌱";
    }

    bot.sendMessage(
        chat_id,
        message,
        "Markdown"
    );
}

// =====================================================
// PLANT ERROR NOTIFICATIONS
// =====================================================

// Sends an error notification only once for each plant.
void sendPlantErrorNotification(int plantNumber)
{
    bool* errorNotified = nullptr;

    String plantName;

    if (plantNumber == 1)
    {
        errorNotified = &plant1ErrorNotified;
        plantName = "English Ivy";
    }
    else if (plantNumber == 2)
    {
        errorNotified = &plant2ErrorNotified;
        plantName = "Anthurium polyschistum";
    }
    else
    {
        return;
    }

    // Avoid sending the same notification repeatedly.
    if (*errorNotified)
        return;

    *errorNotified = true;

    String message =
        "⚠️ *Plant health alert!*\n\n"
        "🌿 *" + plantName + "*\n\n"
        "The soil moisture level has not recovered "
        "as expected after watering.\n\n"
        "Please check the plant and its watering conditions.";

    bot.sendMessage(
        String(CHAT_ID),
        message,
        "Markdown"
    );

    Serial.print("Telegram error notification sent for Plant ");
    Serial.println(plantNumber);
}


// Sends a recovery notification if an error was previously reported.
void sendPlantRecoveryNotification(int plantNumber)
{
    bool* errorNotified = nullptr;

    String plantName;

    if (plantNumber == 1)
    {
        errorNotified = &plant1ErrorNotified;
        plantName = "English Ivy";
    }
    else if (plantNumber == 2)
    {
        errorNotified = &plant2ErrorNotified;
        plantName = "Anthurium polyschistum";
    }
    else
    {
        return;
    }

    // Do not send a recovery message if no error was reported.
    if (!(*errorNotified))
        return;

    *errorNotified = false;

    String message =
        "✅ *Plant recovered!*\n\n"
        "🌿 *" + plantName + "*\n\n"
        "The soil moisture level is now back "
        "within the expected range.";

    bot.sendMessage(
        String(CHAT_ID),
        message,
        "Markdown"
    );

    Serial.print("Telegram recovery notification sent for Plant ");
    Serial.println(plantNumber);
}