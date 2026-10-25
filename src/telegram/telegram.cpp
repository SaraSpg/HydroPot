#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "../globals/globals.h"
#include "../checkSensors/checkSensors.h"
#include "../timeUtils/timeUtils.h"

#include "telegram.h"
#include "secrets.h"

// =========================
// Telegram
// =========================

// Secure Wi-Fi client used to establish the connection with Telegram.
WiFiClientSecure client;

// Telegram bot instance, initialized using the token stored in secrets.h.
UniversalTelegramBot bot(BOT_TOKEN, client);


// =========================
// Telegram Menu
// =========================

// Sends the main menu to the user using Telegram's reply keyboard.
void sendTelegramMenu(String chat_id)
{
    // Define the buttons displayed in the Telegram menu.
    String keyboard =
        "["
        "[{\"text\":\"🪴 Check Your Plants\"}],"
        "[{\"text\":\"ℹ️ Info\"}]"
        "]";

    // Send the menu together with a welcome message.
    bot.sendMessageWithReplyKeyboard(
        chat_id,
        "🌱 *Hydropot*\n\n"
        "Welcome! 👋\n"
        "What would you like to check?",
        "Markdown",
        keyboard,
        true);
}


// =========================
// Telegram Setup
// =========================

void telegramSetup()
{
    Serial.println();
    Serial.println("Starting ESP32...");

    // Start the Wi-Fi connection using the credentials stored in secrets.h.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");

    // Wait until the ESP32 is connected to the Wi-Fi network.
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");

    // Print the IP address assigned to the ESP32.
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Disable certificate verification for the secure connection.
    // This simplifies the Telegram connection for this project.
    client.setInsecure();

    Serial.println("Telegram bot ready!");
}


// =========================
// Handle Telegram messages
// =========================

void telegramHandleMessages()
{
    // Check Telegram for new messages.
    // Only messages received after the last processed message are requested.
    int numNewMessages = bot.getUpdates(
        bot.last_message_received + 1);

    // Process each new message received.
    for (int i = 0; i < numNewMessages; i++)
    {
        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;

        // Print received messages to the Serial Monitor for debugging.
        Serial.println();
        Serial.println("Message received:");
        Serial.println(text);

        Serial.print("Chat ID: ");
        Serial.println(chat_id);


        // =========================
        // START
        // =========================

        // The /start command opens the main Telegram menu.
        if (text == "/start")
        {
            sendTelegramMenu(chat_id);
        }


        // =========================
        // CHECK YOUR PLANTS
        // =========================

        else if (text == "🪴 Check Your Plants")
        {
            // Update the sensor values without changing the FSM state.
            // This allows Telegram to monitor the system without
            // interfering with the automatic irrigation process.
            updateSensorReadings();

            // Calculate humidity percentages matching the sensor thresholds
            int humidityPercent1 = constrain(map(valueSensor1, HUMIDITY_THRESHOLD_1, HUMIDITY_THRESHOLD_1 - 1000, 0, 100), 0, 100);
            int humidityPercent2 = constrain(map(valueSensor2, HUMIDITY_THRESHOLD_2, HUMIDITY_THRESHOLD_2 - 1000, 0, 100), 0, 100);

            // Build the status message for Plant 1.
            String message =
                "💧 *Last Watered*\n\n"

                "🌿 *Plant 1*\n"
                "💧 Sensor value: " +
                String(valueSensor1) +
                "\n"
                "💦 Humidity: " +
                String(humidityPercent1) +
                "%\n";

            // Check whether Plant 1 has ever been watered.
            if (lastWateringTime1 == 0)
            {
                message += "⏱️ No watering recorded yet.\n\n";
            }
            else
            {
                // Calculate the time elapsed since the last watering.
                unsigned long elapsed1 =
                    (millis() - lastWateringTime1) / 1000;

                message += "⏱️ Last watered: ";
                message += formatElapsedTime(elapsed1);
                message += " ago\n\n";
            }


            // Build the status message for Plant 2.
            message +=
                "🌿 *Plant 2*\n"
                "💧 Sensor value: " +
                String(valueSensor2) +
                "\n"
                "💦 Humidity: " +
                String(humidityPercent2) +
                "%\n";

            // Check whether Plant 2 has ever been watered.
            if (lastWateringTime2 == 0)
            {
                message += "⏱️ No watering recorded yet.";
            }
            else
            {
                // Calculate the time elapsed since the last watering.
                unsigned long elapsed2 =
                    (millis() - lastWateringTime2) / 1000;

                message += "⏱️ Last watered: ";
                message += formatElapsedTime(elapsed2);
                message += " ago";
            }

            // Send the complete status message to the user.
            bot.sendMessage(
                chat_id,
                message,
                "Markdown");
        }


        // =========================
        // INFO
        // =========================

        else if (text == "ℹ️ Info")
        {
            // Information message describing the Hydropot system.
            String message =
                "ℹ️ *About Hydropot*\n\n"

                "🌱 Hydropot is an automatic "
                "irrigation system based on an ESP32.\n\n"

                "🌿 It monitors the soil moisture "
                "of two plants and activates the "
                "corresponding pump when watering "
                "is required.\n\n"

                "🪴 *Check Your Plants*\n"
                "Shows the latest sensor readings "
                "and information about the last "
                "watering of each plant.\n\n"

                "⚙️ The Telegram bot can monitor "
                "the system without interfering "
                "with the automatic irrigation process.";

            // Send the information message.
            bot.sendMessage(
                chat_id,
                message,
                "Markdown");
        }
    }
}


// =========================
// Watering Notification
// =========================

// Sends a Telegram notification when a plant is watered.
void sendWateringNotification(
    int plantNumber,
    unsigned long elapsedTime,
    bool firstWatering)
{
    // Use the Telegram chat ID stored in secrets.h.
    String chat_id = CHAT_ID;

    // Create the basic watering notification.
    String message =
        "💧 *Plant " +
        String(plantNumber) +
        " was watered!*\n\n";

    // Display a different message for the first watering.
    if (firstWatering)
    {
        message += "🌱 First watering recorded.";
    }
    else
    {
        // For subsequent waterings, show the time elapsed
        // since the previous watering.
        message += "⏱️ After ";
        message += formatElapsedTime(elapsedTime);
        message += ", the plant was watered. 🌱";
    }

    // Send the notification to Telegram.
    bot.sendMessage(
        chat_id,
        message,
        "Markdown");
}