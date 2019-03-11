package cn.com.test.opus;

public class OpusDecoder {
	
	
	static {
        System.loadLibrary("opusjni");
    }
	
	private native int nativeInitDecoder(int samplingRate, int numberOfChannels);
    private native int nativeDecodeShorts(byte[] in, short[] out, int frames);
    private native int nativeDecodeBytes(byte[] in, byte[] out, int frames);
    private native short [] nativeDecodeShorts(byte[] in,int frames); 
    private native byte [] nativeDecodeBytes(byte [] in,int frames);
    private native void nativeReleaseDecoder(); // 释放内存
    
    /**
     * @methodsName:init
     * @description:创建解码器
     * @param samplingRate 音频采样率
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
     * @param frames 输入帧大小，建议为samplingRate*0.02
     * @return 解码后数据长度
     */
    public int decodeShorts(byte[] in, short[] out, int frames) {
    	return nativeDecodeShorts(in,out,frames);
    }
    
    /**
     * @methodsName:decodeBytes
     * @description:对opus数据进行解码，解码效果不佳，不建议使用
     * @param in opus数据
     * @param out pcm数据
     * @param frames 输入帧大小，建议为samplingRate*0.02
     * @return 解码后数据长度
     */
    public int decodeBytes(byte[] in, byte[] out, int frames) {
    	return nativeDecodeBytes(in,out,frames);
    }
    
    /**
     * @methodsName:decodeShorts
     * @description:对opus数据进行解码
     * @param in opus数据
     * @param frames 输入帧大小，建议为samplingRate*0.02
     * @return 解码后数据
     */
    public short [] decodeShorts(byte[] in,int frames) {
    	return nativeDecodeShorts(in,frames);
    }
    
    /**
     * @methodsName:decodeBytes
     * @description:对opus数据进行解码，解码效果不佳，不建议使用
     * @param in opus数据
     * @param frames 输入帧大小，建议为samplingRate*0.02
     * @return 解码后数据
     */
    public byte [] decodeBytes(byte [] in,int frames) {
    	return nativeDecodeBytes(in,frames);
    }
    
    /**
     * @methodsName:release
     * @description:释放解码器
     */
    public void release() {
    	nativeReleaseDecoder();
    }

}
