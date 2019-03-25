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
	 	
	    private native int nativeInitEncoder(int samplingRate,int numberOfChannels,int framesize,int application); // 压缩初始化
	    private native int nativeEncodeShorts(short[] in, byte[] out);// 压缩数据，返回值为压缩后byte数组长度
	    private native byte [] nativeEncodeShorts(short [] in);  //压缩数据
	    private native void nativeSetBitrate(int bitrate);
	    private native int nativeEncodeFile(String pcmFilePath,String opusFilePath);
	    private native void nativeReleaseEncoder();
	    
	    /**
	    * @methodsName: init
	    * @description: 创建opus编码器 
	    * @param:numberOfChannels:需要编码音频的通道数
	    * @param:application:编码方式 (OPUS_APPLICATION_VOIP/OPUS_APPLICATION_AUDIO/OPUS_APPLICATION_RESTRICTED_LOWDELAY)
	    * @return: int 错误编码，0表示创建成功
	    * @throws: 
	    */
	    public int init(int samplingRate,int numberOfChannels,int framesize,int application) {
	    	return nativeInitEncoder(samplingRate,numberOfChannels,framesize,application);
	    }
	    

	    /**
	     * @methodsName:encodeShorts
	     * @description:对short帧进行编码
	     * @param in 输入数据
	     * @param out 编码后数据存放数组
	     * @return 编码后数据长度
	     */
	    public int encodeShorts(short[] in, byte[] out) {
	    	return nativeEncodeShorts(in,out);
	    }
	    
	    
	    /**
	     * @methodsName:encodeShorts
	     * @description:对short数据进行opus编码
	     * @param in 输入pcm数据
	     * @param frames 帧大小，建议为samplingRate*0.02
	     * @return 编码后opus数据
	     */
	    public byte [] encodeShorts(short [] in) {
	    	return nativeEncodeShorts(in);
	    }
	    
	    
	    /**
	     * @methodsName:encodeFile
	     * @description:将pcm音频文件解码为opus音频文件
	     * @param opusFilePath opus文件路径
	     * @param pcmFilePath pcm文件路径
	     * @return 解码状态，-1表示错误
	     */
	    public int encodeFile(String pcmFilePath,String opusFilePath) {
	    	return nativeEncodeFile(pcmFilePath,opusFilePath);
	    }
	    
	    
	    /**
	     * @methodsName:release
	     * @description:释放编码器
	     */
	    public void release() {
	    	nativeReleaseEncoder();
	    }


}
