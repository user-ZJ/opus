#include"cn_com_test_opus_OpusEncoder.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "jni.h"
#include <time.h>
#if (!defined WIN32 && !defined _WIN32) || defined(__MINGW32__)
#include <unistd.h>
#else
#include <process.h>
#define getpid _getpid
#endif
#include "opus.h"


OpusDecoder *dec = NULL;



JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusDecoder_nativeInitDecoder
(JNIEnv *env, jobject obj, jint samplingRate, jint numberOfChannels)
{
	printf("opus decode init !\n");
	int size;
	int error;
	size = opus_decoder_get_size(numberOfChannels);
	dec = malloc(size);
	error = opus_decoder_init(dec, samplingRate, numberOfChannels);
	return error;
}

JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusDecoder_nativeDecodeShorts___3B_3SI
(JNIEnv *env, jobject obj, jbyteArray in, jshortArray out, jint frames)
{
	jint inputArraySize = (*env)->GetArrayLength(env, in);

	jbyte* opusSignal = (*env)->GetByteArrayElements(env, in, 0);
	jshort* pcmSignal = (*env)->GetShortArrayElements(env, out, 0);
	int dataArraySize = opus_decode(dec, opusSignal, inputArraySize,pcmSignal, frames, 0);
	(*env)->ReleaseByteArrayElements(env, in, opusSignal, JNI_ABORT);
	(*env)->ReleaseShortArrayElements(env, out, pcmSignal, 0);

	return dataArraySize;
}

JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusDecoder_nativeDecodeBytes___3B_3BI
(JNIEnv *env, jobject obj, jbyteArray in, jbyteArray out, jint frames)
{
	jint inputArraySize = (*env)->GetArrayLength(env, in);

	jbyte* opusSignal = (*env)->GetByteArrayElements(env, in, 0);
	jbyte* pcmSignal = (*env)->GetByteArrayElements(env, out, 0);
	int dataArraySize = opus_decode(dec, (const unsigned char *)opusSignal, inputArraySize,(opus_int16 *)pcmSignal, frames, 0);
	(*env)->ReleaseByteArrayElements(env, in, opusSignal, JNI_ABORT);
	(*env)->ReleaseByteArrayElements(env, out, pcmSignal, 0);

	return dataArraySize*2;
}

JNIEXPORT jshortArray JNICALL Java_cn_com_test_opus_OpusDecoder_nativeDecodeShorts___3BI
(JNIEnv *env, jobject obj, jbyteArray in, jint frames)
{
	jint inputArraySize = (*env)->GetArrayLength(env, in);
	jbyte* opusSignal = (*env)->GetByteArrayElements(env, in, 0);
	jshortArray encodedData = (*env)->NewShortArray(env, frames * 2);
	jshort* encodedSignal = (*env)->GetShortArrayElements(env, encodedData, 0);
	int dataArraySize = opus_decode(dec, opusSignal, inputArraySize, encodedSignal, frames, 0);
	jshortArray pcmData = (*env)->NewShortArray(env, dataArraySize);
	(*env)->SetShortArrayRegion(env, pcmData, 0, dataArraySize, encodedSignal);
	(*env)->ReleaseByteArrayElements(env, in, opusSignal, JNI_ABORT);
	(*env)->ReleaseShortArrayElements(env, encodedData, encodedSignal, 0);
	return pcmData;
}

JNIEXPORT jbyteArray JNICALL Java_cn_com_test_opus_OpusDecoder_nativeDecodeBytes___3BI
(JNIEnv *env, jobject obj, jbyteArray in, jint frames) 
{
	jint inputArraySize = (*env)->GetArrayLength(env, in);
	jbyte* opusSignal = (*env)->GetByteArrayElements(env, in, 0);
	jbyteArray encodedData = (*env)->NewByteArray(env, frames * 2);
	jbyte* encodedSignal = (*env)->GetByteArrayElements(env, encodedData, 0);
	int dataArraySize = opus_decode(dec, (const unsigned char *)opusSignal, inputArraySize, (opus_int16 *)encodedSignal, frames, 0);
	jbyteArray pcmData = (*env)->NewByteArray(env, dataArraySize*2);
	(*env)->SetByteArrayRegion(env, pcmData, 0, dataArraySize*2, encodedSignal);
	(*env)->ReleaseByteArrayElements(env, in, opusSignal, JNI_ABORT);
	(*env)->ReleaseByteArrayElements(env, encodedData, encodedSignal, 0);
	return pcmData;
}


JNIEXPORT void JNICALL Java_cn_com_test_opus_OpusDecoder_nativeReleaseDecoder
(JNIEnv *env, jobject obj) 
{
	printf("opus decoder release!!!\n");
	free(dec);
}
