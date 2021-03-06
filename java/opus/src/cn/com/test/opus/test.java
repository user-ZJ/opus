package cn.com.test.opus;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

public class test {
	
	static final int SAMPLE_RATE = 8000;
	static final int FRAME_SIZE = 160;  //输入帧大小，建议为samplingRate*0.02
	static final int NUM_CHANNELS = 1;

	public static void main(String [] args) {
		OpusEncoder encoder = new OpusEncoder();
		OpusDecoder decoder = new OpusDecoder();
		int enError = encoder.init(SAMPLE_RATE, NUM_CHANNELS, FRAME_SIZE,OpusEncoder.OPUS_APPLICATION_AUDIO);
		int deError = decoder.init(SAMPLE_RATE, NUM_CHANNELS);
		File pcmFile = new File("src", "demo.pcm");
		File PcmFile_short = new File("src", "encodedshort.pcm");
		File PcmFile_byte = new File("src", "encodedbyte.pcm");
		byte [] buf = new byte[FRAME_SIZE*NUM_CHANNELS*2];
		int lenth = 0;
		try {
			FileInputStream fileInputStream = new FileInputStream(pcmFile);
			FileOutputStream fileOutputStream_short = new FileOutputStream(PcmFile_short);
			FileOutputStream fileOutputStream_byte = new FileOutputStream(PcmFile_byte);
			while ((lenth = fileInputStream.read(buf)) != -1) {
				//encode:short->byte decode:byte->short 
				short [] bs = BytesTransUtil.getInstance().Bytes2Shorts(buf);
				byte [] encoded = encoder.encodeShorts(bs);
				short [] decoded = decoder.decodeShorts(encoded);
				fileOutputStream_short.write(BytesTransUtil.getInstance().Shorts2Bytes(decoded));
			}
			fileOutputStream_short.close();
			fileOutputStream_byte.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//int res = encoder.encodeFile("E:\\project\\eclipse\\java\\opus\\src\\demo.pcm","E:\\project\\eclipse\\java\\opus\\src\\jdemo.opus");
		int res = decoder.decodeFile("E:\\project\\eclipse\\java\\opus\\src\\jdemo.opus","E:\\project\\eclipse\\java\\opus\\src\\jdemo.pcm");
		
		/*try {
			FileInputStream fileInputStream = new FileInputStream(pcmFile);
			FileOutputStream fileOutputStream_short = new FileOutputStream(PcmFile_short);
			FileOutputStream fileOutputStream_byte = new FileOutputStream(PcmFile_byte);
			byte [] encBuf = new byte[1024];
			byte [] decBuf = new byte[FRAME_SIZE*NUM_CHANNELS*2];
			int encoded_size;
			int decoded_size;
			byte[] encBuf2;
			while ((lenth = fileInputStream.read(buf)) != -1) {
				//encode:short->byte decode:byte->short 
				short [] bs = BytesTransUtil.getInstance().Bytes2Shorts(buf);
				short [] bs_dec = BytesTransUtil.getInstance().Bytes2Shorts(decBuf);
				encoded_size = encoder.encodeShorts(bs, FRAME_SIZE,encBuf);
				encBuf2 = Arrays.copyOf(encBuf, encoded_size);
				decoded_size = decoder.decodeShorts(encBuf2, bs_dec,FRAME_SIZE);
				short [] bs_dec2 = Arrays.copyOf(bs_dec, decoded_size);
				fileOutputStream_short.write(BytesTransUtil.getInstance().Shorts2Bytes(bs_dec2));
			}
			fileOutputStream_short.close();
			fileOutputStream_byte.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
		
		encoder.release();
		decoder.release();
		
		
	}
	
}
