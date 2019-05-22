#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <inttypes.h>

struct ImageMetadata {
    unsigned userData     : 8;
    unsigned xSize        : 16;
    unsigned ySize        : 16;
    unsigned zSize        : 16;
    unsigned sampleType   : 1;
    unsigned reserved_0   : 2;
    unsigned dynamicRange : 4;
    unsigned sampleEncodingOrder       : 1;
    unsigned subFrameInterleavingDepth : 16;
    unsigned reserved_1       : 2;
    unsigned outputWordSize   : 3;
    unsigned entropyCoderType : 1;
    unsigned reserved_2       : 10;
};

struct PredictorMetadata {
    unsigned reserved_0      : 2;
    unsigned predictionBands : 4;
    unsigned predictionMode  : 1;
    unsigned reserved_1      : 1;
    unsigned localSumType    : 1;
    unsigned reserved_2      : 1;
    unsigned registerSize    : 6;
    unsigned weightComponentResolution    : 4;
    unsigned wuScalingExpChangeInterval   : 4;
    unsigned wuScalingExpInitialParameter : 4;
    unsigned wuScalingExpFinalParameter   : 4;
    unsigned reserved_3           : 1;
    unsigned weightInitMethod     : 1;
    unsigned weightInitTableFlag  : 1;
    unsigned weightInitResolution : 5;
};

struct EncoderMetadata {
    unsigned unaryLengthLimit : 5;
    unsigned rescalingCounterSize : 3;
    unsigned initialCountExponent : 3;
    unsigned accumInitConstant : 4;
    unsigned accumInitTableFlag : 1;
};

enum {
    PREDICTOR_MAP,
    PREDICTOR_RESTORE
};

int getLocalSum(uint16_t * currentBand, int sizeY, int sizeX, int y, int x);
int clip(int val, int val_min, int val_max);
int sgn_plus(int val);
int mod_R(int x, int R);
int d(uint16_t *currentBand, int sizeY, int sizeX, int y, int x);
int dN(uint16_t *currentBand, int sizeY, int sizeX, int y, int x);
int dW(uint16_t *currentBand, int sizeY, int sizeX, int y, int x);
int dNW(uint16_t * currentBand, int sizeY, int sizeX, int y, int x);
void getU(int * U, int P, uint16_t *image, int sizeY, int sizeX, int z, int y, int x);
void weightInitDefault(int * W, int om, int P);
int getPredictedD(int * U, int * W, int size);
int getScalingExp(int D, int Om, int v_min, int v_max, int t, int t_inc, int Nx);
uint16_t getMappedPredictionResidual(int s, int scale_s_pred, int s_min, int s_max);
uint16_t getRestoredValue(int mappedResidual, int scale_s_pred, int s_min, int s_max, int s_mid);
void updateW(int * W, int * U, int size, int e, int ro, int w_min, int w_max);
void runPredictor(uint16_t *in, uint16_t *out, ImageMetadata * imageMeta, PredictorMetadata * predMeta, int opType);
size_t getAccum(size_t prevAccum, size_t prevCounter, uint32_t prevResidual, uint32_t gamma);
size_t getCounter(size_t prevCounter, unsigned gamma);
unsigned getCodeWordSize(size_t counter, size_t accum);
void encodeGolomb(uint16_t * in, uint32_t * out, size_t * outSize, ImageMetadata * imageMeta, EncoderMetadata * encoderMeta);
void decodeGolomb(uint32_t * in, uint16_t * out, ImageMetadata * imageMeta, EncoderMetadata * encoderMeta);
int loadFromPGM(char *fileName, uint16_t *data[], unsigned * sizeX, unsigned * sizeY, unsigned * maxValue);
int saveToPGM(char *fileName, uint16_t data[], unsigned sizeX, unsigned sizeY, unsigned maxValue);
void swopBytes(uint16_t * p, size_t size);

#endif // FUNCTIONS_H
