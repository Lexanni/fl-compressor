#include <stdio.h>
#include <math.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

using namespace std;

int getLocalSum(int * currentBand, int sizeY, int sizeX, int y, int x) {
    int sNW, sN, sNE, sW;
    if (sizeX <= 0 || sizeY <= 0)
        return -1;
    if (y > 0 && y < sizeY) {
        if (x > 0 && x < sizeX - 1) {
            sNW = currentBand[sizeX * (y - 1) + (x - 1)];
            sN  = currentBand[sizeX * (y - 1) +    x   ];
            sNE = currentBand[sizeX * (y - 1) + (x + 1)];
            sW  = currentBand[sizeX *    y    + (x - 1)];
            return sNW + sN + sNE + sW;
        } else if (x == 0) {
            sN  = currentBand[sizeX * (y - 1) +    x   ];
            sNE = currentBand[sizeX * (y - 1) + (x + 1)];
            return (sN + sNE) << 1;
        } else if (x == sizeX - 1) {
            sNW = currentBand[sizeX * (y - 1) + (x - 1)];
            sN  = currentBand[sizeX * (y - 1) +    x   ];
            sW  = currentBand[sizeX *    y    + (x - 1)];
            return sW + sNW + (sN << 1);
        }
    } else if ( y == 0 && x > 0 && x < sizeX) {
            return currentBand[x - 1] << 2;
    }
    return -1;
}

TEST_CASE( "getLocalSum", "[local_sum]" ) {
    int ms[] = {
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

int clip(int val, int val_min, int val_max) {
    return min(val_max, max(val_min, val));
}

TEST_CASE("clip", "[clip]" ) {
    REQUIRE ( clip(9, 1, 8) == 8 );
    REQUIRE ( clip(5, 1, 5) == 5 );
    REQUIRE ( clip(4, 1, 5) == 4 );
    REQUIRE ( clip(1, 1, 5) == 1 );
    REQUIRE ( clip(0, 1, 5) == 1 );
}

int sgn_plus(int val) {
    return val < 0 ? -1 : 1;
}

TEST_CASE("sgn_plus", "[sgn_plus]") {
    REQUIRE( sgn_plus( 1) ==  1);
    REQUIRE( sgn_plus( 0) ==  1);
    REQUIRE( sgn_plus(-2) == -1);
    REQUIRE( sgn_plus( 3) ==  1);
    REQUIRE( sgn_plus(-1) == -1);
}

int mod_R(int x, int R) {
    return ((x + (1 << (R - 1))) % (1 << R)) - (1 << (R - 1));
}

TEST_CASE("mod_R", "[mod_R]") {
    REQUIRE( mod_R(10, 5) ==  10);
    REQUIRE( mod_R(20, 5) == -12);
    REQUIRE( mod_R(16, 5) == -16);
    REQUIRE( mod_R(0 , 5) ==   0);
}

int d(int * currentBand, int sizeY, int sizeX, int y, int x) {
    return (currentBand[sizeX * y + x] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
}

TEST_CASE("d", "[d]") {
    int ms[] = {
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

int dN(int * currentBand, int sizeY, int sizeX, int y, int x) {
    if(y == 0)
        return 0;
    return (currentBand[sizeX * (y - 1) + x] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
}

TEST_CASE("dN", "[dN]") {
    int ms[] = {
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

int dW(int * currentBand, int sizeY, int sizeX, int y, int x) {
    if(y == 0)
        return 0;
    if(x == 0)
        return (currentBand[sizeX * (y - 1) + x] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
    return (currentBand[sizeX * y + (x - 1)] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
}

TEST_CASE("dW", "[dW]") {
    int ms[] = {
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

int dNW(int * currentBand, int sizeY, int sizeX, int y, int x) {
    if(y == 0)
        return 0;
    if(x == 0)
        return (currentBand[sizeX * (y - 1) + x] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
    return (currentBand[sizeX * (y - 1) + (x - 1)] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
}

TEST_CASE("dNW", "[dNW]") {
    int ms[] = {
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

void getU(int * U, int P, int * image, int sizeY, int sizeX, int z, int y, int x) {
    int bandSize = sizeY * sizeX;
    int * currentBand = image + bandSize * z;

    U[0] =  dN(currentBand, sizeY, sizeX, y, x);
    U[1] =  dW(currentBand, sizeY, sizeX, y, x);
    U[2] = dNW(currentBand, sizeY, sizeX, y, x);

    for(int i = 1; i <= P; i++) {
        int stepsDown = z - i > 0 ? i : z;
        U[i + 2] = d(currentBand - bandSize * stepsDown, sizeY, sizeX, y, x);
    }
}

TEST_CASE("getU", "[getU]") {
    int ms[4][15] = {
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
        INFO("The number is " << i);
        CHECK(U[i] == U_1[i]);
    }

    getU(U, P, ms[0], sizeY, sizeX, 1, 1, 1);
    int U_2[6] = {15, -5, -5, -4, -4, -4};

    for(int i = 0; i < 6; i++) {
        INFO("The number is " << i);
        CHECK(U[i] == U_2[i]);
    }
}

int main(int argc, char* argv[])
{

    int result = Catch::Session().run(argc, argv);

    return result;
}
