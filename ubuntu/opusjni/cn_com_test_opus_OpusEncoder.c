#include"cn_com_test_opus_OpusEncoder.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <malloc.h>
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


OpusEncoder *enc = NULL;




JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusEncoder_nativeInitEncoder(JNIEnv *env, jobject obj, jint samplingRate, jint numberOfChannels, jint application)
{
	printf("opus encoder init!\n");
	int error;
	int size;

	size = opus_encoder_get_size(numberOfChannels);
	enc = malloc(size);
	error = opus_encoder_init(enc, samplingRate, numberOfChannels, application);

	if (error) {
		free(enc);
	}
	return error;
}

JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusEncoder_nativeEncodeShorts___3SI_3B(JNIEnv *env, jobject obj, jshortArray in, jint frames, jbyteArray out)
{
	jint outputArraySize = (*env)->GetArrayLength(env, out);
	jshort* pcmSignal = (*env)->GetShortArrayElements(env, in, 0);
	jbyte* opusSignal = (*env)->GetByteArrayElements(env, out, 0);
	int dataArraySize = opus_encode(enc, pcmSignal, frames,opusSignal, outputArraySize);
	(*env)->ReleaseShortArrayElements(env, in, pcmSignal, JNI_ABORT);
	(*env)->ReleaseByteArrayElements(env, out, opusSignal, 0);
	return dataArraySize;
}


JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusEncoder_nativeEncodeBytes___3BI_3B(JNIEnv *env, jobject obj, jbyteArray in, jint frames, jbyteArray out)
{

	jint outputArraySize = (*env)->GetArrayLength(env, out);
	jbyte* pcmSignal = (*env)->GetByteArrayElements(env, in, 0);
	jbyte* opusSignal = (*env)->GetByteArrayElements(env, out, 0);
	int dataArraySize = opus_encode(enc, (const opus_int16 *)pcmSignal, frames, (unsigned char *)opusSignal, outputArraySize);
	(*env)->ReleaseByteArrayElements(env, in, pcmSignal, JNI_ABORT);
	(*env)->ReleaseByteArrayElements(env, out, opusSignal, 0);
	return dataArraySize;
}

JNIEXPORT jbyteArray JNICALL Java_cn_com_test_opus_OpusEncoder_nativeEncodeShorts___3SI(JNIEnv *env, jobject obj, jshortArray in,jint frames)
{
	jint inputArraySize = (*env)->GetArrayLength(env, in);
	jshort* pcmSignal = (*env)->GetShortArrayElements(env, in, 0);
	jbyteArray decodedData = (*env)->NewByteArray(env, inputArraySize*2);
	jbyte * decodedSignal = (*env)->GetByteArrayElements(env, decodedData, 0);
	int dataArraySize = opus_encode(enc, pcmSignal, frames, decodedSignal, inputArraySize * 2);
	jbyteArray opusData = (*env)->NewByteArray(env, dataArraySize);
	(*env)->SetByteArrayRegion(env, opusData, 0, dataArraySize, decodedSignal);
	(*env)->ReleaseShortArrayElements(env, in, pcmSignal, JNI_ABORT);
	(*env)->ReleaseByteArrayElements(env, decodedData, decodedSignal, 0);
	return opusData;
}

JNIEXPORT jbyteArray JNICALL Java_cn_com_test_opus_OpusEncoder_nativeEncodeBytes___3BI
(JNIEnv *env, jobject obj, jbyteArray in, jint frames) {
	jint inputArraySize = (*env)->GetArrayLength(env, in);
	jbyte* pcmSignal = (*env)->GetByteArrayElements(env, in, 0);
	jbyteArray decodedData = (*env)->NewByteArray(env, inputArraySize * 2);
	jbyte * decodedSignal = (*env)->GetByteArrayElements(env, decodedData, 0);
	int dataArraySize = opus_encode(enc, (const opus_int16 *)pcmSignal, frames, decodedSignal, inputArraySize * 2);
	jbyteArray opusData = (*env)->NewByteArray(env, dataArraySize);
	(*env)->SetByteArrayRegion(env, opusData, 0, dataArraySize, decodedSignal);
	(*env)->ReleaseByteArrayElements(env, in, pcmSignal, JNI_ABORT);
	(*env)->ReleaseByteArrayElements(env, decodedData, decodedSignal, 0);
	return opusData;
}

JNIEXPORT void JNICALL Java_cn_com_test_opus_OpusEncoder_nativeReleaseEncoder(JNIEnv *env, jobject obj)
{
	printf("opus encoder release!!!\n");
	free(enc);
}
