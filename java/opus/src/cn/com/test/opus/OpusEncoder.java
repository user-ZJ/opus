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
	 	
	    private native int nativeInitEncoder(int samplingRate,int numberOfChannels,int framesize,int application); // ѹ����ʼ��
	    private native int nativeEncodeShorts(short[] in, byte[] out);// ѹ�����ݣ�����ֵΪѹ����byte���鳤��
	    private native byte [] nativeEncodeShorts(short [] in);  //ѹ������
	    private native void nativeSetBitrate(int bitrate);
	    private native int nativeEncodeFile(String pcmFilePath,String opusFilePath);
	    private native void nativeReleaseEncoder();
	    
	    /**
	    * @methodsName: init
	    * @description: ����opus������ 
	    * @param:numberOfChannels:��Ҫ������Ƶ��ͨ����
	    * @param:application:���뷽ʽ (OPUS_APPLICATION_VOIP/OPUS_APPLICATION_AUDIO/OPUS_APPLICATION_RESTRICTED_LOWDELAY)
	    * @return: int ������룬0��ʾ�����ɹ�
	    * @throws: 
	    */
	    public int init(int samplingRate,int numberOfChannels,int framesize,int application) {
	    	return nativeInitEncoder(samplingRate,numberOfChannels,framesize,application);
	    }
	    

	    /**
	     * @methodsName:encodeShorts
	     * @description:��short֡���б���
	     * @param in ��������
	     * @param out ��������ݴ������
	     * @return ��������ݳ���
	     */
	    public int encodeShorts(short[] in, byte[] out) {
	    	return nativeEncodeShorts(in,out);
	    }
	    
	    
	    /**
	     * @methodsName:encodeShorts
	     * @description:��short���ݽ���opus����
	     * @param in ����pcm����
	     * @param frames ֡��С������ΪsamplingRate*0.02
	     * @return �����opus����
	     */
	    public byte [] encodeShorts(short [] in) {
	    	return nativeEncodeShorts(in);
	    }
	    
	    
	    /**
	     * @methodsName:encodeFile
	     * @description:��pcm��Ƶ�ļ�����Ϊopus��Ƶ�ļ�
	     * @param opusFilePath opus�ļ�·��
	     * @param pcmFilePath pcm�ļ�·��
	     * @return ����״̬��-1��ʾ����
	     */
	    public int encodeFile(String pcmFilePath,String opusFilePath) {
	    	return nativeEncodeFile(pcmFilePath,opusFilePath);
	    }
	    
	    
	    /**
	     * @methodsName:release
	     * @description:�ͷű�����
	     */
	    public void release() {
	    	nativeReleaseEncoder();
	    }


}
