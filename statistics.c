#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static struct option CustomOptionList[] = {
    { "interval", 1, NULL, 'n' },
    { "statistic", 1, NULL, 's' },
    { NULL, 0, NULL, 0 },
};

void HandleArguments(int argc, char **argv, int *customInterval, char *customStatisticType) {
    int customRetVal;
    
    while ((customRetVal = getopt_long(argc, argv, "n:s:", CustomOptionList, NULL)) != -1) {
        switch (customRetVal) {
            case 'n':
                *customInterval = atoi(optarg);
                break;

            case 's':
                *customStatisticType = optarg[0];
                break;

            case '?':
                // Handle unknown argument or missing parameter
                break;
        }
    }
}

double CalculateMean(int *data, int size) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

double CalculateVariance(int *data, int size, double mean) {
    double sumSquaredDiff = 0;
    for (int i = 0; i < size; i++) {
        double diff = data[i] - mean;
        sumSquaredDiff += diff * diff;
    }
    return sumSquaredDiff / size;
}

int main(int argc, char **argv) {
    int customInterval = 0;
    char customStatisticType = '\0';
    
    HandleArguments(argc, argv, &customInterval, &customStatisticType);

    if (customInterval <= 0 || (customStatisticType != 'm' && customStatisticType != 'v')) {
        // Invalid input, handle accordingly
        return 1;
    }

    int *data = (int *)malloc(customInterval * sizeof(int));
    if (!data) {
        // Memory allocation failed, handle accordingly
        return 1;
    }

    // Read data from binary file
    FILE *file = fopen("data.dat", "rb");
    if (!file) {
        // File open failed, handle accordingly
        free(data);
        return 1;
    }

    size_t elementsRead = fread(data, sizeof(int), customInterval, file);
    fclose(file);

    if (elementsRead != customInterval) {
        // Read fewer elements than expected, handle accordingly
        free(data);
        return 1;
    }

    double result;
    if (customStatisticType == 'm') {
        result = CalculateMean(data, customInterval);
    } else if (customStatisticType == 'v') {
        double mean = CalculateMean(data, customInterval);
        result = CalculateVariance(data, customInterval, mean);
    }

    printf("%.6f\n", result);

    free(data);
    return 0;
}

