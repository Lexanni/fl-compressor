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
    REQUIRE( mod_R(5, 32) == 5  );
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
    int P = 3;
    int U[10];

    getU(U, P, ms[0], sizeY, sizeX, 3, 1, 1);
    int U_1[6] = {-7, 5, 5, -15, 11, -4};

    for(int i = 0; i < 6; i++) {
        INFO("Index: " << i);
        CHECK(U[i] == U_1[i]);
    }

    getU(U, P, ms[0], sizeY, sizeX, 1, 1, 1);
    int U_2[6] = {15, -5, -5, -4, -4, -4};

    for(int i = 0; i < 6; i++) {
        INFO("Index: " << i);
        CHECK(U[i] == U_2[i]);
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
    CHECK(getMappedPredictionResidual( 5,  12, 0, 32) == 1);
    CHECK(getMappedPredictionResidual( 9,  17, 0, 32) == 1);
    CHECK(getMappedPredictionResidual( 9,  20, 0, 32) == 1);
    CHECK(getMappedPredictionResidual( 8,   3, 0, 32) == 13);
    CHECK(getMappedPredictionResidual( 4,   0, 0, 32) == 8);
    CHECK(getMappedPredictionResidual( 13,  3, 0, 32) == 23);
    CHECK(getMappedPredictionResidual( 7,   0, 0, 32) == 14);
    CHECK(getMappedPredictionResidual( 6,   2, 0, 32) == 10);
    CHECK(getMappedPredictionResidual( 5,   5, 0, 32) == 5);
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

TEST_CASE("fun") {
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

    uint16_t output[12 * 4 * 5] = {0};

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

    fun(input, output, &imageMeta, &predMeta);
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
