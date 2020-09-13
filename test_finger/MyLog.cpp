#include "stdafx.h"

MyMsgQueue<MyString> MyLog::MsgQueue(16);
MyLog::LogLevel limit;

// ׷����־����
void MyLog::AppendLog(const char* text){
    if(conf["AutoLog"]==Stringify(true)){
        MyFile::AppendLog(
            [&](FILE* fp){
                fprintf_s(fp,"%s",text);
            }
        );
    }
}

void MyLog::SetLimit(LogLevel level){
    limit=level;
}

// ���level��info
void MyLog::print(LogLevel level,MyString info){
    // ��־�ȼ��쳣
    if(level>=LOG_HIGHEST){
        ASF_WARNING(1);
        level=LOGD;
    }

    // Խ����Ϣ�����Ϣ�����
    if(level>limit||info==""){
        return;
    }

    if(test.isTest(test.My)){
        test.myTest.My_logs.push_back(info);
        return;
    }

    // ���ӵȼ���ʾ
    MyString pLevel;
    switch(level){
    case LOGU:pLevel="[U]";break;
    case LOGE:pLevel="[E]";break;
    case LOGW:pLevel="[W]";break;
    case LOGD:pLevel="[D]";break;
    case LOGT:pLevel="[T]";break;
    default:pLevel="[Unknown]";break;
    }

    // �����װ�������Ϣ
    MyString content=pLevel+"%s "+info;
    MsgQueue.push(content);
}

char tmp[1<<20];

// ���level��info(printf������)
void MyLog::print(LogLevel level,const char* format,...){
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(level,MyString(tmp));
}

// ���ٶ�������ȼ���log����
#define FastDefineLogName(FuncName,LogLevel)    \
void MyLog::FuncName(MyString info){            \
    print(LogLevel,info);                       \
}                                               \
void MyLog::FuncName(const char * format,...){  \
    va_list ap;                                 \
    va_start(ap,format);                        \
    vsprintf(tmp,format,ap);                    \
    va_end(ap);                                 \
                                                \
    print(LogLevel,MyString(tmp));              \
}

FastDefineLogName(trace,LOGT)
FastDefineLogName(debug,LOGD)
FastDefineLogName(warn,LOGW)
FastDefineLogName(error,LOGE)
FastDefineLogName(user,LOGU)

// ��ǰ�汾
int Version=294;
// ������־,�����´�汾��С�汾�ͷ����¼���汾����ע��
void MyLog::DevelopLog(){
    //user("V0.9 <ʱ��δ֪>:����˴������Ӻ�ͼƬ��ʾ,�������־���ܵĽ���");
    user("V1.0 <2019��03��16��15:36:11>:���ԭʼͼ�������ȡͼ��ť����");
    //user("V1.1 <2019��03��16��15:54:23>:��ɰ�ť����,��ֹ�̳߳�ͻ,��ӿ�����־");
    //user("V1.2 <2019��03��18��00:57:44>:��Ӷ�д�Ĵ���,��ӽ�����,���ѡͼ���С");
    //user("V1.3 <2019��03��24��13:59:42>:��������ù���ɾ��,�Ŵ���ָ��ͼ��,�޸��˰�ť����bug");
    //user("V1.4 <2019��03��24��14:12:08>:�������releaseģʽ��ȡ��warning�Ĵ���,����˴��ļ��а�ť");
    //user("V1.5 <2019��04��10��17:15:45>:�������ȡ��������");
    //user("V1.6 <2019��04��22��22:42:59>:�����˺�о��ȡԭʼͼ����,�޸���ʱ��ͱ༭�����bug");
    user("V2.0 <2019��04��28��17:17:46>:�����˴��ڼܹ�,֧�������ͱ���ģʽ����");
    //user("V2.01<2019��04��29��12:15:13>:�������ھ����������,�޸���һЩ��֪bug");
    user("V2.1 <2019��04��29��16:55:54>:�����˰���ȷ�Ϸ���,����DataPacket��ʶ��Э��");
    user("V2.2 <2019��04��30��14:44:30>:�����˸߼����Թ���");
    user("V2.3 <2019��05��01��20:56:32>:�����µ�Э��ASFComm");
    //user("V2.31<2019��05��01��23:12:42>:������AdjustingImage�����ӿ�");
    user("V2.4 <2019��05��20��01:19:35>:������Syno��Э��,��ע��/�ȶԹ���");
    user("V2.5 <2019��05��20��22:37:36>:������Syno��ɾ��/(����)ȡͼ����,���Ӹ��¹���");
    //user("V2.51<2019��05��24��12:49:24>:�޸�GD32Э����Ĵ���bug,���䲨���ʺ�Э��");
    //user("V2.52<2019��05��26��21:36:00>:����GD32Э���µ�DeviceInfo����,��һ���豸��Ϣ�İ�ť,��ǿ���¹���");
    user("V2.6 <2019��05��27��22:49:28>:�Ż���־������,�����豸��Ϣ����");
    //user("V2.61<2019��05��29��16:29:31>:�޸��豸��Ϣ����ChipVersion bug");
    user("V2.7 <2019��06��12��20:25:27>:����USB���ӷ�ʽ,��ǿ����ɨ�蹦��,��ǿ���潡׳��,�Ż�����ṹ");
    user("V2.8 <2019��06��25��09:09:29>:�����ð����˳���,���û���������.����ͼ�������С�Ĺ���");
    //user("V2.81<2019��07��09��20:52:42>:�޸���Synoͨ�ŷ����bug");
    user("V2.9 <2019��07��21��20:59:21>:����Test�Զ���������,�����ǿ���Լ�ָ������");
    user("V2.91<2019��08��10��11:25:25>:�޸���һЩ��֪bug,����DownChar����");
    user("V2.92<2019��08��18��16:48:55>:�����Զ����DevInfo����,����Search,ControlLED����");
    user("V2.93<2020��06��11��18:07:11>:ɾ��3��ESD������");
    user("V2.94<2020��06��16��23:31:19>:֧��88*112ͼ���С");
}
