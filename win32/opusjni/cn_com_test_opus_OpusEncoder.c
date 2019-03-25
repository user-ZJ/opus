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
#include "opus_defines.h"
#include "opus_types.h"

#define MAX_PACKET 1500

OpusEncoder *enc = NULL;
static FILE *fin = NULL;
static FILE *fout = NULL;
static short *in = NULL;  //读入的数据
static unsigned char *fbytes = NULL;
static unsigned char *data = NULL;
int bitrate_bps;  //音频带宽bits/s
int bandwidth;
static int max_frame_size = 48000 * 2;
int channels;
int frame_size;
opus_int32 sampling_rate;  //音频采样率
int max_payload_bytes; //最大载荷字节数，编码参数，1500
int use_vbr;  //启用恒定比特率，编码参数
int complexity; //复杂度,编码参数，默认为10
int use_inbandfec;  //启用SILK内置FEC
int use_dtx;  //启用SILK DTX,编码参数
int forcechannels;  //强制单声道编码，编码参数
int packet_loss_perc;  ////模拟丢包百分比
int cvbr;  //启用约束变量比特率,编码参数


static void int_to_char(opus_uint32 i, unsigned char ch[4])
{
	ch[0] = i >> 24;
	ch[1] = (i >> 16) & 0xFF;
	ch[2] = (i >> 8) & 0xFF;
	ch[3] = i & 0xFF;
}

static opus_uint32 char_to_int(unsigned char ch[4])
{
	return ((opus_uint32)ch[0] << 24) | ((opus_uint32)ch[1] << 16)
		| ((opus_uint32)ch[2] << 8) | (opus_uint32)ch[3];
}


JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusEncoder_nativeInitEncoder(JNIEnv *env, jobject obj, jint samplingRate, jint numberOfChannels, jint framesize,jint application)
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
	channels = numberOfChannels;
	frame_size = framesize;
	sampling_rate = samplingRate;
	/* defaults: */
	bitrate_bps = 16000; //比特率 16Kbits/s
	use_vbr = 1;  //是否启用恒定比特率
	max_payload_bytes = MAX_PACKET; //最大有效负载 1500
	complexity = 10;  //复杂度
	use_inbandfec = 0;  //是否启用SILK内置FEC
	bandwidth = OPUS_AUTO;
	forcechannels = OPUS_AUTO;  //是否强制单声道编码，即使是立体声输入
	use_dtx = 0;  //是否启用SILK DTX
	packet_loss_perc = 0;  //丢包率
	cvbr = 0;  //启用约束变量比特率,编码参数
	opus_encoder_ctl(enc, OPUS_SET_BITRATE(bitrate_bps));
	opus_encoder_ctl(enc, OPUS_SET_BANDWIDTH(bandwidth));
	opus_encoder_ctl(enc, OPUS_SET_VBR(use_vbr));
	opus_encoder_ctl(enc, OPUS_SET_VBR_CONSTRAINT(cvbr));
	opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(complexity));
	opus_encoder_ctl(enc, OPUS_SET_PACKET_LOSS_PERC(packet_loss_perc));
	return error;
}

JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusEncoder_nativeEncodeShorts___3S_3B(JNIEnv *env, jobject obj, jshortArray in, jbyteArray out)
{
	jint outputArraySize = (*env)->GetArrayLength(env, out);
	jshort* pcmSignal = (*env)->GetShortArrayElements(env, in, 0);
	jbyte* opusSignal = (*env)->GetByteArrayElements(env, out, 0);
	int dataArraySize = opus_encode(enc, pcmSignal, frame_size,opusSignal, outputArraySize);
	(*env)->ReleaseShortArrayElements(env, in, pcmSignal, JNI_ABORT);
	(*env)->ReleaseByteArrayElements(env, out, opusSignal, 0);
	return dataArraySize;
}

JNIEXPORT jbyteArray JNICALL Java_cn_com_test_opus_OpusEncoder_nativeEncodeShorts___3S(JNIEnv *env, jobject obj, jshortArray in)
{
	jint inputArraySize = (*env)->GetArrayLength(env, in);
	jshort* pcmSignal = (*env)->GetShortArrayElements(env, in, 0);
	jbyteArray decodedData = (*env)->NewByteArray(env, inputArraySize*2);
	jbyte * decodedSignal = (*env)->GetByteArrayElements(env, decodedData, 0);
	int dataArraySize = opus_encode(enc, pcmSignal, frame_size, decodedSignal, inputArraySize * 2);
	jbyteArray opusData = (*env)->NewByteArray(env, dataArraySize);
	(*env)->SetByteArrayRegion(env, opusData, 0, dataArraySize, decodedSignal);
	(*env)->ReleaseShortArrayElements(env, in, pcmSignal, JNI_ABORT);
	(*env)->ReleaseByteArrayElements(env, decodedData, decodedSignal, 0);
	return opusData;
}

JNIEXPORT void JNICALL Java_cn_com_test_opus_OpusEncoder_nativeSetBitrate
(JNIEnv *env, jobject obj, jint bitrate) {
	opus_encoder_ctl(enc, OPUS_SET_BITRATE(bitrate));
}


JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusEncoder_nativeEncodeFile
(JNIEnv *env, jobject obj, jstring pcmFilePath, jstring opusFilePath) {
	int stop = 0;
	int len;
	size_t num_read;
	int remaining = 0;
	int curr_read = 0;
	int nb_encoded = 0;
	opus_uint32 enc_final_range; //最终范围
	double tot_samples = 0;  //opus数据总长度
	opus_uint64 tot_in=0;
	int lost = 0, lost_prev = 1;
	opus_int32 count = 0, count_act = 0;  //帧计算器
	double bits = 0.0, bits_max = 0.0, bits_act = 0.0, bits2 = 0.0, nrg;
	int k;
	int i;
	int err = 0;

	const char *pcmFile = (*env)->GetStringUTFChars(env,pcmFilePath,0);
	const char *opusFile = (*env)->GetStringUTFChars(env, opusFilePath, 0);

	err = fopen_s(&fin, pcmFile, "rb");
	if (err)
	{
		fprintf(stderr, "Could not open input file %s\n", (char *)pcmFile);
		return -1;
	}
	err = fopen_s(&fout,opusFile, "wb+");
	if (err)
	{
		fprintf(stderr, "Could not open output file %s\n", (char *)opusFile);
		return -1;
	}
	//(*env)->ReleaseStringUTFChars(env,pcmFilePath, pcmFile);
	//(*env)->ReleaseStringUTFChars(env, opusFilePath, opusFile);
	in = (short*)malloc(max_frame_size*channels * sizeof(short));  //输入buff，48000*2*channels*2
	fbytes = (unsigned char*)malloc(max_frame_size*channels * sizeof(short));  //从pcm文件中按字节读取的数据
	data = (unsigned char*)calloc(max_payload_bytes, sizeof(unsigned char));  //分配1500*2byte内存
	while (!stop) {
		num_read = fread(fbytes, sizeof(short)*channels, frame_size - remaining, fin);  //从pcm中每次读frame_size大小数据
		curr_read = (int)num_read;
		tot_in += curr_read;
		for (i = 0; i<curr_read*channels; i++)
		{
			opus_int32 s;  //byte->int 
			s = fbytes[2 * i + 1] << 8 | fbytes[2 * i];
			s = ((s & 0xFFFF) ^ 0x8000) - 0x8000;
			in[i + remaining * channels] = s;
		}
		if (curr_read + remaining < frame_size)  //剩余数据不足一帧时，结束
		{
			for (i = (curr_read + remaining)*channels; i<frame_size*channels; i++)
				in[i] = 0;
			stop = 1;
		}
		len = opus_encode(enc, in, frame_size, data, max_payload_bytes);
		//opus_packet_get_samples_per_frame 获取Opus数据包中每帧的样本数 160
		//opus_packet_get_nb_frames 获取Opus数据包中的帧数  1
		nb_encoded = opus_packet_get_samples_per_frame(data, sampling_rate)*opus_packet_get_nb_frames(data, len);
		remaining = frame_size - nb_encoded;  //0
		for (i = 0; i<remaining*channels; i++)
			in[i] = in[nb_encoded*channels + i];
		opus_encoder_ctl(enc, OPUS_GET_FINAL_RANGE(&enc_final_range));
		if (len < 0)
		{
			fprintf(stderr, "opus_encode() returned %d\n", len);
			return -1;
		}
		unsigned char int_field[4];
		int_to_char(len, int_field);
		if (fwrite(int_field, 1, 4, fout) != 4) {
			fprintf(stderr, "Error writing.\n");
			return -1;
		}
		int_to_char(enc_final_range, int_field);
		if (fwrite(int_field, 1, 4, fout) != 4) {
			fprintf(stderr, "Error writing.\n");
			return -1;
		}
		if (fwrite(data, 1, len, fout) != (unsigned)len) {
			fprintf(stderr, "Error writing.\n");
			return -1;
		}
		tot_samples += nb_encoded;  //opus数据总长度

		lost_prev = lost;
		if (count >= use_inbandfec) {
			/* count bits */
			bits += len * 8;
			bits_max = (len * 8 > bits_max) ? len * 8 : bits_max;
			bits2 += len * len * 64;
			nrg = 0.0;
			for (k = 0; k < frame_size * channels; k++) {
				nrg += in[k] * (double)in[k];
			}
			nrg /= frame_size * channels;
			if (nrg > 1e5) {
				bits_act += len * 8;
				count_act++;
			}
		}
		count++;
	}
	count -= use_inbandfec;
	if (tot_samples >= 1 && count > 0 && frame_size)
	{
		/* Print out bitrate statistics */
		double var;
		fprintf(stderr, "average bitrate:             %7.3f kb/s\n",
			1e-3*bits*sampling_rate / tot_samples);
		fprintf(stderr, "maximum bitrate:             %7.3f kb/s\n",
			1e-3*bits_max*sampling_rate / frame_size);
		fprintf(stderr, "active bitrate:              %7.3f kb/s\n",
				1e-3*bits_act*sampling_rate / (1e-15 + frame_size * (double)count_act));
		var = bits2 / count - bits * bits / (count*(double)count);
		if (var < 0)
			var = 0;
		fprintf(stderr, "bitrate standard deviation:  %7.3f kb/s\n",
			1e-3*sqrt(var)*sampling_rate / frame_size);
	}
	else {
		fprintf(stderr, "bitrate statistics are undefined\n");
	}

	
	return 0;
}


JNIEXPORT void JNICALL Java_cn_com_test_opus_OpusEncoder_nativeReleaseEncoder(JNIEnv *env, jobject obj)
{
	printf("opus encoder release!!!\n");
	free(enc);
	if (fin)
		fclose(fin);
	if (fout)
		fclose(fout);
	free(in);
	free(fbytes);
	free(data);
}
