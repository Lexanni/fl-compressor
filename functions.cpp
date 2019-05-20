#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

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

int clip(int val, int val_min, int val_max) {
    int t = val_min > val ? val_min : val;
    return val_max < t ? val_max : t;
}

int sgn_plus(int val) {
    return val < 0 ? -1 : 1;
}

int mod_R(int x, int R) {
    return ((x + (1 << (R - 1))) % (1 << R)) - (1 << (R - 1));
}

int d(int * currentBand, int sizeY, int sizeX, int y, int x) {
    return (currentBand[sizeX * y + x] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
}

int dN(int * currentBand, int sizeY, int sizeX, int y, int x) {
    if(y == 0)
        return 0;
    return (currentBand[sizeX * (y - 1) + x] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
}

int dW(int * currentBand, int sizeY, int sizeX, int y, int x) {
    if(y == 0)
        return 0;
    if(x == 0)
        return (currentBand[sizeX * (y - 1) + x] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
    return (currentBand[sizeX * y + (x - 1)] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
}

int dNW(int * currentBand, int sizeY, int sizeX, int y, int x) {
    if(y == 0)
        return 0;
    if(x == 0)
        return (currentBand[sizeX * (y - 1) + x] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
    return (currentBand[sizeX * (y - 1) + (x - 1)] << 2) - getLocalSum(currentBand, sizeY, sizeX, y, x);
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

void weightInitDefault(int * W, int om, int P) {
    W[0] = W[1] = W[2] = 0;
    if(P <= 0)
        return;
    W[3] = 7 * (1 << om) / 8;
    for (int i = 1; i <= P; i++) {
        W[3 + i] = W[3 + i - 1] / 8;
    }
}

int getPredictedD(int * U, int * W, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += U[i] * W[i];
    }
    return sum;
}

int getScalingExp(int D, int Om, int v_min, int v_max, int t, int t_inc, int Nx) {
    return clip(v_min + (t - Nx)/t_inc, v_min, v_max) + D - Om;
}

int getMappedPredictionResidual(int s, int scale_s_pred, int s_min, int s_max) {
    int s_pred = scale_s_pred / 2;
    int residual = s - s_pred;
    int abs_residual = residual < 0 ? -residual : residual;
    int teta_a = s_pred - s_min;
    int teta_b = s_max - s_pred;
    int teta = teta_a < teta_b ? teta_a : teta_b;
    int k = (scale_s_pred & 1) ? -residual : residual;

    if(abs_residual > teta)
        return abs_residual + teta;
    if (0 <= k && k <= teta)
        return abs_residual << 1;
    return (abs_residual << 1) - 1;
}

void updateW(int * W, int * U, int size, int e, int ro, int w_min, int w_max) {
    int sign = sgn_plus(e);
    int f = ro < 0 ? (1 << -ro) : (1 << ro);
    if(ro < 0) {
        for(int i = 0; i < size; i++) {
            W[i] = clip(W[i] + ((sign * (U[i] * f) + 1) / 2), w_min, w_max);
        }
    } else {
        for(int i = 0; i < size; i++) {
            W[i] = clip(W[i] + ((sign * (U[i] / f) + 1) / 2), w_min, w_max);
        }
    }
}

void fun(int * in, int * out, ImageMetadata * imageMeta, PredictorMetadata * predMeta,
         int s_min, int s_max, int s_mid) {

    int sizeX = imageMeta->sizeX;
    int sizeY = imageMeta->sizeY;
    int sizeZ = imageMeta->sizeZ;
    int D = imageMeta->D;
    int P = predMeta->P;
    int R = predMeta->R;
    int Om = predMeta->Om;
    int v_min = predMeta->v_min;
    int v_max = predMeta->v_max;
    int t_inc = predMeta->t_inc;
    int w_min = -(1 << (Om + 2));
    int w_max = (1 << (Om + 2)) - 1;

    int bandSize = sizeY * sizeX;

    int * curBandIn = in;
    int * curBandOut = out;

    int * msU = (int *)malloc(sizeX * sizeY * P * sizeof(int));
    int * msW = (int *)malloc(sizeX * sizeY * P * sizeof(int));

    weightInitDefault(msW, Om, P);

    for(int i = P; i < bandSize; i += P)
        memcpy(&msW[i], msW, (size_t)P * sizeof(int));

    int * U = msU;
    int * W = msW;

    for(int z = 0; z < sizeZ; z++) {
        int * curU = U + P;
        int * curW = W + P;
        int scale_s_pred;

        if(P > 0 && z > 0)
            scale_s_pred = *(curBandIn - bandSize) * 2;
        else
            scale_s_pred = s_mid * 2;

        curBandOut[0] = getMappedPredictionResidual(curBandIn[0], scale_s_pred, s_min, s_max);

        for(int y = 0, t = 1; y < sizeY; y++) {
            for(int x = 1; x < sizeX; x++) {
                int local_sum = getLocalSum(curBandIn, sizeY, sizeX, y, x);
                getU(U, P, in, sizeY, sizeX, z, y, x);
                int d_pred = getPredictedD(U, W, P);
                scale_s_pred = clip((mod_R(d_pred + ((local_sum - (s_mid << 2)) << Om), R) >> (Om - 1)) + 2 * s_mid + 1 , 2 * s_min, 2 * s_max + 1);
                curBandOut[t] = getMappedPredictionResidual(curBandIn[t], scale_s_pred, s_min, s_max);
                int e = 2 * curBandIn[t] - scale_s_pred;
                int ro = getScalingExp(D, Om, v_min, v_max, t, t_inc, sizeX);
                updateW(curW, curU, P, e, ro, w_min, w_max);
                curU += P;
                curW += P;
                t++;
            }
        }
        curBandIn  += bandSize;
        curBandOut += bandSize;
    }

    if(msU)
        free(msU);
    if(msW)
        free(msW);
}

size_t getAccum(size_t prevAccum, size_t prevCounter, uint32_t prevResidual, uint32_t gamma) {

    if(prevCounter < (1 << gamma) - 1)
        return prevAccum + (size_t)prevResidual;

    return (prevAccum + (size_t)prevResidual + 1) / 2;
}

size_t getCounter(size_t prevCounter, unsigned gamma) {
    if(prevCounter < (1 << gamma) - 1)
        return prevCounter + 1;
    return (prevCounter + 1) / 2;
}

unsigned getCodeWordSize(size_t counter, size_t accum) {
    size_t right = accum + ((49 * counter) >> 7);
    size_t left = counter;
    unsigned k = 0;

    do {
        left <<= 1;
        k++;
    } while(left <= right);

    return k - 1;
}

void encodeGolomb(uint32_t * in, uint32_t * out, size_t * outSize, ImageMetadata * imageMeta, EncoderMetadata * encoderMeta) {

    uint32_t sizeX = imageMeta->sizeX;
    uint32_t sizeY = imageMeta->sizeY;
    uint32_t sizeZ = imageMeta->sizeZ;
    int D = imageMeta->D;

    uint32_t bandSize = sizeY * sizeX;

    uint32_t * curBandIn =  (uint32_t *)in;
    uint32_t * curBandOut = (uint32_t *)out;

    unsigned U_max = encoderMeta->unaryLengthLimit;
    unsigned gamma_0 = encoderMeta->accumInitConstant;
    unsigned gamma   = encoderMeta->rescalingCounterSize + 4;
    unsigned k = encoderMeta->accumInitConstant;
    size_t counter = 1 << gamma_0;
    size_t accum   = (3 * (1 << (k + 6)) - 49) * counter / (1 << 7);

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
    }
    if(bitCounter > 0) {
        uint32_t val = (uint32_t)(buffer << (32 - bitCounter));
        *curBandOut = val;
        curBandOut++;
    }

    *outSize = (uint8_t *)curBandOut - (uint8_t *)out;
}

void decodeGolomb(void * in, void * out, ImageMetadata * imageMeta, EncoderMetadata * encoderMeta) {

    uint32_t sizeX = imageMeta->sizeX;
    uint32_t sizeY = imageMeta->sizeY;
    uint32_t sizeZ = imageMeta->sizeZ;
    int D = imageMeta->D;

    uint32_t bandSize = sizeY * sizeX;

    uint32_t * curBandIn =  (uint32_t *)in;
    uint16_t * curBandOut = (uint16_t *)out;

    unsigned U_max = encoderMeta->unaryLengthLimit;
    unsigned gamma_0 = encoderMeta->accumInitConstant;
    unsigned gamma   = encoderMeta->rescalingCounterSize + 4;
    unsigned k = encoderMeta->accumInitConstant;
    size_t counter = 1 << gamma_0;
    size_t accum   = (3 * (1 << (k + 6)) - 49) * counter / (1 << 7);

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
    }
}

