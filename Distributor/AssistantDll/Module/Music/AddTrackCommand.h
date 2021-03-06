#pragma once

namespace ND91Assistant
{
class AndroidMusicManager;
class AudioData;

// 添加歌曲
class AddTrackCommand :public BaseCommand
{
public:
	AddTrackCommand(const DeviceData* pDevice, AndroidMusicManager* pMusic);
	virtual ~AddTrackCommand(void);

	// Summary  : 设置命令参数
	// Returns	: void 
	// Parameter: wstring localpath歌曲本地路径
	// Parameter: wstring remotepath歌曲必已上传，上传后的歌曲手机端路径
	void SetParam(wstring localpath, wstring remotepath);

	// 执行命令
	virtual void Execute();

	AudioData* GetReturnAudio(){return _pReturnAudio;}

protected:

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

	// 命令传送到IOS设备上的文件名（全路径, unicode）
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// 从IOS上获取的命令结果文件名（全路径, unicode）
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename) { }


private:
	AndroidMusicManager*  _pMusic;

	wstring        _LocalPath;

	wstring        _RemotePath;

	AudioData*     _pReturnAudio;
};

}
