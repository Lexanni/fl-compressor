#include "fl_compressor.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int getLocalSum(const uint16_t *band, const int sizeY, const int sizeX, const int y, const int x) {
    int sNW, sN, sNE, sW;
    if (sizeX <= 0 || sizeY <= 0)
        return -1;
    if (y > 0 && y < sizeY) {
        if (x > 0 && x < sizeX - 1) {
            sNW = band[sizeX * (y - 1) + (x - 1)];
            sN  = band[sizeX * (y - 1) +    x   ];
            sNE = band[sizeX * (y - 1) + (x + 1)];
            sW  = band[sizeX *    y    + (x - 1)];
            return sNW + sN + sNE + sW;
        } else if (x == 0) {
            sN  = band[sizeX * (y - 1) +    x   ];
            sNE = band[sizeX * (y - 1) + (x + 1)];
            return (sN + sNE) << 1;
        } else if (x == sizeX - 1) {
            sNW = band[sizeX * (y - 1) + (x - 1)];
            sN  = band[sizeX * (y - 1) +    x   ];
            sW  = band[sizeX *    y    + (x - 1)];
            return sW + sNW + (sN << 1);
        }
    } else if ( y == 0 && x > 0 && x < sizeX) {
        return band[x - 1] << 2;
    }
    return -1;
}

int clip(const int val, const int val_min, const int val_max) {
    int t = val_min > val ? val_min : val;
    return val_max < t ? val_max : t;
}

int sgn_plus(const int val) {
    return val < 0 ? -1 : 1;
}

int mod_R(const int x, const int R) {
    return (int)(((x + (1ll << (R - 1ll))) % (1ll << R)) - (1ll << (R - 1)));
}

int d(const uint16_t *band, const int sizeX, const int y, const int x, const int localSum) {
    return (band[sizeX * y + x] << 2) - localSum;
}

int dN(const uint16_t *band, const int sizeX, const int y, const int x, const int localSum) {
    if(y == 0)
        return 0;
    return (band[sizeX * (y - 1) + x] << 2) - localSum;
}

int dW(const uint16_t *band, const int sizeX, const int y, const int x, const int localSum) {
    if(y == 0)
        return 0;
    if(x == 0)
        return (band[sizeX * (y - 1) + x] << 2) - localSum;
    return (band[sizeX * y + (x - 1)] << 2) - localSum;
}

int dNW(const uint16_t *band, const int sizeX, const int y, const int x, const int localSum) {
    if(y == 0)
        return 0;
    if(x == 0)
        return (band[sizeX * (y - 1) + x] << 2) - localSum;
    return (band[sizeX * (y - 1) + (x - 1)] << 2) - localSum;
}

void getU(int * U, const int P, const uint16_t *image,
          const int sizeY, const int sizeX, const int z, const int y, const int x) {
    int bandSize = sizeY * sizeX;
    const uint16_t * band = image + bandSize * z;

    int localSum = getLocalSum(band, sizeY, sizeX, y, x);

    U[0] =  dN(band, sizeX, y, x, localSum);
    U[1] =  dW(band, sizeX, y, x, localSum);
    U[2] = dNW(band, sizeX, y, x, localSum);

    if(z == 0) {
        int val = 0;
        for(int i = 1; i <= P; i++) {
            U[i + 2] = val;
        }
        return;
    }

    for(int i = 1; i <= P; i++) {
        int stepsDown = z - i > 0 ? i : z;
        const uint16_t * downBand = band - bandSize * stepsDown;
        U[i + 2] = d(downBand, sizeX, y, x, localSum);
    }
}

void weightInitDefault(int * W, const int Om, const int P) {
    W[0] = W[1] = W[2] = 0;
    if(P <= 0)
        return;
    W[3] = 7 * (1 << Om) / 8;
    for (int i = 1; i < P; i++) {
        W[3 + i] = W[3 + i - 1] / 8;
    }
}

int getPredictedD(const int *U, const int *W, const unsigned size) {
    int sum = 0;
    for (unsigned i = 0; i < size; i++) {
        sum += U[i] * W[i];
    }
    return sum;
}

int getScalingExp(const int D, const int Om, const int nu_min, const int nu_max,
                  const int t, const int t_inc, const int Nx) {
    return clip(nu_min + (t - Nx)/t_inc, nu_min, nu_max) + D - Om;
}

uint16_t getMappedPredictionResidual(const int s, const int scale_s_pred, const int s_min, const int s_max) {
    int s_pred = scale_s_pred >> 1;
    int residual = s - s_pred;
    int abs_residual = residual < 0 ? -residual : residual;
    int teta_a = s_pred - s_min;
    int teta_b = s_max - s_pred;
    int teta = teta_a < teta_b ? teta_a : teta_b;
    int k = (scale_s_pred & 1) == 0 ? residual : -residual;

    if(abs_residual > teta)
        return (uint16_t) (abs_residual + teta);
    if (0 <= k && k <= teta)
        return (uint16_t) (abs_residual << 1);
    return (uint16_t) ((abs_residual << 1) - 1);
}

uint16_t getRestoredValue(const int mapped_residual, const int scale_s_pred,
                          const int s_min, const int s_max, const int s_mid) {
    int s_pred = scale_s_pred >> 1;
    int residual;
    int teta_a = s_pred - s_min;
    int teta_b = s_max - s_pred;
    int teta = teta_a < teta_b ? teta_a : teta_b;
    int k = ((scale_s_pred + mapped_residual) & 1) == 0 ? 1 : -1;

    if(mapped_residual > teta * 2) {
        residual =(teta - mapped_residual) * sgn_plus(s_pred - s_mid);
    } else {
        residual =((mapped_residual + 1) >> 1) * k;
    }

    return (uint16_t) (residual + s_pred);
}

void updateW(int * W, int * U, unsigned size, int e, int ro, int w_min, int w_max) {
    int sign = sgn_plus(e);
    int f = ro < 0 ? (1 << -ro) : (1 << ro);
    if(ro < 0) {
        for(unsigned i = 0; i < size; i++) {
            W[i] = clip(W[i] + ((sign * (U[i] * f) + 1) / 2), w_min, w_max);
        }
    } else {
        for(unsigned i = 0; i < size; i++) {
            W[i] = clip(W[i] + ((sign * (U[i] / f) + 1) / 2), w_min, w_max);
        }
    }
}

void runPredictor(const uint16_t *in, uint16_t * out,
                  const struct ImageMetadata *imageMeta,
                  const struct PredictorMetadata *predMeta,
                  const int opType) {

    int sizeX = imageMeta->xSize;
    int sizeY = imageMeta->ySize;
    int sizeZ = imageMeta->zSize;
    int D = imageMeta->dynamicRange;
    D = D == 0 ? 16 : D;

    int P = predMeta->predictionBands;
    int R = predMeta->registerSize;
    int Om = predMeta->weightComponentResolution + 4;
    int nu_min = predMeta->wuScalingExpInitialParameter - 6;
    int nu_max = predMeta->wuScalingExpFinalParameter - 6;
    int t_inc = 1 << (predMeta->wuScalingExpChangeInterval + 4);
    int w_min = -(1 << (Om + 2));
    int w_max = (1 << (Om + 2)) - 1;
    uint16_t s_min = 0;
    uint16_t s_max = (uint16_t)((1L << D) - 1);
    uint16_t s_mid = (uint16_t)((1L << (D - 1)) - 1);

    unsigned bandSize = (unsigned)(sizeY * sizeX);

    const uint16_t * curBandIn = in;
    const uint16_t * predBase = (opType == PREDICTOR_MAP) ? in : out;
    const uint16_t * curPredBase = predBase;
    uint16_t * curBandOut = out;

    unsigned uwSize = (unsigned)(3 + P);

    int * msU = (int *)malloc(bandSize * uwSize * sizeof(int));
    int * msW = (int *)malloc(bandSize * uwSize * sizeof(int));

#ifdef PRINT_MESSAGES
    char progressBar[PB_LENGTH + 1] = {0};
    int progress = 0;
    char pbFormat[100];
    sprintf(pbFormat, "%s%d%s", "\r%3ld%% [%-", PB_LENGTH, "s]");
    memset(progressBar, '-', PB_LENGTH);

    if(opType == PREDICTOR_MAP)
        printf("Prediction...\n");
    else
        printf("Restoration...\n");
#endif

    weightInitDefault(msW, Om, P);

    for(unsigned i = 1; i < bandSize; i++)
        memcpy(msW + uwSize * i, msW, (size_t)uwSize * sizeof(int));

    for(int z = 0; z < sizeZ; z++) {
        int * curU = msU + uwSize;
        int * curW = msW + uwSize;
        int scale_s_pred;

        if(P > 0 && z > 0)
            scale_s_pred = *(curPredBase - bandSize) * 2;
        else
            scale_s_pred = s_mid * 2;

        if(opType == PREDICTOR_MAP)
            curBandOut[0] = getMappedPredictionResidual(curBandIn[0], scale_s_pred, s_min, s_max);
        else
            curBandOut[0] = getRestoredValue(curBandIn[0], scale_s_pred, s_min, s_max, s_mid);

        int t = 1;
        for(int y = 0; y < sizeY; y++) {
            for(int x = (y == 0 ? 1 : 0); x < sizeX; x++) {
                int local_sum = getLocalSum(curPredBase, sizeY, sizeX, y, x);
                getU(curU, P, predBase, sizeY, sizeX, z, y, x);
                int d_pred = getPredictedD(curU, curW, uwSize);
                int a = d_pred + ((local_sum - ((int)s_mid << 2)) << Om);
                int m = mod_R(a, R);
                int val = m >> (Om + 1);
                scale_s_pred = clip(val + 2 * s_mid + 1 , 2 * s_min, 2 * s_max + 1);
                if(opType == PREDICTOR_MAP)
                    curBandOut[t] = getMappedPredictionResidual(curBandIn[t], scale_s_pred, s_min, s_max);
                else
                    curBandOut[t] = getRestoredValue(curBandIn[t], scale_s_pred, s_min, s_max, s_mid);
                int e = 2 * curPredBase[t] - scale_s_pred;
                int ro = getScalingExp(D, Om, nu_min, nu_max, t, t_inc, sizeX);
                updateW(curW, curU, uwSize, e, ro, w_min, w_max);
                curU += uwSize;
                curW += uwSize;
                t++;
            }
        }
        curBandIn  += bandSize;
        curBandOut += bandSize;
        curPredBase += bandSize;
#ifdef PRINT_MESSAGES
        progress = (z + 1) * (PB_LENGTH - 1) / sizeZ;
        memset(progressBar, '#', (size_t)progress + 1);
        printf(pbFormat, (z + 1) * 100 / sizeZ, progressBar);
#endif
    }

    if(msU)
        free(msU);
    if(msW)
        free(msW);
#ifdef PRINT_MESSAGES
    memset(progressBar, '#', PB_LENGTH);
    printf(pbFormat, 100, progressBar);

    if(opType == PREDICTOR_MAP)
        printf("\nPrediction done.\n");
    else
        printf("\nRestoration done.\n");
#endif
}

size_t getAccum(const size_t prevAccum, const size_t prevCounter, const uint32_t prevResidual, const uint32_t gamma) {

    if(prevCounter < (1ull << gamma) - 1)
        return prevAccum + (size_t)prevResidual;

    return (prevAccum + (size_t)prevResidual + 1) / 2;
}

size_t getCounter(const size_t prevCounter, const unsigned gamma) {
    if(prevCounter < (1ull << gamma) - 1)
        return prevCounter + 1;
    return (prevCounter + 1) / 2;
}

unsigned getCodeWordSize(const size_t counter, const size_t accum) {
    size_t right = accum + ((49 * counter) >> 7);
    size_t left = counter;
    unsigned k = 0;

    do {
        left <<= 1;
        k++;
    } while(left <= right);

    return k - 1;
}

void encodeGolomb(const uint16_t *in, uint32_t * out, size_t * outSize,
                  const struct ImageMetadata *imageMeta,
                  const struct EncoderMetadata *encoderMeta) {

    uint32_t sizeX = imageMeta->xSize;
    uint32_t sizeY = imageMeta->ySize;
    uint32_t sizeZ = imageMeta->zSize;
    unsigned D = imageMeta->dynamicRange;
    D = D == 0 ? 16 : D;

    uint32_t bandSize = sizeY * sizeX;

    const uint16_t * curBandIn =  in;
    uint32_t * curBandOut = out;

    unsigned U_max = encoderMeta->unaryLengthLimit;
    unsigned gamma_0 = encoderMeta->accumInitConstant;
    unsigned gamma   = encoderMeta->rescalingCounterSize + 4;
    unsigned k = encoderMeta->accumInitConstant;
    size_t counter = 1ull << gamma_0;
    size_t accum   = (3 * (1 << (k + 6)) - 49) * counter / (1 << 7);

#ifdef PRINT_MESSAGES
    char progressBar[PB_LENGTH + 1] = {0};
    unsigned  progress = 0;
    char pbFormat[100];
    sprintf(pbFormat, "%s%d%s", "\r%3ld%% [%-", PB_LENGTH, "s]");
    memset(progressBar, '-', PB_LENGTH);
    printf("Encoding...\n");
#endif

    uint64_t buffer = 0;
    uint32_t bitCounter = 0;

    for (uint32_t z = 0; z < sizeZ; z++) {
        buffer <<= D;
        buffer += (uint16_t)curBandIn[0];
        bitCounter += D;
        for(uint32_t i = 1; i < bandSize; i++) {
            accum = getAccum(accum, counter, curBandIn[i - 1], gamma);
            counter = getCounter(counter, gamma);
            k = getCodeWordSize(counter, accum);
            div_t d = div((int32_t)curBandIn[i], (1 << k));
            uint32_t u =  (uint32_t)d.quot;
            u = (u < U_max) ? u : U_max;
            buffer <<= u;
            bitCounter += u;
            if(u < U_max) {
                buffer <<= 1;
                buffer += 1;
                bitCounter +=1;
                buffer <<= k;
                bitCounter += k;
                buffer += (uint64_t)d.rem;
            } else {
                buffer <<= D;
                bitCounter += D;
                buffer += (uint16_t)curBandIn[i];
            }
            if(bitCounter >= 32) {
                bitCounter -= 32;
                uint32_t val = (uint32_t)(buffer >> bitCounter);
                *curBandOut = val;
                curBandOut++;
            }
        }
        curBandIn += bandSize;

#ifdef PRINT_MESSAGES
        progress = (z + 1) * (PB_LENGTH - 1) / sizeZ;
        memset(progressBar, '#', (size_t)progress + 1);
        printf(pbFormat, (z + 1) * 100 / sizeZ, progressBar);
#endif
    }
    if(bitCounter > 0) {
        uint32_t val = (uint32_t)(buffer << (32 - bitCounter));
        *curBandOut = val;
        curBandOut++;
    }

    *outSize = (size_t)(curBandOut - out) * sizeof (uint32_t);

#ifdef PRINT_MESSAGES
    memset(progressBar, '#', PB_LENGTH);
    printf(pbFormat, 100, progressBar);
    printf("\nEncoding done.\n");
#endif
}

void decodeGolomb(const uint32_t *in, uint16_t * out,
                  const struct ImageMetadata *imageMeta,
                  const struct EncoderMetadata *encoderMeta) {

    uint32_t sizeX = imageMeta->xSize;
    uint32_t sizeY = imageMeta->ySize;
    uint32_t sizeZ = imageMeta->zSize;
    unsigned D = imageMeta->dynamicRange;
    D = D == 0 ? 16 : D;

    uint32_t bandSize = sizeY * sizeX;

    const uint32_t * curBandIn =  in;
    uint16_t * curBandOut = out;

    unsigned U_max = encoderMeta->unaryLengthLimit;
    unsigned gamma_0 = encoderMeta->accumInitConstant;
    unsigned gamma   = encoderMeta->rescalingCounterSize + 4;
    unsigned k = encoderMeta->accumInitConstant;
    size_t counter = 1ull << gamma_0;
    size_t accum   = (3 * (1 << (k + 6)) - 49) * counter / (1 << 7);

#ifdef PRINT_MESSAGES
    char progressBar[PB_LENGTH + 1] = {0};
    unsigned progress = 0;
    char pbFormat[100];
    sprintf(pbFormat, "%s%d%s", "\r%3ld%% [%-", PB_LENGTH, "s]");
    memset(progressBar, '-', PB_LENGTH);
    printf("Decoding...\n");
#endif

    uint64_t buffer = 0;
    uint64_t lastBit = 1ull << 63;
    uint32_t bitCounter = 0;
    uint32_t sampleCounter = 0;
    uint32_t bandsCounter = 0;
    uint16_t val = 0;

    buffer += *curBandIn;
    buffer <<= 32;
    curBandIn++;
    buffer += *curBandIn;
    curBandIn++;

    while(bandsCounter < sizeZ) {
        val = (uint16_t)(buffer >> (64 - D));
        buffer <<= D;
        bitCounter += D;

        curBandOut[0] = val;
        sampleCounter = 1;

        while(sampleCounter < bandSize) {

            accum = getAccum(accum, counter, val, gamma);
            counter = getCounter(counter, gamma);
            k = getCodeWordSize(counter, accum);

            uint16_t u = 0;
            while((buffer & lastBit) == 0ull) {
                u++;
                buffer <<= 1;
                bitCounter++;
                if(u == U_max)
                    break;
            }
            if(u == U_max) {
                val = (uint16_t)(buffer >> (64 - D));
                buffer <<= D;
                bitCounter += D;
            } else {
                val = (uint16_t)(u << k);
                buffer <<= 1;
                bitCounter++;
                if(k > 0) {
                    val += (uint16_t)(buffer >> (64 - k));
                    buffer <<= k;
                    bitCounter += k;
                }
            }
            if(bitCounter >= 32) {
                bitCounter -= 32;
                buffer >>= bitCounter;
                buffer += *curBandIn;
                buffer <<= bitCounter;
                curBandIn++;
            }

            curBandOut[sampleCounter] = val;
            sampleCounter++;
        }
        bandsCounter++;
        curBandOut += bandSize;
#ifdef PRINT_MESSAGES
        progress = bandsCounter * (PB_LENGTH - 1) / sizeZ;
        memset(progressBar, '#', (size_t)progress + 1);
        printf(pbFormat, bandsCounter * 100 / sizeZ, progressBar);
#endif
    }
#ifdef PRINT_MESSAGES
    memset(progressBar, '#', PB_LENGTH);
    printf(pbFormat, 100, progressBar);
    printf("\nDecoding done.\n");
#endif
}

void swopBytes(uint16_t * p, size_t size) {
    for(size_t i = 0; i < size; i++) {
        uint16_t tmp = p[i] >> 8;
        p[i] = (uint16_t)((p[i] << 8) + tmp);
    }
}

int loadFromPGM(const char *fileName, uint16_t *data[],
                unsigned * sizeX, unsigned * sizeY, unsigned * maxValue) {
    FILE * fp;
    char buffer[1024];

    fp = fopen(fileName, "rb");
    if(!fp)
        return -1;

    fscanf(fp, "%s", buffer);
    if(strcmp(buffer, "P5")) {
        printf("File %s has an invalid format. Must be PGM.\n", fileName);
        return -1;
    }
    if(fscanf(fp, "%u%u%u", sizeX, sizeY, maxValue) != 3)
        return -1;

    size_t size = *sizeX * *sizeY;
    int bitsPerChannel = (int)(log(*maxValue + 1.0) / log(2.0));

    if(!(8 < bitsPerChannel && bitsPerChannel <= 16))
        return -1;

    int ch = fgetc(fp);
    while(ch != '\n')
        ch = fgetc(fp);

    *data = (uint16_t *)malloc(size * sizeof (uint16_t));
    if(!(*data))
        return -1;

    if(fread(*data, sizeof(uint16_t), size, fp) != size) {
        free(*data);
        return -1;
    }
    fclose(fp);

    swopBytes(*data, size);

    return 0;
}

int saveToPGM(const char *fileName, const uint16_t data[],
              const unsigned sizeX, const unsigned sizeY, const unsigned maxValue)
{
    FILE * fp;
    fp = fopen(fileName, "wb");
    if(!fp)
        return -1;
    fprintf(fp, "P5\n%u %u\n%u\n", sizeX, sizeY, maxValue);

    size_t size = sizeX * sizeY;

    uint16_t * p = (uint16_t *)malloc(size * sizeof(uint16_t));
    if(!p) {
        fclose(fp);
        return -1;
    }
    memcpy(p, data, size * sizeof(uint16_t));
    swopBytes(p, size);

    if(fwrite(p, sizeof(uint16_t), size, fp) != size) {
        if(p)
            free(p);
        fclose(fp);
        return -1;
    }

    if(p)
        free(p);
    fclose(fp);

    return 0;
}

int saveCompressedImage(const char * fileName, const void * data, const size_t dataSize,
                        const struct ImageMetadata * imageMeta,
                        const struct PredictorMetadata * predMeta,
                        const struct EncoderMetadata * encoderMeta) {

    FILE * fp;
    fp = fopen(fileName, "wb");
    if(!fp)
        return -1;

    size_t i_size = sizeof (struct ImageMetadata);
    size_t p_size = sizeof (struct PredictorMetadata);
    size_t e_size = sizeof (struct EncoderMetadata);

    size_t total = 0;

    total += fwrite(imageMeta  , i_size  , 1, fp);
    total += fwrite(predMeta   , p_size  , 1, fp);
    total += fwrite(encoderMeta, e_size  , 1, fp);
    total += fwrite(data       , dataSize, 1, fp);

    fclose(fp);

    if(total != 4)
        return -1;

    return 0;
}

int loadCompressedImage(const char * fileName, void ** data, size_t * dataSize,
                        struct ImageMetadata * imageMeta,
                        struct PredictorMetadata * predMeta,
                        struct EncoderMetadata * encoderMeta) {

    FILE * fp;
    fp = fopen(fileName, "rb");
    if(!fp)
        return -1;

    size_t i_size = sizeof (struct ImageMetadata);
    size_t p_size = sizeof (struct PredictorMetadata);
    size_t e_size = sizeof (struct EncoderMetadata);

    size_t headerSize = i_size + p_size + e_size;

    size_t total = 0;

    total += fread(imageMeta  , i_size, 1, fp);
    total += fread(predMeta   , p_size, 1, fp);
    total += fread(encoderMeta, e_size, 1, fp);

    if(total != 3) {
        fclose(fp);
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    *dataSize = (size_t)ftell(fp) - headerSize;
    fseek(fp, (long)headerSize, SEEK_SET);

    *data = malloc(*dataSize);
    if(!(*data)) {
        fclose(fp);
        return -1;
    }

    total += fread(*data, *dataSize, 1, fp);
    fclose(fp);

    if(total != 4)
    {
        free(*data);
        return -1;
    }

    return 0;
}
