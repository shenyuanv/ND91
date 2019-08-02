#pragma  once

#include "Module/Call/CallData.h"

namespace ND91Assistant
{
    // ����ͨ����¼����
    class AddCallDataCommand : public BaseCommand
    {
    public:
        AddCallDataCommand(const DeviceData* pDevice);
        virtual ~AddCallDataCommand();

        // ִ������
        virtual void Execute();

        // װ������ӵĺ��м�¼
        void SetDataList(const vector<const CallData*>* DataList);

        // ����Android�������ݰ�
        virtual std::string GetAndroidBodyData();

        // ����Android���ؽ�����ݰ�
        virtual void SetAndroidReturnData(std::string data);

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename) { }

        // ����ִ�н�����Ƿ�ɹ�
        bool Success();

        int  ExecuteCount() { return _executeCount;};

    private:

        // ִ�а�׿����
        bool AndroidExecute();
        
        // ִ��IOS����
        bool IosExecute();

        // ����δִ�����ݵ�״̬
        void ResetDataStatus();

    private: 
         
        // ��Ҫ���ӵ�ͨ����¼����SetDataList�������ã�
        const vector<const CallData*>* _dataList;
        
        // ��ִ�����������ͨ����¼����
        vector<const CallData*> _executeDataList;
        int _executeCount;          // ʵ��ִ�е�����
        int _executeCountEveryTime; // ÿ��ִ�е�����

        int _maxCount; // ÿ�������������ӵ��������

        // ����ִ���Ƿ�ɹ�
        bool _success;
    };
}