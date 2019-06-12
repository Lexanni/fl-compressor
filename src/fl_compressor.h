#ifndef FL_COMPRESSOR_H
#define FL_COMPRESSOR_H

#include <inttypes.h>

#define PB_LENGTH 50
#define PRINT_MESSAGES

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

#ifdef __cplusplus
extern "C" {
#endif

int getLocalSum(const uint16_t * band, const int sizeY, const int sizeX, const int y, const int x);

int clip(const int val, const int val_min, const int val_max);

int sgn_plus(const int val);

int mod_R(const int x, const int R);

int d(const uint16_t *band, const int sizeX, const int y, const int x, const int localSum);
int dN(const uint16_t *band, const int sizeX, const int y, const int x, const int localSum);
int dW(const uint16_t *band, const int sizeX, const int y, const int x, const int localSum);
int dNW(const uint16_t *band, const int sizeX, const int y, const int x, const int localSum);

void getU(int * U, const int P, const uint16_t *image, const int sizeY,
          const int sizeX, const int z, const int y, const int x);

void weightInitDefault(int * W, const int Om, const int P);

int getPredictedD(const int * U, const int * W, const unsigned size);

int getScalingExp(const int D, const int Om, const int nu_min, const int nu_max,
                  const int t, const int t_inc, const int Nx);

uint16_t getMappedPredictionResidual(const int s, const int scale_s_pred,
                                     const int s_min, const int s_max);

uint16_t getRestoredValue(const int mappedResidual, const int scale_s_pred,
                          const int s_min, const int s_max, const int s_mid);

void updateW(int * W, int * U, unsigned size, int e, int ro, int w_min, int w_max);

void runPredictor(const uint16_t *in, uint16_t *out,
                  const struct ImageMetadata * imageMeta,
                  const struct PredictorMetadata * predMeta,
                  const int opType);

size_t getAccum(const size_t prevAccum, const size_t prevCounter,
                const uint32_t prevResidual, const uint32_t gamma);

size_t getCounter(const size_t prevCounter, const unsigned gamma);

unsigned getCodeWordSize(const size_t counter, const size_t accum);

void encodeGolomb(const uint16_t * in, uint32_t * out, size_t * outSize,
                  const struct ImageMetadata * imageMeta,
                  const struct EncoderMetadata * encoderMeta);

void decodeGolomb(const uint32_t * in, uint16_t * out,
                  const struct ImageMetadata * imageMeta,
                  const struct EncoderMetadata * encoderMeta);

int loadFromPGM(const char *fileName, uint16_t *data[],
                unsigned * sizeX, unsigned * sizeY, unsigned * maxValue);

int saveToPGM(const char *fileName, const uint16_t data[],
              const unsigned sizeX, const unsigned sizeY, const unsigned maxValue);

void swopBytes(uint16_t * p, size_t size);

int loadCompressedImage(const char *fileName, void ** data, size_t * dataSize,
                        struct ImageMetadata * imageMeta,
                        struct PredictorMetadata * predMeta,
                        struct EncoderMetadata * encoderMeta);

int saveCompressedImage(const char *fileName, const void *data, const size_t dataSize,
                        const struct ImageMetadata *imageMeta,
                        const struct PredictorMetadata *predMeta,
                        const struct EncoderMetadata *encoderMeta);

#ifdef __cplusplus
}
#endif

#endif // FL_COMPRESSOR_H
