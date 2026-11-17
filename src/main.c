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
    FILE* file = fopen("temperature_readings.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "Temperature: %d, Min: %d, Max: %d\n", temperature, minTemp, maxTemp);
    fclose(file);
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
            printf("Temperature reading saved to file.\n");
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
            FILE* file = fopen("temperature_readings.txt", "r");
            if (file == NULL)
            {
                printf("Error opening file!\n");
                continue;
            }
            int sum = 0;
            int count = 0;
            int temperature;
            while (fscanf(file, "Temperature: %d, Min: %d, Max: %d\n", &temperature, &minTemp, &maxTemp) == 3)
            {
                sum += temperature;
                count++;
            }
            if (count > 0)
            {
                double average = (double)sum / count;
                printf("Average temperature: %.2f\n", average);
            }
            else
            {
                printf("No temperature readings found. Cannot calculate average.\n");
            }
            fclose(file);
            printf("Average temperature calculation completed.\n");
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