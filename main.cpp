#ifdef UNIT_TESTS

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#else

#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_FILES 500
#define MAX_FILE_NAME_LENGHT 128

int main(int argc, char* argv[])
{
    ImageMetadata imageMeta;
    memset(&imageMeta, 0, sizeof (ImageMetadata));
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

    PredictorMetadata predMeta;
    memset(&predMeta, 0, sizeof (PredictorMetadata));
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

    EncoderMetadata encoderMeta;
    memset(&encoderMeta, 0, sizeof (EncoderMetadata));
    encoderMeta.unaryLengthLimit     = 8;  // : 5;
    encoderMeta.rescalingCounterSize = 1;  // : 3;
    encoderMeta.initialCountExponent = 1;  // : 3;
    encoderMeta.accumInitConstant    = 2;  // : 4;
    encoderMeta.accumInitTableFlag   = 0;  // : 1;

    int compress = 1;
    char outFileName[MAX_FILE_NAME_LENGHT] = "output";

    char * imageNames[MAX_FILES] = {NULL};
    unsigned imageCounter = 0;

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
            strcpy(outFileName, argv[i + 1]);
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

    if(compress) {
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
            printf("No file has been uploaded.\n");
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

        imageMeta.xSize = sizeX;
        imageMeta.ySize = sizeY;
        imageMeta.zSize = loadCounter;
        unsigned d = log2(maxValue + 1);
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

        printf("Compress done.\n");
    } else {
        size_t dataSize = 0;
        for (unsigned i = 0; i < imageCounter; i++) {
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
            for(unsigned i = 0; i < sizeZ; i++) {
                sprintf(buffer, "%s_%d.pgm", outFileName, i);
                saveToPGM(buffer, data + bandTotal * i, sizeX, sizeY, maxValue);
            }
            free(data);
        }
        printf("Decompress done.\n");
    }

    return 0;
}

#endif // UNIT_TESTS
