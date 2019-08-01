#include "stdafx.h"
#include "windows.h"
#include "init.h"
#include <msclr\gcroot.h>
#include "JNIUtil.h"

using namespace System;
using namespace System::IO;
using namespace System::Net;

msclr::gcroot<String^> currentDir;

ref class Dummy {};

static String^ FindWorkingDir()
{
	Dummy^ dummy = gcnew Dummy();

	String^ codeBase = dummy->GetType()->Assembly->CodeBase;

	String^ uri = (gcnew Uri(codeBase))->AbsolutePath;

	return Path::GetDirectoryName(uri);
}

Assembly^ ExecutingAssemblyDirectoryResolveEventHandler(Object^ sender, ResolveEventArgs^ args)
{
	String^ resolvingAssemblyName = "";

	if (args->Name->Contains(","))
	{
		resolvingAssemblyName = currentDir + "\\" + args->Name->Substring(0, args->Name->IndexOf(",")) + ".dll";
	}
	else
	{
		resolvingAssemblyName = currentDir + "\\" + args->Name + ".dll";
	}

	//Load the assembly from the specified path.                               
	Assembly^  MyAssembly = Assembly::LoadFrom(resolvingAssemblyName);
	Console::WriteLine(resolvingAssemblyName);
	//Return the loaded assembly.
	return MyAssembly;
}

[System::Runtime::ExceptionServices::HandleProcessCorruptedStateExceptionsAttribute()]
void OnUnhandledException(System::Object ^sender, System::UnhandledExceptionEventArgs ^e)
{
	Exception^ ex = static_cast<Exception^>(e->ExceptionObject);
	if (ex == nullptr)
	{
		Console::WriteLine("Unknow exception occuried: " + e->ExceptionObject);
		System::Environment::Exit(1);
		return;
	}
	Console::WriteLine(" Unhandled execption occuried: " + ex->Message);
	Console::WriteLine(ex);
	System::Environment::Exit(1);
}

//HANDLE s_currentProcess;
int s_numberOfProcessors;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved) {

	JNIEnv *jenv;
	jvm->GetEnv((void **)&jenv, JNI_VERSION_1_6);

	currentDir = FindWorkingDir();
	Console::WriteLine("Get DLL location: " + currentDir);

	AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(ExecutingAssemblyDirectoryResolveEventHandler);
	if (AppDomain::CurrentDomain->IsDefaultAppDomain())
		AppDomain::CurrentDomain->UnhandledException += gcnew UnhandledExceptionEventHandler(OnUnhandledException);

	SYSTEM_INFO systemInfo;
	//s_currentProcess = GetCurrentProcess();
	GetSystemInfo(&systemInfo);
	s_numberOfProcessors = systemInfo.dwNumberOfProcessors;

	return JNI_VERSION_1_6;
}