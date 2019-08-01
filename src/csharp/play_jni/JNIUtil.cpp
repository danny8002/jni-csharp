
#include "stdafx.h"
#include "JNIUtil.h"
#include <msclr/marshal.h>

using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

// for string convertion
// see https://docs.microsoft.com/en-us/cpp/dotnet/overview-of-marshaling-in-cpp?view=vs-2019

System::String ^ Common::JNIUtil::ToCLRString(const char * value)
{
	marshal_context ctx;
	return ctx.marshal_as<System::String^>(value);
}

System::String ^ Common::JNIUtil::ToCLRString(JNIEnv * env, jstring value)
{
	const char *nativeString = env->GetStringUTFChars(value, 0);

	marshal_context ctx;
	return ctx.marshal_as<System::String^>(nativeString);
}

array<System::Byte>^ Common::JNIUtil::ToCLRByteArray(JNIEnv * env, jbyteArray value)
{
	size_t length = env->GetArrayLength(value);
	array<System::Byte>^ target = gcnew array<unsigned char>(length);

	jbyte* byte = env->GetByteArrayElements(value, NULL);
	Marshal::Copy((System::IntPtr)byte, target, 0, length);
	env->ReleaseByteArrayElements(value, byte, 0);

	return target;
}

jstring Common::JNIUtil::ToJNIString(JNIEnv * env, System::String ^ value)
{
	marshal_context ctx;
	const char* buf = ctx.marshal_as<const char*>(value);

	return env->NewStringUTF(buf);
}

jbyteArray Common::JNIUtil::ToJNIByteArray(JNIEnv * env, array<System::Byte>^ value)
{
	// https://docs.microsoft.com/en-us/cpp/dotnet/how-to-marshal-arrays-using-cpp-interop?view=vs-2019

	pin_ptr<System::Byte> p = &value[0];
	unsigned char* pby = p;
	const signed char* pch = reinterpret_cast<signed char*>(pby);

	jbyteArray array = env->NewByteArray(value->Length);
	env->SetByteArrayRegion(array, 0, value->Length, pch);

	return array;
}

jint Common::JNIUtil::ThrowJNIException(JNIEnv *env, System::String^ value)
{
	marshal_context ctx;
	const char* message = ctx.marshal_as<const char*>(value);

	const char *className = "java/lang/Error";

	jclass exClass = env->FindClass(className);
	// NOT NULL;

	return env->ThrowNew(exClass, message);
}

