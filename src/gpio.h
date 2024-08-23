// const int ledpin = 2;  // Pin WiFi
// const int monpin = 35; // Pin dia Lunes
// const int tuepin = 35; // Pin dia Martes
// const int wedpin = 26; // Pin dia Miercoles
// const int thupin = 27; // Pin dia Jueves
// const int fripin = 14; // Pin dia Viernes
// const int satpin = 12; // Pin dia Sabado
// const int sunpin = 13; // Pin dia Domingo
// int ledState;
// int monState;
// int tueState;
// int wedState;
// int thuState;
// int friState;
// int satState;
// int sunState;
#include <Arduino.h>
const int dayPins[] = {2, 4, 5, 12, 14, 16, 17}; // Pines recomendados para salidas
const char *days[] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};
int dayStates[7];
int prevDayStates[7];

void PinsConfiguration()
{
    for (int i = 0; i < 7; i++)
    {
        pinMode(dayPins[i], OUTPUT);
    }
}