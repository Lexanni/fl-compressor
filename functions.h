#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <inttypes.h>

struct ImageMetadata {
    int sizeX;
    int sizeY;
    int sizeZ;
    int D;
};

struct PredictorMetadata {
    int P;
    int R;
    int Om;
    int t_inc;
    int v_min;
    int v_max;
};

struct EncoderMetadata {
    unsigned unaryLengthLimit : 5;
    unsigned rescalingCounterSize : 3;
    unsigned initialCountExponent : 3;
    unsigned accumInitConstant : 4;
    unsigned accumInitTableFlag : 1;
};

int getLocalSum(int * currentBand, int sizeY, int sizeX, int y, int x);
int clip(int val, int val_min, int val_max);
int sgn_plus(int val);
int mod_R(int x, int R);
int d(int * currentBand, int sizeY, int sizeX, int y, int x);
int dN(int * currentBand, int sizeY, int sizeX, int y, int x);
int dW(int * currentBand, int sizeY, int sizeX, int y, int x);
int dNW(int * currentBand, int sizeY, int sizeX, int y, int x);
void getU(int * U, int P, int * image, int sizeY, int sizeX, int z, int y, int x);
void weightInitDefault(int * W, int om, int P);
int getPredictedD(int * U, int * W, int size);
int getScalingExp(int D, int Om, int v_min, int v_max, int t, int t_inc, int Nx);
int getMappedPredictionResidual(int s, int scale_s_pred, int s_min, int s_max);
void updateW(int * W, int * U, int size, int e, int ro, int w_min, int w_max);
void fun(int * in, int * out, ImageMetadata * imageMeta, PredictorMetadata * predMeta,
         int s_min, int s_max, int s_mid);
size_t getAccum(size_t prevAccum, size_t prevCounter, uint32_t prevResidual, uint32_t gamma);
size_t getCounter(size_t prevCounter, unsigned gamma);
unsigned getCodeWordSize(size_t counter, size_t accum);
void encodeGolomb(uint32_t *in, uint32_t *out, size_t * outSize, ImageMetadata * imageMeta, EncoderMetadata * encoderMeta);
void decodeGolomb(void * in, void * out, ImageMetadata * imageMeta, EncoderMetadata * encoderMeta);

#endif // FUNCTIONS_H
