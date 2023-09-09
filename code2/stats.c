#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define AVG "avg"
#define VAR "var"

enum StatType { Average, Variance };

typedef struct {
    enum StatType stat;
    int N;
} Config;

Config parse_args(int argc, char **argv) {
    Config config = { .stat = Average, .N = 1 };

    static struct option long_options[] = {
        { "stat", required_argument, 0, 's' },
        { "interval", required_argument, 0, 'i' },
        { 0, 0, 0, 0 }
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "s:i:", long_options, NULL)) != -1) {
        switch (opt) {
            case 's':
                if (strcmp(optarg, AVG) == 0) {
                    config.stat = Average;
                } else if (strcmp(optarg, VAR) == 0) {
                    config.stat = Variance;
                } else {
                    fprintf(stderr, "Unknown stat type '%s'\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'i':
                config.N = atoi(optarg);
                if (config.N <= 0) {
                    fprintf(stderr, "Interval must be a positive integer\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [--stat avg|var] [--interval N]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return config;
}

double compute_average(int *data, int length) {
    double sum = 0.0;
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    return sum / length;
}

double compute_variance(int *data, int length, double mean) {
    double sum = 0.0;
    for (int i = 0; i < length; i++) {
        double diff = data[i] - mean;
        sum += diff * diff;
    }
    return sum / length;
}

int main(int argc, char **argv) {
    Config config = parse_args(argc, argv);

    int *data = malloc(config.N * sizeof(int));
    if (!data) {
        fprintf(stderr, "Failed to allocate memory for data\n");
        return EXIT_FAILURE;
    }

    int count = 0;
    char line[100];  // Adjust the buffer size as needed
    while (fgets(line, sizeof(line), stdin)) {
        // Check if the line contains a numeric value
        int value;
        if (sscanf(line, "%*[^0-9]%d", &value) == 1) {
            data[count] = value;
            count++;
            if (count == config.N) {
                double avg = compute_average(data, count);
                if (config.stat == Average) {
                    printf("%.4f\n", avg);
                } else {
                    printf("%.4f\n", compute_variance(data, count, avg));
                }
                count = 0;
            }
        }
    }

    free(data);

    return EXIT_SUCCESS;
}

