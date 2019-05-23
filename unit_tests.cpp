#ifdef UNIT_TESTS

#include "catch.hpp"
#include "functions.h"

TEST_CASE( "getLocalSum", "[local_sum]" ) {
    uint16_t ms[] = {
        7, 	2, 	6, 	7, 	5, 	1, 	8,
        5, 	4, 	9, 	9, 	9, 	6, 	3,
        5, 	2, 	6, 	5, 	6, 	1, 	7,
        2, 	8, 	8, 	8, 	7, 	6, 	5,
        8, 	1, 	9, 	7, 	3, 	9, 	9
    };
    int sizeX = 7;
    int sizeY = 5;
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 0, 0) == -1 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 8, 1) == -1 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 3, 9) == -1 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 0, 1) == 28 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 1, 0) == 18 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 1, 1) == 20 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 0, 6) == 4  );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 2, 3) == 33 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 4, 6) == 25 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 4, 0) == 20 );
    REQUIRE ( getLocalSum(ms, sizeY, sizeX, 2, 6) == 13 );
}

TEST_CASE("clip", "[clip]" ) {
    REQUIRE ( clip(9, 1, 8) == 8 );
    REQUIRE ( clip(5, 1, 5) == 5 );
    REQUIRE ( clip(4, 1, 5) == 4 );
    REQUIRE ( clip(1, 1, 5) == 1 );
    REQUIRE ( clip(0, 1, 5) == 1 );
}

TEST_CASE("sgn_plus", "[sgn_plus]") {
    REQUIRE( sgn_plus( 1) ==  1);
    REQUIRE( sgn_plus( 0) ==  1);
    REQUIRE( sgn_plus(-2) == -1);
    REQUIRE( sgn_plus( 3) ==  1);
    REQUIRE( sgn_plus(-1) == -1);
}

TEST_CASE("mod_R", "[mod_R]") {
    REQUIRE( mod_R(5, 32) ==   5);
    REQUIRE( mod_R(10, 5) ==  10);
    REQUIRE( mod_R(20, 5) == -12);
    REQUIRE( mod_R(16, 5) == -16);
    REQUIRE( mod_R(0 , 5) ==   0);
}

TEST_CASE("d", "[d]") {
    uint16_t ms[] = {
        7, 	2, 	6, 	7, 	5, 	1, 	8,
        5, 	4, 	9, 	9, 	9, 	6, 	3,
        5, 	2, 	6, 	5, 	6, 	1, 	7,
        2, 	8, 	8, 	8, 	7, 	6, 	5,
        8, 	1, 	9, 	7, 	3, 	9, 	9
    };
    int sizeX = 7;
    int sizeY = 5;
    REQUIRE (d(ms, sizeY, sizeX, 1, 1) ==  -4);
    REQUIRE (d(ms, sizeY, sizeX, 0, 1) == -20);
    REQUIRE (d(ms, sizeY, sizeX, 1, 0) ==   2);
    REQUIRE (d(ms, sizeY, sizeX, 4, 6) ==  11);
}

TEST_CASE("dN", "[dN]") {
    uint16_t ms[] = {
        7, 	2, 	6, 	7, 	5, 	1, 	8,
        5, 	4, 	9, 	9, 	9, 	6, 	3,
        5, 	2, 	6, 	5, 	6, 	1, 	7,
        2, 	8, 	8, 	8, 	7, 	6, 	5,
        8, 	1, 	9, 	7, 	3, 	9, 	9
    };
    int sizeX = 7;
    int sizeY = 5;
    REQUIRE (dN(ms, sizeY, sizeX, 1, 1) == -12);
    REQUIRE (dN(ms, sizeY, sizeX, 0, 1) ==   0);
    REQUIRE (dN(ms, sizeY, sizeX, 1, 0) ==  10);
    REQUIRE (dN(ms, sizeY, sizeX, 4, 6) ==  -5);
}

TEST_CASE("dW", "[dW]") {
    uint16_t ms[] = {
        7, 	2, 	6, 	7, 	5, 	1, 	8,
        5, 	4, 	9, 	9, 	9, 	6, 	3,
        5, 	2, 	6, 	5, 	6, 	1, 	7,
        2, 	8, 	8, 	8, 	7, 	6, 	5,
        8, 	1, 	9, 	7, 	3, 	9, 	9
    };
    int sizeX = 7;
    int sizeY = 5;
    REQUIRE (dW(ms, sizeY, sizeX, 1, 1) ==   0);
    REQUIRE (dW(ms, sizeY, sizeX, 0, 1) ==   0);
    REQUIRE (dW(ms, sizeY, sizeX, 1, 0) ==  10);
    REQUIRE (dW(ms, sizeY, sizeX, 4, 6) ==  11);
}

TEST_CASE("dNW", "[dNW]") {
    uint16_t ms[] = {
        7, 	2, 	6, 	7, 	5, 	1, 	8,
        5, 	4, 	9, 	9, 	9, 	6, 	3,
        5, 	2, 	6, 	5, 	6, 	1, 	7,
        2, 	8, 	8, 	8, 	7, 	6, 	5,
        8, 	1, 	9, 	7, 	3, 	9, 	9
    };
    int sizeX = 7;
    int sizeY = 5;
    REQUIRE (dNW(ms, sizeY, sizeX, 1, 1) ==   8);
    REQUIRE (dNW(ms, sizeY, sizeX, 0, 1) ==   0);
    REQUIRE (dNW(ms, sizeY, sizeX, 1, 0) ==  10);
    REQUIRE (dNW(ms, sizeY, sizeX, 4, 6) ==  -1);
}

TEST_CASE("getU", "[getU]") {
    uint16_t ms[4][15] = {
        {
            2, 	8, 	7, 	3, 	9,
            3, 	4, 	3, 	7, 	5,
            5, 	6, 	7, 	3, 	3
        },{
            2, 	7, 	2, 	1, 	9,
            2, 	6, 	9, 	5, 	9,
            7, 	5, 	2, 	5, 	7
        },{
            7, 	7, 	3, 	5, 	2,
            6, 	2, 	1, 	5, 	3,
            2, 	9, 	3, 	2, 	8
        },{
            6, 	3, 	4, 	3, 	3,
            6, 	1, 	6, 	4, 	7,
            1, 	5, 	3, 	3, 	5
        }
    };
    int sizeX = 5;
    int sizeY = 3;
    int z = 3;
    int y = 1;
    int x = 1;
    int P = 3;
    int U[10];

    getU(U, P, ms[0], sizeY, sizeX, z, y, x);
    int U_1[6] = {-7, 5, 5, -15, 11, -4};

    for(int i = 0; i < 6; i++) {
        INFO("Index: " << i);
        CHECK(U[i] == U_1[i]);
    }

    z = 1;
    y = 1;
    x = 1;
    P = 3;

    getU(U, P, ms[0], sizeY, sizeX, z, y, x);
    int U_2[6] = {15, -5, -5, -4, -4, -4};

    for(int i = 0; i < 6; i++) {
        INFO("Index: " << i);
        CHECK(U[i] == U_2[i]);
    }

    z = 0;
    y = 0;
    x = 1;
    P = 2;

    getU(U, P, ms[0], sizeY, sizeX, z, y, x);
    int U_3[5] = {0, 0, 0, 0, 0};

    for(int i = 0; i < 5; i++) {
        INFO("Index: " << i);
        CHECK(U[i] == U_3[i]);
    }
}


TEST_CASE("weightInitDefault", "[weightInitDefault]") {
    int W[10];

    int W_1[6] = {0, 0, 0, 14, 1, 0};
    weightInitDefault(W, 4, 3);
    for(int i = 0; i < 6; i++) {
        INFO("Index: " << i);
        CHECK(W[i] == W_1[i]);
    }

    int W_2[6] = {0, 0, 0, 224, 28, 3};
    weightInitDefault(W, 8, 3);
    for(int i = 0; i < 6; i++) {
        INFO("Index: " << i);
        CHECK(W[i] == W_2[i]);
    }
}

TEST_CASE("getPredictedD") {
    int U[] = {
        7, 	2, 	6, 	7, 	5, 	1, 	8,
        5, 	4, 	9, 	9, 	9, 	6, 	3
    };
    int W[] = {
        5, 	2, 	6, 	5, 	6, 	1, 	7,
        2, 	8, 	8, 	8, 	7, 	6, 	5
    };
    REQUIRE(getPredictedD(U, W, 1) == 35);
    REQUIRE(getPredictedD(U, W, 2) == 39);
    REQUIRE(getPredictedD(U, W, 3) == 75);
}

TEST_CASE("getMappedPredictionResidual") {
    CHECK(getMappedPredictionResidual( 9,   8, 0, 63) == 9);
    CHECK(getMappedPredictionResidual( 9,  17, 0, 63) == 1);
    CHECK(getMappedPredictionResidual( 9,  22, 0, 63) == 3);
    CHECK(getMappedPredictionResidual( 8,   3, 0, 63) == 8);
    CHECK(getMappedPredictionResidual( 4,   0, 0, 63) == 4);
    CHECK(getMappedPredictionResidual( 13,  5, 0, 63) == 13);
    CHECK(getMappedPredictionResidual( 8,  31, 0, 63) == 14);
    CHECK(getMappedPredictionResidual( 7,  10, 0, 63) == 4);
    CHECK(getMappedPredictionResidual(11,  40, 0, 63) == 17);
}

TEST_CASE("getRestoredValue") {
    CHECK(getRestoredValue( 9,  8, 0, 63, 32) == 9);
    CHECK(getRestoredValue( 1, 17, 0, 63, 32) == 9);
    CHECK(getRestoredValue( 3, 22, 0, 63, 32) == 9);
    CHECK(getRestoredValue( 4,  0, 0, 63, 32) == 4);
    CHECK(getRestoredValue( 5,  5, 0, 63, 32) == 5);
    CHECK(getRestoredValue( 4, 10, 0, 63, 32) == 7);
    CHECK(getRestoredValue(17, 40, 0, 63, 32) == 11);
    CHECK(getRestoredValue(13,  5, 0, 63, 32) == 13);
    CHECK(getRestoredValue(14, 31, 0, 63, 32) == 8);
}

TEST_CASE("updateW") {
    int U_1[3] = {5, 10, 15};
    int W_1[3] = {3, 1, 2};

    int size = 3;
    int e = 1;
    int ro = 1;
    int w_min = 0;
    int w_max = 4;

    int W_1_std[3] = {4, 4, 4};

    updateW(W_1, U_1, size, e, ro, w_min, w_max);

    for(int i = 0; i < size; i++) {
        INFO("Index: " << i);
        CHECK(W_1[i] == W_1_std[i]);
    }

    int U_2[3] = {5, 10, 15};
    int W_2[3] = {3, 1, 2};

    e = 3;
    ro = 2;
    w_min = -8;
    w_max = 8;

    int W_2_std[3] = {4, 2, 4};

    updateW(W_2, U_2, size, e, ro, w_min, w_max);

    for(int i = 0; i < size; i++) {
        INFO("Index: " << i);
        CHECK(W_2[i] == W_2_std[i]);
    }

    int U_3[3] = {-2, 1, -3};
    int W_3[3] = {-1, 0,  3};

    e = -3;
    ro = -1;
    w_min = -8;
    w_max = 8;

    int W_3_std[3] = {1, 0, 6};

    updateW(W_3, U_3, size, e, ro, w_min, w_max);

    for(int i = 0; i < size; i++) {
        INFO("Index: " << i);
        CHECK(W_3[i] == W_3_std[i]);
    }
}

TEST_CASE("runPredictor") {
    uint16_t input[12 * 4 * 5] = {
        100, 	101, 	100, 	100, 	101,
        100, 	102, 	101, 	101, 	101,
        102, 	100, 	100, 	100, 	103,
        102, 	100, 	101, 	100, 	102,

        100, 	104, 	100, 	102, 	100,
        103, 	103, 	100, 	101, 	103,
        102, 	102, 	100, 	100, 	100,
        102, 	101, 	101, 	100, 	102,

        102, 	100, 	101, 	101, 	101,
        102, 	101, 	101, 	100, 	101,
        102, 	100, 	105, 	100, 	100,
        101, 	100, 	100, 	100, 	100,

        100, 	101, 	101, 	100, 	101,
        100, 	102, 	101, 	101, 	101,
        102, 	100, 	100, 	100, 	103,
        102, 	100, 	101, 	100, 	102,

        100, 	104, 	100, 	102, 	100,
        103, 	103, 	100, 	101, 	103,
        102, 	102, 	100, 	100, 	100,
        102, 	101, 	101, 	100, 	102,

        102, 	100, 	101, 	101, 	101,
        102, 	101, 	101, 	100, 	101,
        102, 	100, 	105, 	100, 	100,
        101, 	100, 	100, 	100, 	100,

        100, 	101, 	101, 	100, 	101,
        100, 	102, 	101, 	101, 	101,
        102, 	100, 	100, 	100, 	103,
        102, 	100, 	101, 	100, 	102,

        100, 	104, 	100, 	102, 	100,
        103, 	103, 	100, 	101, 	103,
        102, 	102, 	100, 	100, 	100,
        102, 	101, 	101, 	100, 	102,

        102, 	100, 	101, 	101, 	101,
        102, 	101, 	101, 	100, 	101,
        102, 	100, 	105, 	100, 	100,
        101, 	100, 	100, 	100, 	100,

        100, 	101, 	101, 	100, 	101,
        100, 	102, 	101, 	101, 	101,
        102, 	100, 	100, 	100, 	103,
        102, 	100, 	101, 	100, 	102,

        100, 	104, 	100, 	102, 	100,
        103, 	103, 	100, 	101, 	103,
        102, 	102, 	100, 	100, 	100,
        102, 	101, 	101, 	100, 	102,

        102, 	100, 	101, 	101, 	101,
        102, 	101, 	101, 	100, 	101,
        102, 	100, 	105, 	100, 	100,
        101, 	100, 	100, 	100, 	100
    };

    uint16_t mappedResiduals[12 * 4 * 5] = {0};
    uint16_t restoredData[12 * 4 * 5] = {0};

    ImageMetadata imageMeta;
    imageMeta.dynamicRange = 0;
    imageMeta.xSize = 5;
    imageMeta.ySize = 4;
    imageMeta.zSize = 12;

    PredictorMetadata predMeta;
    predMeta.predictionBands = 2;
    predMeta.predictionMode = 0;
    predMeta.localSumType = 0;
    predMeta.registerSize = 32;
    predMeta.weightComponentResolution = 0;
    predMeta.wuScalingExpChangeInterval = 0;
    predMeta.wuScalingExpInitialParameter = 0;
    predMeta.wuScalingExpFinalParameter = 15;
    predMeta.weightInitMethod = 0;
    predMeta.weightInitTableFlag = 0;
    predMeta.weightInitResolution = 0;

    runPredictor(input, mappedResiduals, &imageMeta, &predMeta, PREDICTOR_MAP);
    runPredictor(mappedResiduals, restoredData, &imageMeta, &predMeta, PREDICTOR_RESTORE);

    int size = 12 * 4 * 5;

    for(int i = 0; i < size; i++) {
        INFO("Index: " << i);
        CHECK(input[i] == restoredData[i]);
    }

}

TEST_CASE("getCodeWordSize") {
    REQUIRE(getCodeWordSize(5, 6)     == 0);
    REQUIRE(getCodeWordSize(16, 35)   == 1);
    REQUIRE(getCodeWordSize(80, 1120) == 3);
    REQUIRE(getCodeWordSize(15, 10)   == 0);
    REQUIRE(getCodeWordSize(12, 856)  == 6);
}

TEST_CASE("Golomb") {

    uint16_t input[3 * 4 * 5] = {
        1, 	1, 	0, 	0, 	1,
        0, 	2, 	1, 	1, 	1,
        2, 	0, 	0, 	0, 	3,
        2, 	0, 	1, 	0, 	2,

        0, 	4, 	0, 	2, 	0,
        3, 	3, 	0, 	1, 	3,
        2, 	2, 	0, 	0, 	0,
        2, 	1, 	1, 	0, 	2,

        2, 	0, 	1, 	1, 	1,
        2, 	1, 	1, 	0, 	1,
        2, 	0, 	5, 	0, 	0,
        1, 	0, 	0, 	0, 	0
    };

    uint32_t encodedOut[3 * 4 * 5] = {0};
    uint16_t decodedOut[3 * 4 * 5] = {0};
    size_t outSize = 0;

    ImageMetadata imageMeta;
    imageMeta.dynamicRange = 0;
    imageMeta.xSize = 5;
    imageMeta.ySize = 4;
    imageMeta.zSize = 3;

    EncoderMetadata encoderMeta;
    encoderMeta.unaryLengthLimit = 8;
    encoderMeta.accumInitConstant = 2;
    encoderMeta.initialCountExponent = 1;
    encoderMeta.rescalingCounterSize = 1;

    encodeGolomb(input,encodedOut, &outSize, &imageMeta, &encoderMeta);
    decodeGolomb(encodedOut, decodedOut, &imageMeta, &encoderMeta);

    int size = 3 * 4 * 5;

    for(int i = 0; i < size; i++) {
        INFO("Index: " << i);
        CHECK(input[i] == decodedOut[i]);
    }

}

TEST_CASE("save/load PGM") {

    uint16_t data[6 * 7] = {
        215, 	2447, 	3842, 	1083, 	119, 	672, 	1782,
        2406, 	3397, 	2431, 	2823, 	1154, 	1462, 	3492,
        898, 	2892, 	1144, 	2849, 	2422, 	3584, 	1153,
        1287, 	1389, 	1479, 	3597, 	2460, 	2089, 	2111,
        589, 	2685, 	140, 	3056, 	1198, 	2115, 	656,
        2311, 	2867, 	1166, 	386, 	1462, 	2163, 	3300
    };
    uint16_t * loadedData = NULL;

    char fileName[] = "test.pgm";
    unsigned sizeX = 7;
    unsigned sizeY = 6;
    unsigned maxValue = 4095;
    unsigned loadedSizeX;
    unsigned loadedSizeY;
    unsigned loadedMaxValue;

    CHECK(saveToPGM(fileName, data, sizeX, sizeY, maxValue) == 0);
    CHECK(loadFromPGM(fileName, &loadedData, &loadedSizeX, &loadedSizeY, &loadedMaxValue) == 0);

    if(loadedData) {
        int size = sizeX * sizeY;
        for(int i = 0; i < size; i++) {
            INFO("Index: " << i);
            CHECK(data[i] == loadedData[i]);
        }
        free(loadedData);
    }
}

TEST_CASE("save/load compressed image") {

    char fileName[] = "test_compressed.fl";
    uint32_t data[6 * 5] = {
        581612148,      840150402,      170625740,      1768229585, 	4127652133,
        2166189274, 	921143472,      2829598208, 	4086598280, 	1337180266,
        2599861949, 	1672172116, 	855853699,      2135216968, 	3721482342,
        2363560580, 	1555567274, 	3700090308, 	974886365,      3518892602,
        2865892643, 	3278841227, 	1314848842, 	2703874718, 	229157794,
        4130334434, 	119450082,      794094992,      756900839,      635552470
    };
    uint32_t * loadedData = NULL;
    ImageMetadata l_imageMeta;
    PredictorMetadata l_predMeta;
    EncoderMetadata l_encoderMeta;

    size_t size = 6 * 5;
    size_t dataSize = size * sizeof (uint32_t);
    size_t loadDadaSize = 0;

    ImageMetadata imageMeta;
    imageMeta.userData     = 127; // :  8;
    imageMeta.xSize        = 500; // : 16;
    imageMeta.ySize        = 800; // : 16;
    imageMeta.zSize        = 200; // : 16;
    imageMeta.sampleType   = 0;   // : 1;
    imageMeta.reserved_0   = 3;   // : 2;
    imageMeta.dynamicRange = 8;   // : 4;
    imageMeta.sampleEncodingOrder       = 0;     //  : 1;
    imageMeta.subFrameInterleavingDepth = 12000; // : 16;
    imageMeta.reserved_1       = 2; // : 2;
    imageMeta.outputWordSize   = 5; // : 3;
    imageMeta.entropyCoderType = 0; // : 1;
    imageMeta.reserved_2       = 0; // : 10;

    PredictorMetadata predMeta;
    predMeta.reserved_0      = 3; // : 2;
    predMeta.predictionBands = 5; // : 4;
    predMeta.predictionMode  = 0; // : 1;
    predMeta.reserved_1      = 1; // : 1;
    predMeta.localSumType    = 0; // : 1;
    predMeta.reserved_2      = 0; // : 1;
    predMeta.registerSize    = 3; // : 6;
    predMeta.weightComponentResolution    = 10; // : 4;
    predMeta.wuScalingExpChangeInterval   = 6;  // : 4;
    predMeta.wuScalingExpInitialParameter = 0;  // : 4;
    predMeta.wuScalingExpFinalParameter   = 12; // : 4;
    predMeta.reserved_3           = 0;  // : 1;
    predMeta.weightInitMethod     = 0;  // : 1;
    predMeta.weightInitTableFlag  = 0;  // : 1;
    predMeta.weightInitResolution = 20; // : 5;

    EncoderMetadata encoderMeta;
    encoderMeta.unaryLengthLimit     = 7;  // : 5;
    encoderMeta.rescalingCounterSize = 2;  // : 3;
    encoderMeta.initialCountExponent = 6;  // : 3;
    encoderMeta.accumInitConstant    = 11; // : 4;
    encoderMeta.accumInitTableFlag   = 0;  // : 1;

    CHECK(saveCompressedImage(fileName, data, dataSize, &imageMeta, &predMeta, &encoderMeta) == 0);

    memset(&l_imageMeta, 0, sizeof (ImageMetadata));
    memset(&l_predMeta,  0, sizeof (PredictorMetadata));
    memset(&l_encoderMeta, 0, sizeof (EncoderMetadata));

    CHECK(loadCompressedImage(fileName, (void **)&loadedData, &loadDadaSize,
                              &l_imageMeta, &l_predMeta, &l_encoderMeta) == 0);

    CHECK(memcmp(&imageMeta, &l_imageMeta, sizeof (ImageMetadata)) == 0);
    CHECK(memcmp(&predMeta,  &l_predMeta, sizeof (PredictorMetadata)) == 0);
    CHECK(memcmp(&encoderMeta, &l_encoderMeta, sizeof (EncoderMetadata)) == 0);

    if(loadedData) {
        for(int i = 0; i < size; i++) {
            INFO("Index: " << i);
            CHECK(data[i] == loadedData[i]);
        }
        free(loadedData);
    }

}

#endif // UNIT_TESTS
