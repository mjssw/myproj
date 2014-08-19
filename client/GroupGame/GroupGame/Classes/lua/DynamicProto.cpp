#include "DynamicProto.h"
using namespace google;
using namespace google::protobuf;
using namespace std;

CDynamicProto* CDynamicProto::m_pInstance = NULL;

void CDynamicProto::Init(const string &protoDir)
{
	m_tree.MapPath( "", protoDir );
}

Message* CDynamicProto::CreateMessage(const string &protofile, const std::string &name)
{
	const FileDescriptor *fd = m_importer.Import( protofile );
	if( !fd )
	{
		return NULL;
	}

    const Descriptor *desc = fd->pool()->FindMessageTypeByName( name );
	if( !desc )
	{
		return NULL;
	}

	MessageFactory *fact = NULL;
	map<string, MessageFactory*>::iterator it = m_protoFactory.find( protofile );
	if( it == m_protoFactory.end() )
	{
		fact = new DynamicMessageFactory( fd->pool() );
		m_protoFactory[ protofile ] = fact;
	}
	else
	{
		fact = it->second;
	}
	if( !fact )
	{
		return NULL;
	}

	const Message *temp = fact->GetPrototype( desc );
	if( !temp )
	{
		return NULL;
	}

	return temp->New();
}

#define _GET_COMPLEX_MESSAGE(parent, field, func) \
	if( !parent ) return NULL;\
	const Descriptor *desc = parent->GetDescriptor();\
	if( !desc ) return NULL;\
	const Reflection *ref = parent->GetReflection();\
	if( !ref ) return NULL;\
	const FieldDescriptor *fd = desc->FindFieldByName( field );\
	if( !fd ) return NULL;\
	return ref->func( parent, fd );

Message* CDynamicProto::MutableMessage(Message *parent, const string &field)
{
	_GET_COMPLEX_MESSAGE( parent, field, MutableMessage );
}

Message* CDynamicProto::AddMessage(Message *parent, const string &field)
{
	_GET_COMPLEX_MESSAGE( parent, field, AddMessage );
}

#undef _GET_COMPLEX_MESSAGE

void CDynamicProto::DeleteMessage(google::protobuf::Message *message)
{
	if( message )
	{
		delete message;
	}
}

#define _SET_MESSAGE_VALUE(message, field, value, setfunc) \
	if( !message ) return false;\
	const Descriptor *desc = message->GetDescriptor();\
	if( !desc ) return false;\
	const Reflection *ref = message->GetReflection();\
	if( !ref ) return false;\
    const FieldDescriptor *fd = desc->FindFieldByName( field );\
	if( !fd ) return false;\
	ref->setfunc( message, fd, value );\
	return true;

bool CDynamicProto::SetInt32 (Message *message, const std::string &field, int32  value)
{
	_SET_MESSAGE_VALUE( message, field, value, SetInt32 );
}

bool CDynamicProto::SetInt64 (Message *message, const std::string &field, int64  value)
{
	_SET_MESSAGE_VALUE( message, field, value, SetInt64 );
}

bool CDynamicProto::SetUInt32(Message *message, const std::string &field, uint32 value)
{
	_SET_MESSAGE_VALUE( message, field, value, SetUInt32 );
}

bool CDynamicProto::SetUInt64(Message *message, const std::string &field, uint64 value)
{
	_SET_MESSAGE_VALUE( message, field, value, SetUInt64 );
}

bool CDynamicProto::SetFloat (Message *message, const std::string &field, float  value)
{
	_SET_MESSAGE_VALUE( message, field, value, SetFloat );
}

bool CDynamicProto::SetDouble(Message *message, const std::string &field, double value)
{
	_SET_MESSAGE_VALUE( message, field, value, SetDouble );
}

bool CDynamicProto::SetBool  (Message *message, const std::string &field, bool   value)
{
	_SET_MESSAGE_VALUE( message, field, value, SetBool );
}

bool CDynamicProto::SetString(Message *message, const std::string &field, const std::string& value)
{
	_SET_MESSAGE_VALUE( message, field, value, SetString );
}

#undef _SET_MESSAGE_VALUE

#define _ADD_MESSAGE_VALUE(message, field, value, addfunc) \
	if( !message ) return false;\
	const Descriptor *desc = message->GetDescriptor();\
	if( !desc ) return false;\
	const Reflection *ref = message->GetReflection();\
	if( !ref ) return false;\
    const FieldDescriptor *fd = desc->FindFieldByName( field );\
	if( !fd ) return false;\
	ref->addfunc( message, fd, value );\
	return true;

bool CDynamicProto::AddInt32 (Message *message, const std::string &field, int32  value)
{
	_ADD_MESSAGE_VALUE( message, field, value, AddInt32 );
}

bool CDynamicProto::AddInt64 (Message *message, const std::string &field, int64  value)
{
	_ADD_MESSAGE_VALUE( message, field, value, AddInt64 );
}

bool CDynamicProto::AddUInt32(Message *message, const std::string &field, uint32 value)
{
	_ADD_MESSAGE_VALUE( message, field, value, AddUInt32 );
}

bool CDynamicProto::AddUInt64(Message *message, const std::string &field, uint64 value)
{
	_ADD_MESSAGE_VALUE( message, field, value, AddUInt64 );
}

bool CDynamicProto::AddFloat (Message *message, const std::string &field, float  value)
{
	_ADD_MESSAGE_VALUE( message, field, value, AddFloat );
}

bool CDynamicProto::AddDouble(Message *message, const std::string &field, double value)
{
	_ADD_MESSAGE_VALUE( message, field, value, AddDouble );
}

bool CDynamicProto::AddBool  (Message *message, const std::string &field, bool   value)
{
	_ADD_MESSAGE_VALUE( message, field, value, AddBool );
}

bool CDynamicProto::AddString(Message *message, const std::string &field, const std::string& value)
{
	_ADD_MESSAGE_VALUE( message, field, value, AddString );
}

#undef _ADD_MESSAGE_VALUE

bool CDynamicProto::ParseFromArray(google::protobuf::Message *msg, const void *data, int size)
{
	if( !msg || !data || size < 0 )
	{
		return false;
	}
	return msg->ParseFromArray( data, size );
}

#define _GET_MESSAGE_VALUE(message, field, getfunc, errret) \
	const Reflection *ref = message.GetReflection();\
	const Descriptor *desc = message.GetDescriptor();\
	const FieldDescriptor *fd = desc->FindFieldByName( field );\
	if( !fd ) return errret;\
	return ref->getfunc( message, fd );

int32  CDynamicProto::GetInt32 (const Message& message, const string &field)
{
	_GET_MESSAGE_VALUE( message, field, GetInt32, 0 );
}

int64  CDynamicProto::GetInt64 (const Message& message, const string &field)
{
	_GET_MESSAGE_VALUE( message, field, GetInt64, 0 );
}

uint32 CDynamicProto::GetUInt32(const Message& message, const string &field)
{
	_GET_MESSAGE_VALUE( message, field, GetUInt32, 0 );
}

uint64 CDynamicProto::GetUInt64(const Message& message, const string &field)
{
	_GET_MESSAGE_VALUE( message, field, GetUInt64, 0 );
}

float  CDynamicProto::GetFloat (const Message& message, const string &field)
{
	_GET_MESSAGE_VALUE( message, field, GetFloat, 0 );
}

double CDynamicProto::GetDouble(const Message& message, const string &field)
{
	_GET_MESSAGE_VALUE( message, field, GetDouble, 0 );
}

bool   CDynamicProto::GetBool  (const Message& message, const string &field)
{
	_GET_MESSAGE_VALUE( message, field, GetBool, false );
}

string CDynamicProto::GetString(const Message& message, const string &field)
{
	_GET_MESSAGE_VALUE( message, field, GetString, "" );
}

#undef _GET_MESSAGE_VALUE

const Message* CDynamicProto::GetMessage(const Message &parent, const string &field)
{
	const Reflection *ref = parent.GetReflection();
	const Descriptor *desc = parent.GetDescriptor();
	const FieldDescriptor *fd = desc->FindFieldByName( field );
	if( !fd )
	{
		return NULL;
	}
	return &(ref->GetMessage( parent, fd ));
}

	
#define _GET_REPEATED_MESSAGE_VALUE(message, field, idx, getfunc, errret) \
	const Reflection *ref = message.GetReflection();\
	const Descriptor *desc = message.GetDescriptor();\
	const FieldDescriptor *fd = desc->FindFieldByName( field );\
	if( !fd || !fd->is_repeated() ) return errret;\
	return ref->getfunc( message, fd, idx );

int32  CDynamicProto::GetRepeatedInt32 (const Message& message, const string &field, int index)
{
	_GET_REPEATED_MESSAGE_VALUE( message, field, index, GetRepeatedInt32, 0 );
}

int64  CDynamicProto::GetRepeatedInt64 (const Message& message, const string &field, int index)
{
	_GET_REPEATED_MESSAGE_VALUE( message, field, index, GetRepeatedInt64, 0 );
}

uint32 CDynamicProto::GetRepeatedUInt32(const Message& message, const string &field, int index)
{
	_GET_REPEATED_MESSAGE_VALUE( message, field, index, GetRepeatedUInt32, 0 );
}

uint64 CDynamicProto::GetRepeatedUInt64(const Message& message, const string &field, int index)
{
	_GET_REPEATED_MESSAGE_VALUE( message, field, index, GetRepeatedUInt64, 0 );
}

float  CDynamicProto::GetRepeatedFloat (const Message& message, const string &field, int index)
{
	_GET_REPEATED_MESSAGE_VALUE( message, field, index, GetRepeatedFloat, 0 );
}

double CDynamicProto::GetRepeatedDouble(const Message& message, const string &field, int index)
{
	_GET_REPEATED_MESSAGE_VALUE( message, field, index, GetRepeatedDouble, 0 );
}

bool   CDynamicProto::GetRepeatedBool  (const Message& message, const string &field, int index)
{
	_GET_REPEATED_MESSAGE_VALUE( message, field, index, GetRepeatedBool, false );
}

string CDynamicProto::GetRepeatedString(const Message& message, const string &field, int index)
{
	_GET_REPEATED_MESSAGE_VALUE( message, field, index, GetRepeatedString, "" );
}

#undef _GET_REPEATED_MESSAGE_VALUE

const Message* CDynamicProto::GetRepeatedMessage(const Message &parent, const string &field, int index)
{
	const Reflection *ref = parent.GetReflection();
	const Descriptor *desc = parent.GetDescriptor();
	const FieldDescriptor *fd = desc->FindFieldByName( field );
	if( !fd || !fd->is_repeated() )
	{
		return NULL;
	}
	return &(ref->GetRepeatedMessage( parent, fd, index ));
}

int CDynamicProto::GetFieldSize(const google::protobuf::Message& message, const std::string &field)
{
	const Reflection *ref = message.GetReflection();
	const Descriptor *desc = message.GetDescriptor();
	const FieldDescriptor *fd = desc->FindFieldByName( field );
	return ref->FieldSize( message, fd );
}

void CDynamicProto::AddError(const string& filename, int line, int column, const string& message)
{
}

