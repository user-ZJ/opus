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
    private native void nativeReleaseDecoder(); // �ͷ��ڴ�
    
    /**
     * @methodsName:init
     * @description:����������
     * @param samplingRate ��Ƶ������
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
     * @param frames ����֡��С������ΪsamplingRate*0.02
     * @return ��������ݳ���
     */
    public int decodeShorts(byte[] in, short[] out, int frames) {
    	return nativeDecodeShorts(in,out,frames);
    }
    
    /**
     * @methodsName:decodeBytes
     * @description:��opus���ݽ��н��룬����Ч�����ѣ�������ʹ��
     * @param in opus����
     * @param out pcm����
     * @param frames ����֡��С������ΪsamplingRate*0.02
     * @return ��������ݳ���
     */
    public int decodeBytes(byte[] in, byte[] out, int frames) {
    	return nativeDecodeBytes(in,out,frames);
    }
    
    /**
     * @methodsName:decodeShorts
     * @description:��opus���ݽ��н���
     * @param in opus����
     * @param frames ����֡��С������ΪsamplingRate*0.02
     * @return ���������
     */
    public short [] decodeShorts(byte[] in,int frames) {
    	return nativeDecodeShorts(in,frames);
    }
    
    /**
     * @methodsName:decodeBytes
     * @description:��opus���ݽ��н��룬����Ч�����ѣ�������ʹ��
     * @param in opus����
     * @param frames ����֡��С������ΪsamplingRate*0.02
     * @return ���������
     */
    public byte [] decodeBytes(byte [] in,int frames) {
    	return nativeDecodeBytes(in,frames);
    }
    
    /**
     * @methodsName:release
     * @description:�ͷŽ�����
     */
    public void release() {
    	nativeReleaseDecoder();
    }

}
