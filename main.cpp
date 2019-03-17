#include <stdio.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

//int main()
//{
//    printf("Hello World!\n");
//    return 0;
//}

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

int main(int argc, char* argv[])
{

    int result = Catch::Session().run(argc, argv);

    return result;
}
