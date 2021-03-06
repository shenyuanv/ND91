#include "stdafx.h"

const std::string _Asf_Header_Object				= "75B22630-668E-11CF-A6D9-00AA0062CE6C";
const std::string _Asf_File_Properties_Object		= "8CABDCA1-A947-11CF-8EE4-00C00C205365";
const std::string _Asf_Stream_Properties_Object		= "B7DC0791-A9B7-11CF-8EE6-00C00C205365";
const std::string _Asf_Header_Extension_Object		= "5FBF03B5-A92E-11CF-8EE3-00C00C205365";
const std::string _Asf_Codec_List_Object			= "86D15240-311D-11D0-A3A4-00A0C90348F6";
const std::string _Asf_Extended_Description_Object	= "D2D0A440-E307-11D2-97F0-00A0C95EA850";

WmvParser::WmvParser( const wstring path )
: MediaBase(path)
{
	_pos		= 0;		
	_header_num	= 0;
	_bufsize	= 0;
	
}

WmvParser::~WmvParser(void)
{

}


bool ND91Assistant::WmvParser::ParserFormat()
{
	RETURN_FALSE_IF ( !Verification() );

	BinaryBufferReader binaryBuf( _path );
	binaryBuf.SetOffset(_pos);

	//read header object
	for (int i = 0; i < _header_num; i++)
	{
		unsigned char buf[16] = {0};
		binaryBuf.ReadBuffer((char*)buf, 16);

		std::string strGuid = BufferToGuidString(buf);

		__int64 objectSize; 		
		binaryBuf.ReadInt64(objectSize);
		_pos = binaryBuf.GetOffset();

		long nextPosition = _pos + (int)objectSize - 24;

		if ( strGuid == _Asf_Codec_List_Object)
		{
		    return Read_ASF_Codec_List_Object(&binaryBuf);
		}

		binaryBuf.SetOffset(nextPosition);
	}

	return false;
	
}


long ND91Assistant::WmvParser::GetMediaDuration()
{
	RETURN_VALUE_IF (!Verification(), 0);

	BinaryBufferReader binaryBuf( _path );
	binaryBuf.SetOffset(_pos);

	for (int i = 0; i < _header_num; i++)
	{
		unsigned char buf[16] = {0};
		binaryBuf.ReadBuffer((char*)buf, 16);

		std::string strGuid = BufferToGuidString(buf);

		__int64 objectSize; 		
		binaryBuf.ReadInt64(objectSize);
		_pos = binaryBuf.GetOffset();

		long nextPosition = _pos + (int)objectSize - 24;

		if ( strGuid == _Asf_File_Properties_Object)
			return Read_ASF_File_Properties_Object(&binaryBuf);

		binaryBuf.SetOffset(nextPosition);

		if ( nextPosition >= _bufsize )
			break;
	}

	return 0;
}

long ND91Assistant::WmvParser::Read_ASF_File_Properties_Object(BinaryBufferReader* binaryBuf)
{
	binaryBuf->SetOffset(_pos + 40);

	__int64 play_duration, send_duration, preroll;

	RETURN_VALUE_IF( !binaryBuf->ReadInt64(play_duration), 0);
	RETURN_VALUE_IF( !binaryBuf->ReadInt64(send_duration), 0);
	RETURN_VALUE_IF( !binaryBuf->ReadInt64(preroll), 0);

	long duration = (int)( send_duration / 10000000 - 1.3);
	
	return duration;
}

bool ND91Assistant::WmvParser::Read_ASF_Codec_List_Object(BinaryBufferReader* binaryBuf)
{
	int pos = binaryBuf->GetOffset();
	binaryBuf->SetOffset(pos + 16);

	__int32 length;
	binaryBuf->ReadInt32(length);

	for (int i = 0; i < length; i++)
	{
		short type, name_length, description_length, information_length;
		std::string strname, strdescriptions;	

		binaryBuf->ReadShort(type);
		binaryBuf->ReadShort(name_length);
		char* names = new char[name_length*2];
		binaryBuf->ReadBuffer( names, name_length * 2 );
		strname = names;
		binaryBuf->ReadShort(description_length);
		char* descriptions = new char[description_length*2];
		binaryBuf->ReadBuffer( descriptions, description_length * 2 );
		strdescriptions = descriptions;
		binaryBuf->ReadShort(information_length);
		char* informations = new char[description_length];
		binaryBuf->ReadBuffer(informations, information_length);

		if (type == 2)
			return true;
		else if (type == 1)
			return true;
		else
			return false;
	}
	return true;
}

void ND91Assistant::WmvParser::ReadASF_Extended_Content_Description_Object()
{

}

std::string ND91Assistant::WmvParser::BufferToGuidString( unsigned char* buf )
{
	RETURN_VALUE_IF ( NULL == buf, "");

	char buff[64] = {0};
	_snprintf_s(
		buff,
		sizeof(buff),
		"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		buf[3], buf[2], buf[1], buf[0], buf[5], buf[4], buf[7], buf[6], 
		buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);

	std::string strGuid = buff;

	return strGuid;
}

bool ND91Assistant::WmvParser::Verification()
{
	_media->_fileSize = CFileOperation::GetFileSizeW(_path);

	BinaryBufferReader binaryBuf( _path );

	unsigned char buf[16] = {0};
	binaryBuf.ReadBuffer((char*)buf, 16);
	std::string strGuid = BufferToGuidString(buf);

	binaryBuf.ReadInt64(_bufsize);

	if (strGuid == _Asf_Header_Object)
	{
		binaryBuf.ReadInt32(_header_num);
		_pos = binaryBuf.GetOffset() + 2;
		binaryBuf.SetOffset(_pos);

		return true;
	}
	else
		return false;
	
}