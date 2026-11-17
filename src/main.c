#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#define PIN 17

int isTemperatureAcceptable(int temperature, int minTemp, int maxTemp)
{
    if (temperature >= minTemp && temperature <= maxTemp)
    {
        return 1; // temperature is acceptable
    }
    else
    {
        return 0; // temperature is not acceptable
    }
}
void saveTemperatureToFile(int temperature, int minTemp, int maxTemp)
{
    // \\C:\Users\hxt5811@mavs.uta.edu\Documents\cse1006-final-main\data\temperature_log.csv
    FILE* file = fopen("../data/temperature_log.csv", "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%d, %s\n", temperature, isTemperatureAcceptable(temperature, minTemp, maxTemp) ? "NORMAL" : "WARNING");
    fclose(file);
    printf("Temperature reading saved to file.\n");
}
// main function
int main(int argc, char* argv[])
{
    gpioInitialise();
    gpioSetMode(PIN, PI_OUTPUT);
    int temperature;
    int minTemp = 68; // default minimum temperature
    int maxTemp = 76; // default maximum temperature
    int choice;
    while (1)
    {
        printf("1. Enter temperature reading\n");
        printf("2. Set acceptable temperature range\n");
        printf("3. Calculate average of recorded temperatures\n");
        printf("4. Exit\n");
        printf("Choose an option(1-4): ");
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("Enter the classroom temperature reading in Fahrenheit: ");
            scanf("%d", &temperature);
            if (isTemperatureAcceptable(temperature, minTemp, maxTemp))
            {
                printf("NORMAL\n");
                gpioWrite(PIN, 1); // turn on LED for NORMAL status
            }
            else
            {
                printf("WARNING\n");
                gpioWrite(PIN, 0); // turn off LED for WARNING status
            }
            saveTemperatureToFile(temperature, minTemp, maxTemp);
        }
        else if (choice == 2)
        {
            printf("Enter the minimum acceptable temperature in Fahrenheit: ");
            scanf("%d", &minTemp);
            printf("Enter the maximum acceptable temperature in Fahrenheit: ");
            scanf("%d", &maxTemp);
            printf("Acceptable temperature range set to %dF - %dF\n", minTemp, maxTemp);
        }
        else if (choice == 3)
        {
            // Calculate average of recorded temperatures
            FILE* file = fopen("../data/temperature_log.csv", "r");
            if (file == NULL)
            {
                printf("Error opening file!\n");
                continue;
            }
            int sum = 0;
            int count = 0;
            int temperature;
            char status[10];
            while (fscanf(file, "%d, %s\n", &temperature, status) == 2)
            {
                sum += temperature;
                count++;
            }
            if (count > 0)
            {
                double average = (double)sum / count;
                printf("Average temperature: %.2f\n", average);
                printf("Average temperature calculation completed.\n");
            }
            else
            {
                printf("No temperature readings found. Cannot calculate average.\n");
            }
            fclose(file);
        }
        else if (choice == 4)
        {
            break;
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }
    }
    gpioTerminate();
    return 0;
}