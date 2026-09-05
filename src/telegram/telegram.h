#ifndef TELEGRAM_H
#define TELEGRAM_H

void telegramSetup();
void telegramHandleMessages();
void sendWateringNotification(int plantNumber, unsigned long elapsedTime, bool firstWatering);

#endif