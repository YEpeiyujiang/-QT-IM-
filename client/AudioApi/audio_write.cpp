#include "audio_write.h"

Audio_Write::Audio_Write(QObject *parent) : QObject(parent)
{
    //首先初始化设备
    //声卡采样格式
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        QMessageBox::information(NULL , "提示", "打开音频设备失败");
        format = info.nearestFormat(format);
    }

    /* QAudioOutput* */audio_out    = new QAudioOutput(format, this);
    /*QIODevice* */myBuffer_out     = audio_out->start();
    //向 QIODevice 写入数据 写什么数据 audio_out就播放什么

    ///解码器初始化
    speex_bits_init(&bits_dec);
    Dec_State = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));

}

Audio_Write::~Audio_Write()
{
    delete audio_out;
}

void Audio_Write::slot_net_rx(QByteArray ba)
{
#ifndef USE_SPEEX
    myBuffer_out->write( ba.data() , 640 );
#else
    //解码 得到 320字节 大端转为小端  总共640字节 执行2次
    char bytes[800] = {0};
    int i = 0;
    float output_frame1[320] = {0};
    char buf[800] = {0};
    memcpy(bytes,ba.data(),ba.length() / 2);
    //解压缩数据 106 62
    //speex_bits_reset(&bits_dec);
    speex_bits_read_from(&bits_dec,bytes,ba.length() / 2);
    int error = speex_decode(Dec_State,&bits_dec,output_frame1);
    //将解压后数据转换为声卡识别格式
    //大端转化为小端
    short num = 0;
    for (i = 0;i < 160;i++)
    {
        num = output_frame1[i];
        buf[2 * i] = num;
        buf[2 * i + 1] = num >> 8;
        //qDebug() << "float out" << num << output_frame1[i];
    }
    memcpy(bytes,ba.data() + ba.length() / 2,ba.length() / 2);
    //解压缩数据
    //speex_bits_reset(&bits_dec);
    speex_bits_read_from(&bits_dec,bytes,ba.length() / 2);
    error = speex_decode(Dec_State,&bits_dec,output_frame1);
    //将解压后数据转换为声卡识别格式
    //大端
    for (i = 0;i < 160;i++)
    {
        num = output_frame1[i];
        buf[2 * i + 320] = num;
        buf[2 * i + 1 + 320] = num >> 8;
    }
    myBuffer_out->write(buf,640);
    return;
#endif
}


