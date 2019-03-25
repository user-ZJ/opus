package cn.com.test.opus;

public class OpusDecoder {
	
	
	static {
        System.loadLibrary("opusjni");
    }
	
	private native int nativeInitDecoder(int samplingRate, int numberOfChannels);
    private native int nativeDecodeShorts(byte[] in, short[] out);
    private native short [] nativeDecodeShorts(byte[] in); 
    private native int nativeDecodeFile(String opusFilePath,String pcmFilePath);
    private native void nativeReleaseDecoder(); // 释放内存
    
    /**
     * @methodsName:init
     * @description:创建解码器
     * @param numberOfChannels 音频通道数
     * @return 错误编码，0表示创建成功
     */
    public int init(int samplingRate, int numberOfChannels) {
    	return nativeInitDecoder(samplingRate,numberOfChannels);
    }
    
    /**
     * @methodsName:decodeShorts
     * @description:对opus数据进行解码
     * @param in opus数据
     * @param out pcm数据
     * @return 解码后数据长度
     */
    public int decodeShorts(byte[] in, short[] out) {
    	return nativeDecodeShorts(in,out);
    }
    
   
    /**
     * @methodsName:decodeShorts
     * @description:对opus数据进行解码
     * @param in opus数据
     * @param frames 输入帧大小，建议为samplingRate*0.02
     * @return 解码后数据
     */
    public short [] decodeShorts(byte[] in) {
    	return nativeDecodeShorts(in);
    }
    
    /**
     * @methodsName:decodeFile
     * @description:将opus音频文件解码为pcm音频文件
     * @param opusFilePath opus文件路径
     * @param pcmFilePath pcm文件路径
     * @return 解码状态，-1表示错误
     */
    public int decodeFile(String opusFilePath,String pcmFilePath) {
    	return nativeDecodeFile(opusFilePath,pcmFilePath);
    }
    
    
    /**
     * @methodsName:release
     * @description:释放解码器
     */
    public void release() {
    	nativeReleaseDecoder();
    }

}
