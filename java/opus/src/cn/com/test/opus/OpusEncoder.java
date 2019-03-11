package cn.com.test.opus;

import java.util.Arrays;

/**
 * @author zhanjie
 *
 */
public class OpusEncoder {
	
	
	 	static {
	        System.loadLibrary("opusjni");
	    }
	 	
	 	// Native pointer to OpusEncoder.
	    private long address;
	 	
	 	public static final int OPUS_APPLICATION_VOIP                = 2048;
	    public static final int OPUS_APPLICATION_AUDIO               = 2049;
	    public static final int OPUS_APPLICATION_RESTRICTED_LOWDELAY = 2051;
	 	
	    private native int nativeInitEncoder(int samplingRate,int numberOfChannels,int application); // 压缩初始化
	    private native int nativeEncodeShorts(short[] in, int frames, byte[] out);// 压缩数据，返回值为压缩后byte数组长度
	    private native int nativeEncodeBytes(byte[] in, int frames, byte[] out);
	    private native byte [] nativeEncodeShorts(short [] in,int frames);  //压缩数据
	    private native byte [] nativeEncodeBytes(byte [] in,int frames);  //压缩数据
	    private native void nativeReleaseEncoder();
	    
	    /**
	    * @methodsName: init
	    * @description: 创建opus编码器 
	    * @param:samplingRate:需要编码音频的采样率
	    * @param:numberOfChannels:需要编码音频的通道数
	    * @param:application:编码方式 (OPUS_APPLICATION_VOIP/OPUS_APPLICATION_AUDIO/OPUS_APPLICATION_RESTRICTED_LOWDELAY)
	    * @return: int 错误编码，0表示创建成功
	    * @throws: 
	    */
	    public int init(int samplingRate,int numberOfChannels,int application) {
	    	return nativeInitEncoder(samplingRate,numberOfChannels,application);
	    }
	    

	    /**
	     * @methodsName:encodeShorts
	     * @description:对short帧进行编码
	     * @param in 输入数据
	     * @param frames 输入帧大小，建议为samplingRate*0.02
	     * @param out 编码后数据存放数组
	     * @return 编码后数据长度
	     */
	    public int encodeShorts(short[] in, int frames, byte[] out) {
	    	return nativeEncodeShorts(in,frames,out);
	    }
	    
	    /**
	     * @methodsName:encodeBytes
	     * @description:对byte数据进行opus编码,编码效果不佳，不建议使用
	     * @param in 输入pcm数据
	     * @param frames 帧大小，建议为samplingRate*0.02
	     * @param out 编码后opus数据
	     * @return 编码后数据长度
	     */
	    public int encodeBytes(byte[] in, int frames, byte[] out) {
	    	return nativeEncodeBytes(in, frames, out);
	    }
	    
	    /**
	     * @methodsName:encodeShorts
	     * @description:对short数据进行opus编码
	     * @param in 输入pcm数据
	     * @param frames 帧大小，建议为samplingRate*0.02
	     * @return 编码后opus数据
	     */
	    public byte [] encodeShorts(short [] in,int frames) {
	    	return nativeEncodeShorts(in,frames);
	    }
	    
	    /**
	     * @methodsName:encodeBytes
	     * @description:对byte数据进行opus编码,编码效果不佳，不建议使用
	     * @param in 输入pcm数据
	     * @param frames 帧大小，建议为samplingRate*0.02
	     * @return 编码后opus数据
	     */
	    public byte [] encodeBytes(byte [] in,int frames) {
	    	return nativeEncodeBytes(in,frames);
	    }
	    
	    /**
	     * @methodsName:release
	     * @description:释放编码器
	     */
	    public void release() {
	    	nativeReleaseEncoder();
	    }


}
