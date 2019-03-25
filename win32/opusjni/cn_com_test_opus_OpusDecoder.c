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
#include "opus_defines.h"
#include "opus_types.h"

#define MAX_PACKET 1500

static int max_frame_size = 48000 * 2;
OpusDecoder *dec = NULL;
static FILE *fin = NULL;
static FILE *fout = NULL;
static unsigned char *data = NULL;
static unsigned char *fbytes = NULL;
static short *out = NULL;
int frame_size, channels;  //帧大小（编码参数），通道数
opus_int32 sampling_rate;  //音频采样率
int max_payload_bytes; //最大载荷字节数，编码参数，1500


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


JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusDecoder_nativeInitDecoder
(JNIEnv *env, jobject obj, jint samplingRate, jint numberOfChannels)
{
	printf("opus decode init !\n");
	int size;
	int error;
	channels = numberOfChannels;
	sampling_rate = samplingRate;
	/* defaults: */
	max_payload_bytes = MAX_PACKET; //最大有效负载 1500
	frame_size = sampling_rate / 50;
	size = opus_decoder_get_size(numberOfChannels);
	dec = malloc(size);
	error = opus_decoder_init(dec, samplingRate, numberOfChannels);
	return error;
}

JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusDecoder_nativeDecodeShorts___3B_3S
(JNIEnv *env, jobject obj, jbyteArray in, jshortArray out)
{
	jint inputArraySize = (*env)->GetArrayLength(env, in);

	jbyte* opusSignal = (*env)->GetByteArrayElements(env, in, 0);
	jshort* pcmSignal = (*env)->GetShortArrayElements(env, out, 0);
	int dataArraySize = opus_decode(dec, opusSignal, inputArraySize,pcmSignal, frame_size, 0);
	(*env)->ReleaseByteArrayElements(env, in, opusSignal, JNI_ABORT);
	(*env)->ReleaseShortArrayElements(env, out, pcmSignal, 0);

	return dataArraySize;
}

JNIEXPORT jshortArray JNICALL Java_cn_com_test_opus_OpusDecoder_nativeDecodeShorts___3B
(JNIEnv *env, jobject obj, jbyteArray in)
{
	jint inputArraySize = (*env)->GetArrayLength(env, in);
	jbyte* opusSignal = (*env)->GetByteArrayElements(env, in, 0);
	jshortArray encodedData = (*env)->NewShortArray(env, frame_size * 2);
	jshort* encodedSignal = (*env)->GetShortArrayElements(env, encodedData, 0);
	int dataArraySize = opus_decode(dec, opusSignal, inputArraySize, encodedSignal, frame_size, 0);
	jshortArray pcmData = (*env)->NewShortArray(env, dataArraySize);
	(*env)->SetShortArrayRegion(env, pcmData, 0, dataArraySize, encodedSignal);
	(*env)->ReleaseByteArrayElements(env, in, opusSignal, JNI_ABORT);
	(*env)->ReleaseShortArrayElements(env, encodedData, encodedSignal, 0);
	return pcmData;
}

JNIEXPORT jint JNICALL Java_cn_com_test_opus_OpusDecoder_nativeDecodeFile
(JNIEnv *env, jobject obj, jstring opusFilePath, jstring pcmFilePath) {
	int stop = 0;
	size_t num_read;
	int len;
	opus_uint32 enc_final_range; //最终范围
	

	opus_int32 output_samples;
	int lost = 0, lost_prev = 1;
	int packet_loss_perc=0;  ////模拟丢包百分比
	opus_int32 count = 0, count_act = 0;  //帧计算器
	opus_int32 skip = 0;
	opus_uint64 tot_in=0, tot_out=0; //解码数据（PCM）总数
	double tot_samples = 0;  //opus数据总长度
	opus_uint32 dec_final_range;
	double bits = 0.0, bits_max = 0.0, bits_act = 0.0, bits2 = 0.0;  //用于计算比特率
	int err = 0;

	const char *pcmFile = (*env)->GetStringUTFChars(env, pcmFilePath, 0);
	const char *opusFile = (*env)->GetStringUTFChars(env, opusFilePath, 0);

	err = fopen_s(&fin,opusFile, "rb");
	if (err)
	{
		fprintf(stderr, "Could not open input file %s\n", opusFile);
		return -1;
	}
	err = fopen_s(&fout,pcmFile, "wb+");
	if (err)
	{
		fprintf(stderr, "Could not open output file %s\n", pcmFile);
		return -1;
	}
	fprintf(stderr, "Decoding with %ld Hz output (%d channels)\n",(long)sampling_rate, channels);
	out = (short*)malloc(max_frame_size*channels * sizeof(short)); //输出buff，48000*2*channels*2
	fbytes = (unsigned char*)malloc(max_frame_size*channels * sizeof(short));  //从pcm文件中按字节读取的数据
	data = (unsigned char*)calloc(max_payload_bytes, sizeof(unsigned char));  //分配1500*2byte内存
	while (!stop) {
		unsigned char ch[4];
		num_read = fread(ch, 1, 4, fin);
		if (num_read != 4)
			break;
		len = char_to_int(ch);  //4byte->int,单帧数据长度（负载）
		if (len>max_payload_bytes || len<0)
		{
			fprintf(stderr, "Invalid payload length: %d\n", len);
			break;
		}
		num_read = fread(ch, 1, 4, fin);
		if (num_read != 4)
			break;
		enc_final_range = char_to_int(ch);  //编码最终范围
		num_read = fread(data, 1, len, fin);
		if (num_read != (size_t)len)
		{
			fprintf(stderr, "Ran out of input, "
				"expecting %d bytes got %d\n",
				len, (int)num_read);
			break;
		}
		lost = len == 0 || (packet_loss_perc>0 && rand() % 100 < packet_loss_perc); //是否有数据丢失
		if (lost)
			opus_decoder_ctl(dec, OPUS_GET_LAST_PACKET_DURATION(&output_samples));
		else
			output_samples = max_frame_size;  //48000 * 2
		if (count >= 0) {
			output_samples = opus_decode(dec, lost ? NULL : data, len, out, output_samples, 0);
			if (output_samples>0)
			{
				if (output_samples>skip) {
					int i;
					for (i = 0; i<(output_samples - skip)*channels; i++)
					{
						//shot->byte
						short s;
						s = out[i + (skip*channels)];
						fbytes[2 * i] = s & 0xFF;
						fbytes[2 * i + 1] = (s >> 8) & 0xFF;
					}
					if (fwrite(fbytes, sizeof(short)*channels, output_samples - skip, fout) != (unsigned)(output_samples - skip)) {
						fprintf(stderr, "Error writing.\n");
						return -1;
					}
					tot_out += output_samples - skip;  //解码数据总数
				}
				if (output_samples<skip) skip -= output_samples;
				else skip = 0;
			}
			else {
				fprintf(stderr, "error decoding frame: %s\n",
					opus_strerror(output_samples));
			}
			tot_samples += output_samples; //解码数据总数
		}

		opus_decoder_ctl(dec, OPUS_GET_FINAL_RANGE(&dec_final_range));
		if (enc_final_range != 0 && !lost && !lost_prev	&& dec_final_range != enc_final_range) {
			fprintf(stderr, "Error: Range coder state mismatch "
				"between encoder and decoder "
				"in frame %ld: 0x%8lx vs 0x%8lx\n",
				(long)count,
				(unsigned long)enc_final_range,
				(unsigned long)dec_final_range);
			return -1;
		}
		lost_prev = lost;
		if (count >= 0) {
			/* count bits */
			bits += len * 8;
			bits_max = (len * 8 > bits_max) ? len * 8 : bits_max;
			bits2 += len * len * 64;
		}
		count++;
	}

	if (count > 0)
		frame_size = (int)(tot_samples / count);
	if (tot_samples >= 1 && count > 0 && frame_size)
	{
		/* Print out bitrate statistics */
		double var;
		fprintf(stderr, "average bitrate:             %7.3f kb/s\n",
			1e-3*bits*sampling_rate / tot_samples);
		fprintf(stderr, "maximum bitrate:             %7.3f kb/s\n",
			1e-3*bits_max*sampling_rate / frame_size);
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


JNIEXPORT void JNICALL Java_cn_com_test_opus_OpusDecoder_nativeReleaseDecoder
(JNIEnv *env, jobject obj) 
{
	printf("opus decoder release!!!\n");
	free(dec);
	if (fin)
		fclose(fin);
	if (fout)
		fclose(fout);
	free(data);
	free(fbytes);
	free(out);
}
