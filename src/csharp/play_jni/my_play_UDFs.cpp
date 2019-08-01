#include "stdafx.h"
#include "my_play_UDFs.h"
#include "JNIUtil.h"

using namespace MyTest;
using namespace Microsoft::SatoriStreamingV2;
using namespace System::Runtime::InteropServices;
using namespace System;

/*
 * Class:     my_play_UDFs
 * Method:    run
 * Signature: (Lmy/play/KafkaMessage;)Ljava/lang/String;
 */
JNIEXPORT jbyteArray JNICALL Java_my_play_UDFs_run
(JNIEnv *env, jclass dummy, jobject jKafkaMsg)
{

	//public String key;
	//public byte[] value;
	//public String topic;
	//public int partition;
	//public long offset;
	//public String timestamp;

	jclass jc = env->GetObjectClass(jKafkaMsg);
	jfieldID keyIndex = env->GetFieldID(jc, "key", "Ljava/lang/String;");
	jfieldID valueIndex = env->GetFieldID(jc, "value", "[B");
	jfieldID topicIndex = env->GetFieldID(jc, "topic", "Ljava/lang/String;");
	jfieldID partitionIndex = env->GetFieldID(jc, "partition", "I");
	jfieldID offsetIndex = env->GetFieldID(jc, "offset", "J");
	jfieldID timeIndex = env->GetFieldID(jc, "timestamp", "Ljava/lang/String;");


	KafkaMessage^ msg = gcnew KafkaMessage();

	// key
	jstring key = (jstring)(env->GetObjectField(jKafkaMsg, keyIndex));
	msg->Key = JNIUtil::ToCLRString(env, key);

	// value
	jbyteArray value = (jbyteArray)env->GetObjectField(jKafkaMsg, valueIndex);
	msg->Value = JNIUtil::ToCLRByteArray(env, value);

	// topic
	jstring topic = (jstring)(env->GetObjectField(jKafkaMsg, topicIndex));
	msg->Topic = JNIUtil::ToCLRString(env, topic);

	// parition
	jint parition = env->GetIntField(jKafkaMsg, partitionIndex);
	msg->Partition = parition;

	// offset
	jlong offset = env->GetLongField(jKafkaMsg, offsetIndex);
	msg->Offset = offset;

	// timestamp
	jstring time = (jstring)(env->GetObjectField(jKafkaMsg, timeIndex));
	msg->Timestamp = JNIUtil::ToCLRString(env, time);


	try {
		array<System::Byte>^ result = UDFs::Run(msg);
		return JNIUtil::ToJNIByteArray(env, result);
	}
	catch (System::Exception^ ex) {
		Console::Error->WriteLine("CPP Expcetion: " + ex);
		JNIUtil::ThrowJNIException(env, ex->ToString());
	}

	return env->NewByteArray(0);
}