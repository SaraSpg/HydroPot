#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <Arduino.h>

// Telegram setup
void telegramSetup();

// Handle incoming Telegram messages
void telegramHandleMessages();

// Scheduled plant status reports
void telegramScheduledReports();

// Send plant status
void sendPlantStatus(String chat_id);

// Watering notification
void sendWateringNotification(
    int plantNumber,
    unsigned long elapsedTime,
    bool firstWatering
);
// Plant error notifications
void sendPlantErrorNotification(int plantNumber);
void sendPlantRecoveryNotification(int plantNumber);

#endif