#include "fl_compressor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_FILES 500
#define MAX_FILE_NAME_LENGHT 128

int main(int argc, char* argv[])
{
    struct ImageMetadata imageMeta;
    memset(&imageMeta, 0, sizeof (struct ImageMetadata));
    imageMeta.userData     = 0;   // :  8;
    imageMeta.xSize        = 0;   // : 16;
    imageMeta.ySize        = 0;   // : 16;
    imageMeta.zSize        = 0;   // : 16;
    imageMeta.sampleType   = 0;   // : 1;
    imageMeta.reserved_0   = 0;   // : 2;
    imageMeta.dynamicRange = 0;   // : 4;
    imageMeta.sampleEncodingOrder       = 0; //  : 1;
    imageMeta.subFrameInterleavingDepth = 0; // : 16;
    imageMeta.reserved_1       = 0; // : 2;
    imageMeta.outputWordSize   = 0; // : 3;
    imageMeta.entropyCoderType = 0; // : 1;
    imageMeta.reserved_2       = 0; // : 10;

    struct PredictorMetadata predMeta;
    memset(&predMeta, 0, sizeof (struct PredictorMetadata));
    predMeta.reserved_0      = 0;  // : 2;
    predMeta.predictionBands = 2;  // : 4;
    predMeta.predictionMode  = 0;  // : 1;
    predMeta.reserved_1      = 0;  // : 1;
    predMeta.localSumType    = 0;  // : 1;
    predMeta.reserved_2      = 0;  // : 1;
    predMeta.registerSize    = 32; // : 6;
    predMeta.weightComponentResolution    = 0;  // : 4;
    predMeta.wuScalingExpChangeInterval   = 0;  // : 4;
    predMeta.wuScalingExpInitialParameter = 0;  // : 4;
    predMeta.wuScalingExpFinalParameter   = 15; // : 4;
    predMeta.reserved_3           = 0;  // : 1;
    predMeta.weightInitMethod     = 0;  // : 1;
    predMeta.weightInitTableFlag  = 0;  // : 1;
    predMeta.weightInitResolution = 0;  // : 5;

    struct EncoderMetadata encoderMeta;
    memset(&encoderMeta, 0, sizeof (struct EncoderMetadata));
    encoderMeta.unaryLengthLimit     = 8;  // : 5;
    encoderMeta.rescalingCounterSize = 1;  // : 3;
    encoderMeta.initialCountExponent = 1;  // : 3;
    encoderMeta.accumInitConstant    = 2;  // : 4;
    encoderMeta.accumInitTableFlag   = 0;  // : 1;

    int compress = 1;
    char outFileName[MAX_FILE_NAME_LENGHT] = "output";

    char * imageNames[MAX_FILES] = {NULL};
    unsigned imageCounter = 0;

    printf( "//\n"
            "// Fast Lossless Compressor version 0.1\n"
            "//\n");

    if (argc == 1) {
        printUsage();
        return -1;
    }
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "/?")) {
            printUsage();
            return -1;
        }
        else if (!strcmp(argv[i], "--compress")) {
            compress = 1;
        }
        else if (!strcmp(argv[i], "--decompress")) {
            compress = 0;
        }
        else if (!strcmp(argv[i], "--output")) {
            strncpy(outFileName, argv[i + 1], MAX_FILE_NAME_LENGHT - 1);
            i++;
        }
        else if (!strcmp(argv[i], "-U")) {
            int U = atoi(argv[i + 1]);
            if(U <= 0 || U > 20) {
                printf("Incorrect value of -U key (unary limit).\n"
                       "Correct: 0 < U <= 20\n");
                printUsage();
                return -1;
            }
            encoderMeta.unaryLengthLimit = (unsigned)U;
            i++;
        }
        else if (!strcmp(argv[i], "-K")) {
            int K = atoi(argv[i + 1]);
            if(K < 0 || K > 15) {
                printf("Incorrect value of -K key (accumulator init constant).\n"
                       "Correct: 0 <= K <= 15.\n");
                printUsage();
                return -1;
            }
            encoderMeta.accumInitConstant = (unsigned)K;
            i++;
        }
        else if (!strcmp(argv[i], "-G")) {
            int G = atoi(argv[i + 1]);
            if(G < 1 || G > 16) {
                printf("Incorrect value of -G key (init count exponent)).\n"
                       "Correct: 1 <= G <= 16.\n");
                printUsage();
                return -1;
            }
            encoderMeta.initialCountExponent = (unsigned)(G == 16 ? 0 : G);
            i++;
        }
        else if (!strcmp(argv[i], "-P")) {
            int P = atoi(argv[i + 1]);
            if(P < 0 || P > 15) {
                printf("Incorrect value of -P key \n"
                       "(count of previous band for prediction)).\n"
                       "Correct: 0 <= P <= 15.\n");
                printUsage();
                return -1;
            }
            predMeta.predictionBands = (unsigned)P;
            i++;
        }
        else if (!strcmp(argv[i], "-t_inc")) {
            int t_inc = atoi(argv[i + 1]);
            if(t_inc < 0 || t_inc > 15) {
                printf("Incorrect value of -t_inc key\n"
                       "(weight update scaling exponent change interval).\n"
                       "Correct: 0 <= t_inc <= 15.\n");
                printUsage();
                return -1;
            }
            predMeta.wuScalingExpChangeInterval = (unsigned)t_inc;
            i++;
        }
        else if (!strcmp(argv[i], "-nu_min")) {
            int nu_min = atoi(argv[i + 1]);
            if(nu_min < 0 || nu_min > 15) {
                printf("Incorrect value of -nu_min key\n"
                       "(weight update scaling exponent initial parameter).\n"
                       "Correct: 0 <= nu_min <= 15.\n");
                printUsage();
                return -1;
            }
            predMeta.wuScalingExpInitialParameter = (unsigned)nu_min;
            i++;
        }
        else if (!strcmp(argv[i], "-nu_max")) {
            int nu_max = atoi(argv[i + 1]);
            if(nu_max < 0 || nu_max > 15) {
                printf("Incorrect value of -nu_max key\n"
                       "(weight update scaling exponent final parameter).\n"
                       "Correct: 0 <= nu_max <= 15.\n");
                printUsage();
                return -1;
            }
            predMeta.wuScalingExpFinalParameter = (unsigned)nu_max;
            i++;
        }
        else if (!strcmp(argv[i], "-Om")) {
            int Om = atoi(argv[i + 1]);
            if(Om < 0 || Om > 15) {
                printf("Incorrect value of -Om key (weight component resolution)).\n"
                       "Correct: 0 <= Om <= 15.\n");
                printUsage();
                return -1;
            }
            predMeta.weightComponentResolution = (unsigned)Om;
            i++;
        }
        else {
            imageNames[imageCounter] = argv[i];
            imageCounter++;
        }
    }

    if(imageCounter == 0) {
        printUsage();
        return -1;
    }

    uint16_t * images[MAX_FILES] = {NULL};
    uint16_t * data = NULL;
    uint16_t * mappedResiduals = NULL;
    uint32_t * encodedData = NULL;

    unsigned loadCounter = 0;

    unsigned sizeX = 0;
    unsigned sizeY = 0;
    unsigned sizeZ = 0;
    unsigned maxValue = 0;

    struct timespec start, stop;

    if(compress) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        printf("Opening files...\n");
        for (unsigned i = 0; i < imageCounter; i++) {
            int res = loadFromPGM(imageNames[i], &images[loadCounter],
                                  &sizeX, &sizeY, &maxValue);
            if(res != 0) {
                printf("Cannot load %s file.\n", imageNames[i]);
                continue;
            }
            loadCounter++;
        }
        if(loadCounter == 0) {
            printf("No files has been uploaded.\nCompression failed.");
            return -1;
        }

        size_t bandSize = sizeX * sizeY * sizeof (uint16_t);
        size_t size =  loadCounter * bandSize;
        data = (uint16_t *)malloc(size);
        if(!data) {
            printf("Cannot allocate %lld MB from memory.\n", size >> 20);
            return -1;
        }

        for (unsigned i = 0; i < loadCounter; i++) {
            memcpy((uint8_t *)data + bandSize * i, images[i], bandSize);
            free(images[i]);
        }

        printf("Start compression %d files totaling %lld bytes (%.2lf MB)...\n",
               loadCounter, size, (double) size / (1 << 20));

        imageMeta.xSize = sizeX;
        imageMeta.ySize = sizeY;
        imageMeta.zSize = loadCounter;
        unsigned d = (unsigned)log2(maxValue + 1);
        imageMeta.dynamicRange = d == 16 ? 0 : d;

        mappedResiduals = (uint16_t *)malloc(size);
        if(!mappedResiduals) {
            printf("Cannot allocate %lld MB from memory.\n", size >> 20);
            return -1;
        }

        runPredictor(data, mappedResiduals, &imageMeta, &predMeta, PREDICTOR_MAP);
        free(data);
        encodedData = (uint32_t *)malloc(size);
        if(!encodedData) {
            printf("Cannot allocate %lld MB from memory.\n", size >> 20);
            return -1;
        }
        size_t outSize = 0;
        encodeGolomb(mappedResiduals, encodedData, &outSize, &imageMeta, &encoderMeta);
        free(mappedResiduals);
        saveCompressedImage(outFileName, encodedData, outSize, &imageMeta, &predMeta, &encoderMeta);
        free(encodedData);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        struct timespec elapsed_time;
        elapsed_time.tv_sec = stop.tv_sec - start.tv_sec;
        elapsed_time.tv_nsec = stop.tv_nsec - start.tv_nsec;

        printf("Compression done.\n");
        printf("Elapsed time is %d.%d seconds.\n", elapsed_time.tv_sec, elapsed_time.tv_nsec / 1000);

        outSize += sizeof (struct ImageMetadata) +
            sizeof (struct PredictorMetadata) +
            sizeof (struct EncoderMetadata);

        printf("Compressed file size is %lld bytes (%.2lf MB).\n",
               outSize, (double)outSize / (1 << 20));
        printf("Compression ratio is %.2lf%%.\n", (double)outSize * 100 / size);
    } else {
        for (unsigned i = 0; i < imageCounter; i++) {
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
            size_t dataSize = 0;

            printf("Starting decompression %s...\n", imageNames[i]);

            int res = loadCompressedImage(imageNames[i], (void **)&encodedData, &dataSize,
                                          &imageMeta, &predMeta, &encoderMeta);
            if(res != 0) {
                printf("Cannot load %s file.\n", imageNames[i]);
                continue;
            }
            sizeX = imageMeta.xSize;
            sizeY = imageMeta.ySize;
            sizeZ = imageMeta.zSize;
            maxValue = (1 << (imageMeta.dynamicRange == 0 ? 16 : imageMeta.dynamicRange)) - 1;

            size_t bandTotal = sizeX * sizeY;
            size_t size = sizeZ * bandTotal * sizeof (uint16_t);

            mappedResiduals = (uint16_t *)malloc(size);
            if(!mappedResiduals) {
                printf("Cannot allocate %lld MB from memory.\n", size >> 20);
                return -1;
            }

            decodeGolomb(encodedData, mappedResiduals, &imageMeta, &encoderMeta);
            free(encodedData);

            data = (uint16_t *)malloc(size);
            if(!data) {
                printf("Cannot allocate %lld MB from memory.\n", size >> 20);
                return -1;
            }

            runPredictor(mappedResiduals, data, &imageMeta, &predMeta, PREDICTOR_RESTORE);
            free(mappedResiduals);
            char buffer[1024] = {0};
            for(unsigned j = 0; j < sizeZ; j++) {
                sprintf(buffer, "%s%04d.pgm", outFileName, j);
                saveToPGM(buffer, data + bandTotal * j, sizeX, sizeY, maxValue);
            }
            free(data);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
            struct timespec elapsed_time;
            elapsed_time.tv_sec = stop.tv_sec - start.tv_sec;
            elapsed_time.tv_nsec = stop.tv_nsec - start.tv_nsec;

            printf("Decompression done.\n");
            printf("Decompressed %d files totaling %.2lf MB\n", sizeZ, (double)size / (1 << 20));
            printf("Elapsed time is %d.%d seconds.\n", elapsed_time.tv_sec, elapsed_time.tv_nsec / 1000);
        }
    }
    return 0;
}
