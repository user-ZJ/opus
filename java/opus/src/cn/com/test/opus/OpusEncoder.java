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
	 	
	    private native int nativeInitEncoder(int samplingRate,int numberOfChannels,int application); // ѹ����ʼ��
	    private native int nativeEncodeShorts(short[] in, int frames, byte[] out);// ѹ�����ݣ�����ֵΪѹ����byte���鳤��
	    private native int nativeEncodeBytes(byte[] in, int frames, byte[] out);
	    private native byte [] nativeEncodeShorts(short [] in,int frames);  //ѹ������
	    private native byte [] nativeEncodeBytes(byte [] in,int frames);  //ѹ������
	    private native void nativeReleaseEncoder();
	    
	    /**
	    * @methodsName: init
	    * @description: ����opus������ 
	    * @param:samplingRate:��Ҫ������Ƶ�Ĳ�����
	    * @param:numberOfChannels:��Ҫ������Ƶ��ͨ����
	    * @param:application:���뷽ʽ (OPUS_APPLICATION_VOIP/OPUS_APPLICATION_AUDIO/OPUS_APPLICATION_RESTRICTED_LOWDELAY)
	    * @return: int ������룬0��ʾ�����ɹ�
	    * @throws: 
	    */
	    public int init(int samplingRate,int numberOfChannels,int application) {
	    	return nativeInitEncoder(samplingRate,numberOfChannels,application);
	    }
	    

	    /**
	     * @methodsName:encodeShorts
	     * @description:��short֡���б���
	     * @param in ��������
	     * @param frames ����֡��С������ΪsamplingRate*0.02
	     * @param out ��������ݴ������
	     * @return ��������ݳ���
	     */
	    public int encodeShorts(short[] in, int frames, byte[] out) {
	    	return nativeEncodeShorts(in,frames,out);
	    }
	    
	    /**
	     * @methodsName:encodeBytes
	     * @description:��byte���ݽ���opus����,����Ч�����ѣ�������ʹ��
	     * @param in ����pcm����
	     * @param frames ֡��С������ΪsamplingRate*0.02
	     * @param out �����opus����
	     * @return ��������ݳ���
	     */
	    public int encodeBytes(byte[] in, int frames, byte[] out) {
	    	return nativeEncodeBytes(in, frames, out);
	    }
	    
	    /**
	     * @methodsName:encodeShorts
	     * @description:��short���ݽ���opus����
	     * @param in ����pcm����
	     * @param frames ֡��С������ΪsamplingRate*0.02
	     * @return �����opus����
	     */
	    public byte [] encodeShorts(short [] in,int frames) {
	    	return nativeEncodeShorts(in,frames);
	    }
	    
	    /**
	     * @methodsName:encodeBytes
	     * @description:��byte���ݽ���opus����,����Ч�����ѣ�������ʹ��
	     * @param in ����pcm����
	     * @param frames ֡��С������ΪsamplingRate*0.02
	     * @return �����opus����
	     */
	    public byte [] encodeBytes(byte [] in,int frames) {
	    	return nativeEncodeBytes(in,frames);
	    }
	    
	    /**
	     * @methodsName:release
	     * @description:�ͷű�����
	     */
	    public void release() {
	    	nativeReleaseEncoder();
	    }


}
