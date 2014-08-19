#ifndef _DYNAMIC_PROTO_H_
#define _DYNAMIC_PROTO_H_

#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/compiler/importer.h"
#include <map>

class CDynamicProto : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
	static CDynamicProto& Instance()
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CDynamicProto();
		}
		return *m_pInstance;
	}
private:
	CDynamicProto() : 
		m_tree(),
		m_importer( &m_tree, this )
	{
	}
	static CDynamicProto *m_pInstance;

public:
	void Init(const std::string &protoDir);
	google::protobuf::Message* CreateMessage(const std::string &protofile, const std::string &name);
	google::protobuf::Message* MutableMessage(google::protobuf::Message *parent, const std::string &field);
	google::protobuf::Message* AddMessage(google::protobuf::Message *parent, const std::string &field);

	bool SetInt32 (google::protobuf::Message *message, const std::string &field, google::protobuf::int32  value);
	bool SetInt64 (google::protobuf::Message *message, const std::string &field, google::protobuf::int64  value);
	bool SetUInt32(google::protobuf::Message *message, const std::string &field, google::protobuf::uint32 value);
	bool SetUInt64(google::protobuf::Message *message, const std::string &field, google::protobuf::uint64 value);
	bool SetFloat (google::protobuf::Message *message, const std::string &field, float  value);
	bool SetDouble(google::protobuf::Message *message, const std::string &field, double value);
	bool SetBool  (google::protobuf::Message *message, const std::string &field, bool   value);
	bool SetString(google::protobuf::Message *message, const std::string &field, const std::string& value);
	
	bool AddInt32 (google::protobuf::Message *message, const std::string &field, google::protobuf::int32  value);
	bool AddInt64 (google::protobuf::Message *message, const std::string &field, google::protobuf::int64  value);
	bool AddUInt32(google::protobuf::Message *message, const std::string &field, google::protobuf::uint32 value);
	bool AddUInt64(google::protobuf::Message *message, const std::string &field, google::protobuf::uint64 value);
	bool AddFloat (google::protobuf::Message *message, const std::string &field, float  value);
	bool AddDouble(google::protobuf::Message *message, const std::string &field, double value);
	bool AddBool  (google::protobuf::Message *message, const std::string &field, bool   value);
	bool AddString(google::protobuf::Message *message, const std::string &field, const std::string& value);

	virtual void AddError(const std::string& filename, int line, int column, const std::string& message);

private:
	google::protobuf::compiler::DiskSourceTree m_tree;
	google::protobuf::compiler::Importer m_importer;
	std::map<std::string, google::protobuf::MessageFactory*> m_protoFactory;
};
#endif
