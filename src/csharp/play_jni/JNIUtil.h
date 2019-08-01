
#pragma once
#include "jni.h"

namespace Microsoft
{
	namespace SatoriStreamingV2 {

		public ref class JNIUtil {

		public:

			static System::String^ ToCLRString(const char* value);
			static System::String^ ToCLRString(JNIEnv *env, jstring value);

			static array<System::Byte>^ ToCLRByteArray(JNIEnv *env, jbyteArray value);

			static jstring    ToJNIString(JNIEnv *env, System::String^ value);
			static jbyteArray ToJNIByteArray(JNIEnv *env, array<System::Byte>^ value);

			static jint ThrowJNIException(JNIEnv *env, System::String^ value);


		};
	}

}