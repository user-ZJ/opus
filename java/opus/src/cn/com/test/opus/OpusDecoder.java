package cn.com.test.opus;

public class OpusDecoder {
	
	
	static {
        System.loadLibrary("opusjni");
    }
	
	private native int nativeInitDecoder(int samplingRate, int numberOfChannels);
    private native int nativeDecodeShorts(byte[] in, short[] out);
    private native short [] nativeDecodeShorts(byte[] in); 
    private native int nativeDecodeFile(String opusFilePath,String pcmFilePath);
    private native void nativeReleaseDecoder(); // �ͷ��ڴ�
    
    /**
     * @methodsName:init
     * @description:����������
     * @param numberOfChannels ��Ƶͨ����
     * @return ������룬0��ʾ�����ɹ�
     */
    public int init(int samplingRate, int numberOfChannels) {
    	return nativeInitDecoder(samplingRate,numberOfChannels);
    }
    
    /**
     * @methodsName:decodeShorts
     * @description:��opus���ݽ��н���
     * @param in opus����
     * @param out pcm����
     * @return ��������ݳ���
     */
    public int decodeShorts(byte[] in, short[] out) {
    	return nativeDecodeShorts(in,out);
    }
    
   
    /**
     * @methodsName:decodeShorts
     * @description:��opus���ݽ��н���
     * @param in opus����
     * @param frames ����֡��С������ΪsamplingRate*0.02
     * @return ���������
     */
    public short [] decodeShorts(byte[] in) {
    	return nativeDecodeShorts(in);
    }
    
    /**
     * @methodsName:decodeFile
     * @description:��opus��Ƶ�ļ�����Ϊpcm��Ƶ�ļ�
     * @param opusFilePath opus�ļ�·��
     * @param pcmFilePath pcm�ļ�·��
     * @return ����״̬��-1��ʾ����
     */
    public int decodeFile(String opusFilePath,String pcmFilePath) {
    	return nativeDecodeFile(opusFilePath,pcmFilePath);
    }
    
    
    /**
     * @methodsName:release
     * @description:�ͷŽ�����
     */
    public void release() {
    	nativeReleaseDecoder();
    }

}
