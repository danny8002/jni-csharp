#pragma once
#include "jni.h"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;

Assembly^ ExecutingAssemblyDirectoryResolveEventHandler(Object^ sender, ResolveEventArgs^ args);

[System::Runtime::ExceptionServices::HandleProcessCorruptedStateExceptionsAttribute()]
void OnUnhandledException(System::Object ^sender, System::UnhandledExceptionEventArgs ^e);

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved);