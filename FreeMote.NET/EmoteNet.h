// EmoteNet.h
#include "clix.hpp"
#include <msclr\marshal.h>
#pragma once
using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;
using namespace System::Threading;
using namespace clix;

#define MAX_LENGTH 50
#define StringToCharPtr(x) marshalString<E_UTF8>(x).c_str()

//static const char* StringToCharPtr(String ^ str)
//{
//	if (str == nullptr || str->Length > 50)
//	{
//		return "";
//	}
//	marshal_context context;
//	
//	//context.marshal_as<const char*>(str);
//	char* result;
//	strcpy(result, context.marshal_as<const char*>(str));
//	return result;
//}

namespace FreeMote {
	//Make sure you used enum class to create a .NET-compatible enumeration, and not a native C++ enum type.
	//Later versions of Visual C++ require public enum class, to disambiguate from the enum class contextual keyword which is part of C++11 (and therefore creates a native type).


	// 変換
	static const float MSTOF60THS = 60.0f / 1000.0f;  // msを1/60秒カウントへ変換。
	static const float F60THSTOMS = 1000.0f / 60.0f;  // 1/60秒カウントをmsへ変換。

	//MARK: 值类型不要用^引用，否则C#中会表示成ValueType。IntPtr是值类型

	//Use pointer for delegate, see: http://stackoverflow.com/questions/8675638/c-cli-delegate-as-function-pointer-system-accessviolationexception
	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	public delegate void TextureFilterFunction(Byte[], unsigned long);

	[System::FlagsAttribute]
	public enum class TimelinePlayFlags :UInt32
	{
		NONE = 0,
		TIMELINE_PLAY_PARALLEL = IEmotePlayer::TIMELINE_PLAY_PARALLEL,
		TIMELINE_PLAY_DIFFERENCE = IEmotePlayer::TIMELINE_PLAY_DIFFERENCE
	};
	public enum class AlphaOp :UInt32
	{
		ALPHA_OP_DESTALPHA_MULTIPLY_INVSRCALPHA_PLUS_SRCALPHA = IEmoteDevice::ALPHA_OP_DESTALPHA_MULTIPLY_INVSRCALPHA_PLUS_SRCALPHA,
		ALPHA_OP_DESTALPHA_PLUS_SRCALPHA = IEmoteDevice::ALPHA_OP_DESTALPHA_PLUS_SRCALPHA
	};
	public enum class MaskMode :UInt32
	{
		MASK_MODE_STENCIL = IEmoteDevice::MASK_MODE_STENCIL,
		MASK_MODE_ALPHA = IEmoteDevice::MASK_MODE_ALPHA
	};
	public enum class ShaderModel :UInt32
	{
		SHADER_MODEL_NO_SHADER = IEmoteDevice::SHADER_MODEL_NO_SHADER,
		SHADER_MODEL_2 = IEmoteDevice::SHADER_MODEL_2,
		SHADER_MODEL_3 = IEmoteDevice::SHADER_MODEL_3
	};

	private ref class Lock {
		Object^ m_pObject;
	public:
		Lock(Object ^ pObject) : m_pObject(pObject) {
			Monitor::Enter(m_pObject);
		}
		~Lock() {
			Monitor::Exit(m_pObject);
		}
	};

	//How to Dispose & Finalize in C++/CLI : http://www.cnblogs.com/cxwx/archive/2010/08/07/1794762.html

	//EmotePlayer
	public ref class EmotePlayer : IDisposable
	{
	public:

		void AddRef()
		{
			sPlayer->AddRef();
		}

		void AddUserDefinedVariable(String^ var)
		{
			return sPlayer->AddUserDefinedVariable(StringToCharPtr(var));
		}

		void Release()
		{
			sPlayer->Release();
		}

		int RefCount()
		{
			return sPlayer->RefCount();
		}

		void AssignState(EmotePlayer^ another)
		{
			sPlayer->AssignState(another->NativePlayer);
		}

		void Show()
		{
			sPlayer->Show();
		}

		void Hide()
		{
			sPlayer->Hide();
		}

		bool IsHidden()
		{
			return sPlayer->IsHidden();
		}

		void SetSmoothing(bool state)
		{
			sPlayer->SetSmoothing(state);
		}
		bool GetSmoothing()
		{
			return sPlayer->GetSmoothing();
		}

		void SetMeshDivisionRatio(float ratio)
		{
			sPlayer->SetMeshDivisionRatio(ratio);
		}
		float GetMeshDivisionRatio()
		{
			return sPlayer->GetMeshDivisionRatio();
		}

		void SetQueuing(bool state)
		{
			sPlayer->SetQueuing(state);
		}
		bool GetQueuing()
		{
			return sPlayer->GetQueuing();
		}

		void SetHairScale(float scale)
		{
			sPlayer->SetHairScale(scale);
		}
		float GetHairScale()
		{
			return sPlayer->GetHairScale();
		}

		void SetPartsScale(float scale)
		{
			sPlayer->SetPartsScale(scale);
		}
		float GetPartsScale()
		{
			return sPlayer->GetPartsScale();
		}

		void SetBustScale(float scale)
		{
			sPlayer->SetBustScale(scale);
		}
		float GetBustScale()
		{
			return sPlayer->GetBustScale();
		}

		void SetCoord(float x, float y, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetCoord(x, y, frameCount, easing);
		}
		void GetCoord([OutAttribute]float% x, [OutAttribute]float% y)
		{
			float tx = 0, ty = 0;
			sPlayer->GetCoord(tx, ty);
			x = tx;
			y = ty;
		}

		void SetScale(float scale, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetScale(scale, frameCount, easing);
		}
		float GetScale()
		{
			return sPlayer->GetScale();
		}

		void SetRot(float rot, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetRot(rot, frameCount, easing);
		}
		float GetRot()
		{
			return sPlayer->GetRot();
		}

		void SetColor(UInt32 rgba, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetColor(rgba, frameCount, easing);
		}
		UInt32 GetColor()
		{
			return sPlayer->GetColor();
		}

		void SetGrayscale(float rate, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetGrayscale(rate, frameCount, easing);
		}
		float GetGrayscale()
		{
			return sPlayer->GetGrayscale();
		}

		UInt32 CountVariables()
		{
			return sPlayer->CountVariables();
		}

		String^ GetVariableLabelAt(UInt32 index)
		{
			return marshalString<E_UTF8>(sPlayer->GetVariableLabelAt(index));
		}

		UInt32 CountVariableFrameAt(UInt32 variableIndex)
		{
			return (UInt32)(sPlayer->CountVariableFrameAt(variableIndex));
		}

		String^ GetVariableFrameLabelAt(UInt32 variableIndex, UInt32 frameIndex)
		{
			return marshalString<E_UTF8>(sPlayer->GetVariableFrameLabelAt(variableIndex, frameIndex));
		}

		float GetVariableFrameValueAt(UInt32 variableIndex, UInt32 frameIndex)
		{
			return sPlayer->GetVariableFrameValueAt(variableIndex, frameIndex);
		}

		float GetVariable(String^ var)
		{
			return sPlayer->GetVariable(StringToCharPtr(var));
		}
		void SetVariable(String^ var, float value, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetVariable(StringToCharPtr(var), value, frameCount, easing);
		}

		void SetVariableDiff(String^ module, String^ label, float value, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetVariableDiff(StringToCharPtr(module), StringToCharPtr(label), value, frameCount, easing);
		}
		float GetVariableDiff(String^ module, String^ label)
		{
			return sPlayer->GetVariableDiff(StringToCharPtr(module), StringToCharPtr(label));
		}

		void SetOuterForce(String^ label, float ofx, float ofy, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetOuterForce(StringToCharPtr(label), ofx, ofy, frameCount, easing);
		}
		void GetOuterForce(String^ label, [OutAttribute]float% ofx, [OutAttribute]float% ofy)
		{
			float _ofx, _ofy;
			sPlayer->GetOuterForce(StringToCharPtr(label), _ofx, _ofy);
			ofx = _ofx;
			ofy = _ofy;
		}

		void StartWind(float start, float goal, float speed, float powMin, float powMax)
		{
			sPlayer->StartWind(start, goal, speed, powMin, powMax);
		}
		void StopWind()
		{
			sPlayer->StopWind();
		}

		UInt32 CountMainTimelines()
		{
			return sPlayer->CountMainTimelines();
		}
		String ^ GetMainTimelineLabelAt(UInt32 index)
		{
			return marshalString<E_UTF8>(sPlayer->GetMainTimelineLabelAt(index));
		}

		UInt32 CountDiffTimelines()
		{
			return sPlayer->CountDiffTimelines();
		}
		String^ GetDiffTimelineLabelAt(UInt32 index)
		{
			return marshalString<E_UTF8>(sPlayer->GetDiffTimelineLabelAt(index));
		}

		UInt32 CountPlayingTimelines()
		{
			return sPlayer->CountPlayingTimelines();
		}
		String^ GetPlayingTimelineLabelAt(UInt32 index)
		{
			return marshalString<E_UTF8>(sPlayer->GetPlayingTimelineLabelAt(index));
		}
		UInt32 GetPlayingTimelineFlagsAt(UInt32 index)
		{
			return sPlayer->GetPlayingTimelineFlagsAt(index);
		}

		bool IsLoopTimeline(String^ label)
		{
			return sPlayer->IsLoopTimeline(StringToCharPtr(label));
		}

		float GetTimelineTotalFrameCount(String^ label)
		{
			return sPlayer->GetTimelineTotalFrameCount(StringToCharPtr(label));
		}

		void PlayTimeline(String^ label, TimelinePlayFlags flag)
		{
			sPlayer->PlayTimeline(StringToCharPtr(label), (emote_uint32_t)flag);
		}
		bool IsTimelinePlaying(String^ label)
		{
			return sPlayer->IsTimelinePlaying(StringToCharPtr(label));
		}
		void StopTimeline(String^ label)
		{
			sPlayer->StopTimeline(StringToCharPtr(label));
		}

		void SetTimelineBlendRatio(String^ label, float value, [Optional]float frameCount, [Optional]float easing, bool stopWhenBlendDone)
		{
			sPlayer->SetTimelineBlendRatio(StringToCharPtr(label), value, frameCount, easing, stopWhenBlendDone);
		}
		float GetTimelineBlendRatio(String^ label)
		{
			return sPlayer->GetTimelineBlendRatio(StringToCharPtr(label));
		}

		void FadeInTimeline(String^ label, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->FadeInTimeline(StringToCharPtr(label), frameCount, easing);
		}
		void FadeOutTimeline(String^ label, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->FadeOutTimeline(StringToCharPtr(label), frameCount, easing);
		}

		bool IsAnimating()
		{
			return sPlayer->IsAnimating();
		}

		void Skip()
		{
			sPlayer->Skip();
		}

		void Pass()
		{
			sPlayer->Pass();
		}

		void Progress(float frameCount)
		{
			sPlayer->Progress(frameCount);
		}

		bool IsModified()
		{
			return sPlayer->IsModified();
		}

		void Render()
		{
			sPlayer->Render();
		}
		
#pragma region Stereovision

		void SetStereovisionEnabled(bool state)
		{
			sPlayer->SetStereovisionEnabled(state);
		}
		bool GetStereovisionEnabled()
		{
			return sPlayer->GetStereovisionEnabled();
		}

		void SetStereovisionVolume(float volume)
		{
			sPlayer->SetStereovisionVolume(volume);
		}
		float GetStereovisionVolume()
		{
			return sPlayer->GetStereovisionVolume();
		}

		void SetStereovisionParallaxRatio(float ratio)
		{
			sPlayer->SetStereovisionParallaxRatio(ratio);
		}
		float GetStereovisionParallaxRatio()
		{
			return sPlayer->GetStereovisionParallaxRatio();
		}

		void SetStereovisionRenderScreen(UInt32 index)
		{
			sPlayer->SetStereovisionRenderScreen(index);
		}
		UInt32 GetStereovisionRenderScreen()
		{
			return sPlayer->GetStereovisionRenderScreen();
		}

#pragma endregion

#pragma region CharaProfile

		bool IsCharaProfileAvailable()
		{
			return sPlayer->IsCharaProfileAvailable();
		}
		
		float GetCharaHeight()
		{
			return sPlayer->GetCharaHeight();
		}

		UInt32 CountCharaProfiles()
		{
			return sPlayer->CountCharaProfiles();
		}

		String^ GetCharaProfileLabelAt(UInt32 profileIndex)
		{
			return marshalString<E_UTF8>(sPlayer->GetCharaProfileLabelAt(profileIndex));
		}

		float GetCharaProfile(String^ label)
		{
			return sPlayer->GetCharaProfile(StringToCharPtr(label));
		}

#pragma endregion

#pragma region Log
		void StartRecordAPILog()
		{
			sPlayer->StartRecordAPILog();
		}

		void StopRecordAPILog()
		{
			sPlayer->StopRecordAPILog();
		}

		bool IsRecordingAPILog()
		{
			return sPlayer->IsRecordingAPILog();
		}
		
		void StartReplayAPILog()
		{
			sPlayer->StartReplayAPILog();
		}

		void StopReplayAPILog()
		{
			sPlayer->StopReplayAPILog();
		}

		bool IsReplayingAPILog()
		{
			return sPlayer->IsReplayingAPILog();
		}

		void ClearAPILog()
		{
			sPlayer->ClearAPILog();
		}

		String^ GetAPILog()
		{
			return marshalString<E_UTF8>(sPlayer->GetAPILog());
		}

		void SetAPILog(String^ log)
		{
			sPlayer->SetAPILog(StringToCharPtr(log));
		}
#pragma endregion


		UInt32 GetTransformOrderMask()
		{
			return sPlayer->GetTransformOrderMask();
		}

		void SetTransformOrderMask(UInt32 mask)
		{
			sPlayer->SetTransformOrderMask(mask);
		}

		UInt32 GetSuitableClearColor()
		{
			return sPlayer->GetSuitableClearColor();
		}

		void SetOuterRot(float rot, [Optional]float frameCount, [Optional]float easing)
		{
			sPlayer->SetOuterRot(rot, frameCount, easing);
		}
		float GetOuterRot()
		{
			return sPlayer->GetOuterRot();
		}

		//ADDED:
		/// <summary>
		/// 设置偏移
		/// </summary>
		/// <param name="ofsx">横向偏移</param>
		/// <param name="ofsy">竖向偏移</param>
		void OffsetCoord(int ofsx, int ofsy)
		{
			float x, y;
			sPlayer->GetCoord(x, y);
			sPlayer->SetCoord(x + ofsx, y + ofsy);
		}

		/// <summary>
		/// 设置缩放
		/// </summary>
		/// <param name="ofstScale">缩放增量百分比</param>
		void OffsetScale(float ofstScale)
		{
			float scale;
			scale = sPlayer->GetScale();
			sPlayer->SetScale(scale * ofstScale);
		}

		/// <summary>
		/// 设置旋转
		/// </summary>
		/// <param name="ofstRot">旋转增量百分比</param>
		void OffsetRot(float ofstRot)
		{
			float rot;
			rot = sPlayer->GetRot();
			sPlayer->SetRot(rot + ofstRot);
		}

		///// <summary>
		///// E-mote切换口型
		///// </summary>
		//void SwitchMouth(void)
		//{
		//	UpdateMouthTimeline((sMouthIndex + 1) % 2);
		//}

		void SetVariables(IDictionary<String^, float>^ table, float time, float easing)
		{
			for each (auto t in table)
			{
				auto lbl = StringToCharPtr(t.Key);
				// E-moteは1/60秒を1単位で駆動するので時間単位を変換。
				sPlayer->SetVariable(lbl, t.Value, time * MSTOF60THS, easing);
			}
			//OutputDebugString(L"emote update variables.\n");
		}

		//ref: void foo(String^% refstr); 
		//out: [System::Runtime::InteropServices::OutAttribute]
	internal:
		~EmotePlayer()
		{
			if (sPlayer != NULL)
			{
				sPlayer->Release();
				//delete sPlayer;
			}
		}

		EmotePlayer(IEmotePlayer *player)
		{
			sPlayer = player;
		}
		void AssignState(IEmotePlayer *another)
		{
			sPlayer->AssignState(another);
		}
		IEmotePlayer * Clone()
		{
			return sPlayer->Clone();
		}
		property IEmotePlayer* NativePlayer
		{
			IEmotePlayer* get() { return sPlayer; }
		}

		//void UpdateMouthTimeline(int newIndex)
		//{
		//	sPlayer->StopTimeline(sMouthTimelineList[sMouthIndex]);
		//	sMouthIndex = newIndex;
		//	sPlayer->PlayTimeline(sMouthTimelineList[sMouthIndex], IEmotePlayer::TIMELINE_PLAY_PARALLEL);
		//	//OutputDebugString(L"emote update pose timeline.\n");
		//}

		//void UpdatePoseTimeline(int newIndex)
		//{
		//	sPlayer->StopTimeline(sPoseTimelineList[sPoseIndex]);
		//	sPoseIndex = newIndex;
		//	sPlayer->PlayTimeline(sPoseTimelineList[sPoseIndex], IEmotePlayer::TIMELINE_PLAY_PARALLEL);
		//	//OutputDebugString(L"emote update pose timeline.\n");
		//}

	private:
		IEmotePlayer *sPlayer;
		//marshal_context context;
		//const char* StringToCharPtr(String ^ str)
		//{
		//	if (String::IsNullOrWhiteSpace(str) || str->Length > MAX_LENGTH)
		//	{
		//		return "";
		//	}
		//	//marshal_context context;
		//	//context.marshal_as<const char*>(str);
		//	//std::string s = marshalString<E_ANSI>(str); //MARK: can not be used here because managed string is released and so do the const char*
		//	//return s.c_str();
		//	return context.marshal_as<const char*>(str);
		//}

		int sPoseIndex, sMouthIndex;
	};


	public ref class EmoteDevice : IDisposable
	{
	public:
		/// <summary>
		/// 增加引用计数
		/// <para>手动增加的要手动负责<see cref="Release"/></para>
		/// </summary>
		UInt32 AddRef()
		{
			return device->AddRef();
		}

		/// <summary>
		/// 减少引用计数，当引用计数为0时自动释放自身
		/// </summary>			
		void Release()
		{
			device->Release();
		}

		void SetAlphaOp(AlphaOp op)
		{
			device->SetAlphaOp((IEmoteDevice::alpha_op_t)op);
		}

		AlphaOp GetAlphaOp()
		{
			return (AlphaOp)device->GetAlphaOp();
		}

		void SetMaskMode(MaskMode mask)
		{
			device->SetMaskMode((IEmoteDevice::mask_mode_t)mask);
		}

		MaskMode GetMaskMode()
		{
			return (MaskMode)device->GetMaskMode();
		}

		void SetMipMapEnabled(bool state)
		{
			device->SetMipMapEnabled(state);
		}

		bool GetMipMapEnabled()
		{
			return device->GetMipMapEnabled();
		}

		void SetMaskRegionClipping(bool state)
		{
			device->SetMaskRegionClipping(state);
		}

		bool GetMaskRegionClipping()
		{
			return device->GetMaskRegionClipping();
		}

		void SetPixelateDivision(int division)
		{
			device->SetPixelateDivision(division);
		}

		int GetPixelateDivision()
		{
			return device->GetPixelateDivision();
		}

		void SetProtectTranslucentTextureColor(bool state)
		{
			device->SetProtectTranslucentTextureColor(state);
		}

		bool GetProtectTranslucentTextureColor()
		{
			return device->GetProtectTranslucentTextureColor();
		}

		void SetShaderModel(ShaderModel shaderModel)
		{
			device->SetShaderModel((IEmoteDevice::shader_model_t)shaderModel);
		}

		ShaderModel GetShaderModel()
		{
			return (ShaderModel)device->GetShaderModel();
		}

		void SetMaxTextureSize(UInt32 width, UInt32 height)
		{
			device->SetMaxTextureSize(width, height);
		}

		/// <summary>
		/// 恢复状态（从Device Lost状态复原）
		/// <para>当IDirect3DDevice9::Present()检测到设备丢失时，在IDirect3DDevice9::Reset()调用之前请调用此函数。</para>
		/// <para>在内部进行资源的保存和设备恢复的准备。</para>
		/// </summary>			
		void OnDeviceLost()
		{
			device->OnDeviceLost();
		};

		void OnRenderTarget(IntPtr renderTargetTexture)
		{
			device->OnRenderTarget((LPDIRECT3DTEXTURE9)(renderTargetTexture.ToPointer()));
		};

		void SetUseD3D9Ex(bool state)
		{
			device->SetUseD3D9Ex(state);
		}

		bool GetUseD3D9Ex()
		{
			return device->GetUseD3D9Ex();
		}

		static String^ GetBuildDateTime()
		{
			auto str = EmoteGetBuildDateTime();
			return marshalString<E_UTF8>(str);			
		}

		static String^ GetSDKVersion()
		{
			auto str = EmoteGetSDKVersion();
			return marshalString<E_UTF8>(str);
		}

		static bool CheckValidObject(array<Byte>^ image)
		{
			pin_ptr<emote_uint8_t> pinnedBytes = &image[0];
			return EmoteCheckValidObject(pinnedBytes, image->Length);
		}
		
		//MARK: How to use array types in XML doc, SEE: http://stackoverflow.com/questions/2367493/how-to-refer-to-array-types-in-documentation-comments
		/// <summary>
		/// 纹理预处理函数
		/// <para><see cref="T:byte[]"/> image字节数组（默认颜色格式为BGRA）; <see cref="uint"/> 数组长度</para>
		/// </summary>			
		property TextureFilterFunction^ TextureFilter;
		property bool UseTextureFilter;

	internal:
		~EmoteDevice()
		{
			if (device != NULL)
			{
				device->Release();
				//delete device;
			}
		}

		EmoteDevice(IEmoteDevice* sDevice)
		{
			device = sDevice;
			UseTextureFilter = false;
		};

		void OnRenderTarget(LPDIRECT3DTEXTURE9 renderTargetTexture)
		{
			device->OnRenderTarget(renderTargetTexture);
		};

		property IEmoteDevice* NativeDevice
		{
			IEmoteDevice* get() { return device; }
		};

		EmotePlayer ^ CreatePlayer(String ^ path);

		EmotePlayer ^ EmoteDevice::CreatePlayer(array<String^>^ paths);
		//void FilterProcess(BYTE * image, ULONG imageSize);

	private:
		IEmoteDevice* device = nullptr;

	};



	/// <summary>
	/// E-mote
	/// </summary>
	public ref class Emote : IDisposable
	{
	public:
		/// <summary>
		/// E-mote释放资源
		/// </summary>
		~Emote() //Dispose!
		{
			for each (auto player in PlayerCollection->Values)
			{
				try
				{
					if (player != nullptr)
					{
						//delete player->NativePlayer;
						delete player;
						player = nullptr;
					}
				}
				catch (const std::exception&)
				{
				}
				catch (System::Exception^)
				{
				}
			}
			if (device != nullptr)
			{
				delete device;
				device = nullptr;
			}
		}

		/// <summary>
		/// EmoteDevice
		/// </summary>
		property const EmoteDevice^ Device
		{
			const EmoteDevice^ get()
			{
				return device;
			}
		}

		//MARK: Indexer in C++/CLI. SEE: <Foundations of C++/CLI> p193
		property const EmotePlayer^ default[String^]
		{
			const EmotePlayer^ get(String^ name)
			{
				if (PlayerCollection->ContainsKey(name))
				{
					return PlayerCollection[name];
				}
				throw gcnew KeyNotFoundException("Can not find any player named " + name);
			}
		}

			//const EmotePlayer^ operator [] (String^ name)
			//{
			//	if (PlayerCollection->ContainsKey(name))
			//	{
			//		return PlayerCollection[name];
			//	}
			//	throw gcnew KeyNotFoundException("Can not find any player named " + name);
			//};

			/// <summary>
			/// E-mote
			///	<para>使用此版本的构造函数不需要调用<see cref="D3DInit"/>和<see cref="EmoteInit"/>。请手动创建DX9设备</para>
			/// </summary>
			/// <param name="dxHandle">DX9 Device指针</param>
			Emote(IntPtr dxHandle)
		{
			EmoteInit(dxHandle);
		};

		Emote(IntPtr handle, bool useD3DSurface)
		{
			D3DInit(handle, -1, -1, false);
		};

		Emote(IntPtr handle, int width, int height)
		{
			D3DInit(handle, width, height, false);
		};

		Emote(IntPtr handle, int width, int height, bool useD3DSurface)
		{
			D3DInit(handle, width, height, useD3DSurface);
		};

		/// <summary>
		/// D3D Initialize
		/// </summary>
		/// <param name="handle">窗口句柄</param>
		/// <param name="width">渲染区宽度</param>
		/// <param name="height">渲染区高度</param>
		/// <param name="useD3DSurface">是否使用Surface</param>
		/// <returns></returns>
		void D3DInit(IntPtr handle, int width, int height, bool useD3DSurface);

		/// <summary>
		/// Device.Reset
		/// </summary>
		void D3DReset()
		{
			D3DPRESENT_PARAMETERS sD3Dpp;
			memset(&sD3Dpp, 0, sizeof(sD3Dpp));
			sD3Dpp.BackBufferWidth = sScreenWidth;
			sD3Dpp.BackBufferHeight = sScreenHeight;
			sD3Dpp.BackBufferFormat = D3DFMT_UNKNOWN;
			sD3Dpp.Windowed = true;
			sD3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			sD3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

			sD3DDevice->Reset(&sD3Dpp);
		};

		/// <summary>
		/// E-mote初始化
		/// </summary>
		EmoteDevice^ EmoteInit();

		/// <summary>
		/// 当Device Lost时，在Reset前执行此方法。会在内部执行<see cref="EmoteDevice.OnDeviceLost"/>方法，无需再次执行。
		/// </summary>
		void OnDeviceLost();

		/// <summary>
		/// 在Device Reset之后可以执行此方法，也可以不使用。会在内部执行<see cref="D3DInitRenderState"/>方法，无需再次执行。
		/// </summary>
		void OnDeviceReset();

		/// <summary>
		/// D3D初始化渲染状态
		/// </summary>
		void D3DInitRenderState();

		/// <summary>
		/// D3D初始化渲染状态
		/// </summary>
		/// <param name="screenWidth">宽</param>
		/// <param name="screenHeight">高</param>
		/// <param name="handle">Direct3D9 Device</param>
		void D3DInitRenderState(int screenWidth, int screenHeight, IntPtr handle);

		/// <summary>
		/// Device.TestCooperativeLevel
		/// </summary>
		/// <return>HRESULT</return>
		unsigned long D3DTestCooperativeLevel();

		/// <summary>
		/// E-mote更新（作用于全部Player）
		/// </summary>
		/// <param name="frameCount">经过的帧数（frame count）,1/60秒为单位</param>
		void Update(float frameCount);

		/// <summary>
		/// E-mote绘制（作用于全部Player）
		/// </summary>
		void Draw();

		void DrawCanvasTexture(void);
		void AttachCanvasTexture(void);
		void DetachCanvasTexture(void);

		/// <summary>
		/// E-mote跳过（作用于全部Player）
		/// </summary>
		void Skip(void);
		/// <summary>
		/// E-mote开始显示（作用于全部Player）
		/// </summary>
		void Show(void);

		/// <summary>
		/// 创建E-mote Player
		/// </summary>
		/// <param name="name">名字(不可重复)</param>
		/// <param name="path">立绘文件路径</param>
		EmotePlayer^ CreatePlayer(String^ name, String^ path);

		/// <summary>
		/// [Obsolete]创建E-mote Player
		/// </summary>
		/// <param name="name">名字(不可重复)</param>
		/// <param name="paths">立绘文件路径</param>
		EmotePlayer^ Emote::CreatePlayer(String^ name, array<String^>^ paths);

		/// <summary>
		/// 删除E-mote Player
		/// </summary>
		/// <param name="player">EmotePlayer名</param>
		void DeletePlayer(String^ player)
		{
			Lock lock(PlayerCollection);
			if (PlayerCollection->ContainsKey(player))
			{
				EmotePlayer^ p = PlayerCollection[player];
				PlayerCollection->Remove(player);
				if (p != nullptr)
				{
					delete p;
				}
			}
		}

		/// <summary>
		/// 删除E-mote Player（删除后会立即释放其资源）
		/// </summary>
		/// <param name="player">EmotePlayer对象</param>
		void DeletePlayer(EmotePlayer^ player)
		{
			if (player == nullptr)
			{
				return;
			}
			Lock lock(PlayerCollection);
			if (PlayerCollection->ContainsValue(player))
			{
				List<String^>^ toBeDeleted = gcnew List<String^>();
				for each (auto pair in PlayerCollection)
				{
					if (pair.Value == player)
					{
						toBeDeleted->Add(pair.Key);
					}
				}
				for each (auto str in toBeDeleted)
				{
					PlayerCollection->Remove(str);
				}
			}
			delete player;
		};

		/// <summary>
		/// D3D释放资源
		/// </summary>
		void D3DRelease(void);

		//void EmoteRelease(void);

		/// <summary>
		/// D3D开始渲染
		/// </summary>
		void D3DBeginScene(void);
		/// <summary>
		/// D3D结束渲染
		/// </summary>
		void D3DEndScene(void);

		/// <summary>
		/// D3D设备指针
		/// </summary>
		property IntPtr D3Device;

		/// <summary>
		/// D3D Surface指针
		/// </summary>
		property IntPtr D3DSurface
		{
			IntPtr get()
			{
				if (sSurface > 0)
				{
					return IntPtr(sSurface);
				}
				if (sD3DDevice != nullptr)
				{
					LPDIRECT3DSURFACE9 surf;
					sD3DDevice->GetRenderTarget(0, &surf);
					return IntPtr(surf);
				}
				return IntPtr::Zero;
			}
		}

		property IntPtr D3DCanvasTexture
		{
			IntPtr get()
			{
				return IntPtr(sCanvasTexture);
			}
			void set(IntPtr ptr)
			{
				sCanvasTexture = (LPDIRECT3DTEXTURE9)ptr.ToPointer();
			}
		}

		/// <summary>
		/// 是否使用Surface
		/// </summary>
		property bool UseD3DSurface
		{
			bool get() { return useD3DSurface; }
		}

		/// <summary>
		/// Surface宽
		/// </summary>
		property int SurfaceWidth
		{
			int get() { return sScreenWidth; }
		}

		/// <summary>
		/// Surface高
		/// </summary>
		property int SurfaceHeight
		{
			int get() { return sScreenHeight; }
		}
		//C# const is literal in C++/CLI, and
		//C# readonly is initonly in C++ / CLI

	internal:
		/// <summary>
		/// E-mote初始化
		/// </summary>
		EmoteDevice^ EmoteInit(IntPtr dxHandle);

		FreeMote::Emote()
		{};
	private:
		EmoteDevice^ device;
		IEmoteDevice *sEmoteDevice;
		//IEmotePlayer *sEmotePlayer;
		LPDIRECT3DSURFACE9 sSurface;
		LPDIRECT3DDEVICE9 sD3DDevice;
		LPDIRECT3DDEVICE9EX sD3DDeviceEx = NULL;

		Dictionary<String^, FreeMote::EmotePlayer^>^ PlayerCollection;

		bool useD3DSurface = false;
		bool useD3DEx = false;
		bool transparent = false;

		int sScreenWidth;
		int sScreenHeight;

		LPDIRECT3DTEXTURE9 sCanvasTexture;
		int sCanvasTextureWidth, sCanvasTextureHeight;

		void RequireCanvasTexture(void);
	};
}
