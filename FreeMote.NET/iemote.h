//**********************************
// THIS FILE IS AUTO GENERATED FILE.
// DON'T EDIT THIS FILE.
// EDIT TEMPLATE FILE INSTEAD.
//**********************************
  
#ifndef __EMOTE_DRIVER_H
#define __EMOTE_DRIVER_H

#include <cstddef>
#include <d3d9.h>

typedef BYTE emote_uint8_t;
typedef ULONG emote_uint32_t;
typedef LONG emote_int32_t;

#ifdef EMOTE_STATIC_LIBRARY
#define _EXPORT
#else
#ifdef EMOTE_EXPORTS
#define _EXPORT __declspec(dllexport)
#else
#define _EXPORT __declspec(dllimport)
#endif // EMOTE_EXPORTS
#endif // EMOTE_STATIC_LIBRARY



typedef const emote_uint8_t * emote_image_ptr_t;

class _EXPORT IEmoteDevice
{
public:
  virtual ~IEmoteDevice(void) {};

  virtual emote_uint32_t AddRef(void) = 0;
  virtual emote_uint32_t Release(void) = 0;
  virtual emote_uint32_t RefCount(void) const = 0;

  enum mask_mode_t {
    MASK_MODE_STENCIL,
    MASK_MODE_ALPHA
  };


  typedef void* (*ObjMemAlloc)(std::size_t size);
  typedef void (*ObjMemFree)(void *ptr);
  struct ObjMemAllocator
  {
    ObjMemAlloc alloc;
    ObjMemFree free;
  };


  enum shader_model_t {
    SHADER_MODEL_NO_SHADER,
    SHADER_MODEL_2,
    SHADER_MODEL_3
  };

  enum alpha_op_t {
    ALPHA_OP_DESTALPHA_MULTIPLY_INVSRCALPHA_PLUS_SRCALPHA,
    ALPHA_OP_DESTALPHA_PLUS_SRCALPHA
  };

  struct InitParam {
    LPDIRECT3DDEVICE9 d3dDevice;
    ObjMemAllocator objAllocator;
  };

  virtual const IEmoteDevice::InitParam &GetInitParam(void) const = 0;

  virtual void SetMaskMode(mask_mode_t maskMode) = 0;
  virtual mask_mode_t GetMaskMode(void) const = 0;

  virtual void SetMaskRegionClipping(bool state) = 0;
  virtual bool GetMaskRegionClipping(void) const = 0;


  virtual void CreatePlayer(emote_image_ptr_t emoteObjectImage, emote_uint32_t emoteObjectSize, class IEmotePlayer **player) = 0;
  virtual void CreatePlayer(emote_uint32_t emoteObjectNum, const emote_image_ptr_t *emoteObjectImage, const emote_uint32_t *emoteObjectSize, class IEmotePlayer **player) = 0;

  virtual void SetProtectTranslucentTextureColor(bool state) = 0;
  virtual bool GetProtectTranslucentTextureColor(void) const = 0;

  virtual shader_model_t GetAvailableShaderModel(void) const = 0;
  virtual void SetShaderModel(shader_model_t model) = 0;
  virtual shader_model_t GetShaderModel(void) const = 0;

  virtual void SetMipMapEnabled(bool state) = 0;
  virtual bool GetMipMapEnabled(void) const = 0;

  virtual void SetAlphaOp(alpha_op_t alphaOp) = 0;
  virtual alpha_op_t GetAlphaOp(void) const = 0;

  virtual void SetPixelateDivision(int division) = 0;
  virtual int GetPixelateDivision(void) const = 0;

  virtual void SetMaxTextureSize(emote_uint32_t width, emote_uint32_t height) = 0;

  virtual void OnDeviceLost(void) = 0;
  virtual void OnRenderTarget(LPDIRECT3DTEXTURE9 renderTargetTexture) = 0;


  virtual void SetUseD3D9Ex(bool state) = 0;
  virtual bool GetUseD3D9Ex(void) const = 0;
};


class _EXPORT IEmotePlayer
{
public:
  enum timeline_play_flags_t {
    TIMELINE_PLAY_PARALLEL   = 1 << 0,
    TIMELINE_PLAY_DIFFERENCE = 1 << 1
  };

  enum transform_order_mask_t {
    POSITION_TRANSFORM_ORDER_MASK_TRANSLATE_TO_SCALE = 1 << 0,
    POSITION_TRANSFORM_ORDER_MASK_SCALE_TO_TRANSLATE = 1 << 1,
    PHYSICS_TRANSFORM_ORDER_MASK_TRANSLATE_TO_SCALE = 1 << 8,
    PHYSICS_TRANSFORM_ORDER_MASK_SCALE_TO_TRANSLATE = 1 << 9,
    
    TRANSFORM_ORDER_MASK_TRANSLATE_TO_SCALE = POSITION_TRANSFORM_ORDER_MASK_TRANSLATE_TO_SCALE | PHYSICS_TRANSFORM_ORDER_MASK_TRANSLATE_TO_SCALE,
    TRANSFORM_ORDER_MASK_SCALE_TO_TRANSLATE = POSITION_TRANSFORM_ORDER_MASK_SCALE_TO_TRANSLATE | PHYSICS_TRANSFORM_ORDER_MASK_SCALE_TO_TRANSLATE,
    TRANSFORM_ORDER_MASK_DEFAULT = POSITION_TRANSFORM_ORDER_MASK_TRANSLATE_TO_SCALE | PHYSICS_TRANSFORM_ORDER_MASK_SCALE_TO_TRANSLATE
  };

  virtual ~IEmotePlayer(void) {}

  virtual emote_uint32_t AddRef(void) = 0;
  virtual emote_uint32_t Release(void) = 0;
  virtual emote_uint32_t RefCount(void) const = 0;

  virtual IEmotePlayer *Clone(void) = 0;
  virtual void AssignState(IEmotePlayer *another) = 0;

  virtual void Show(void) = 0;
  virtual void Hide(void) = 0;
  virtual bool IsHidden(void) const = 0;

  virtual void SetSmoothing(bool state) = 0;
  virtual bool GetSmoothing(void) const = 0;

  virtual void SetMeshDivisionRatio(float ratio) = 0;
  virtual float GetMeshDivisionRatio(void) const = 0;

  virtual void SetQueuing(bool state) = 0;
  virtual bool GetQueuing(void) const = 0;

  virtual void SetTransformOrderMask(emote_uint32_t mask) = 0;
  virtual emote_uint32_t GetTransformOrderMask(void) const = 0;

  virtual void SetHairScale(float scale) = 0;
  virtual float GetHairScale(void) const = 0;

  virtual void SetPartsScale(float scale) = 0;
  virtual float GetPartsScale(void) const = 0;

  virtual void SetBustScale(float scale) = 0;
  virtual float GetBustScale(void) const = 0;

  virtual void SetCoord(float x, float y, float frameCount = 0, float easing = 0) = 0;
  virtual void GetCoord(float &x, float &y) const = 0;

  virtual void SetScale(float scale, float frameCount = 0, float easing = 0) = 0;
  virtual float GetScale(void) const = 0;

  virtual void SetRot(float rot, float frameCount = 0, float easing = 0) = 0;
  virtual float GetRot(void) const = 0;
  
  virtual void SetColor(emote_uint32_t rgba, float frameCount = 0, float easing = 0) = 0;
  virtual emote_uint32_t GetColor(void) const = 0;

  virtual void SetGrayscale(float rate, float frameCount = 0, float easing = 0) = 0;
  virtual float GetGrayscale(void) const = 0;

  virtual void SetAsOriginalScale(bool state) = 0;
  virtual bool IsAsOriginalScale(void) const = 0;

  virtual float GetState(const char *label) = 0;

  virtual emote_uint32_t CountVariables(void) const = 0;
  virtual const char *GetVariableLabelAt(emote_uint32_t variableIndex) const = 0;
  virtual emote_uint32_t CountVariableFrameAt(emote_uint32_t variableIndex) const = 0;
  virtual const char *GetVariableFrameLabelAt(emote_uint32_t variableIndex, emote_uint32_t frameIndex) const = 0;
  virtual float GetVariableFrameValueAt(emote_uint32_t variableIndex, emote_uint32_t frameIndex) const = 0;

  virtual void AddUserDefinedVariable(const char *label) = 0;
  virtual void SetVariable(const char *label, float value, float frameCount = 0, float easing = 0) = 0;
  virtual float GetVariable(const char *label) const = 0;

  virtual void SetVariableDiff(const char *module, const char *label, float value, float frameCount = 0, float easing = 0) = 0;
  virtual float GetVariableDiff(const char *module, const char *label) const = 0;

  virtual void SetOuterForce(const char *label, float ofx, float ofy, float frameCount = 0, float easing = 0) = 0;
  virtual void GetOuterForce(const char *label, float &ofx, float &ofy) const = 0;

  virtual void StartWind(float start, float goal, float speed, float powMin, float powMax) = 0;
  virtual void StopWind(void) = 0;

  virtual emote_uint32_t CountMainTimelines(void) const = 0;
  virtual const char *GetMainTimelineLabelAt(emote_uint32_t index) const = 0;
  virtual emote_uint32_t CountDiffTimelines(void) const = 0;
  virtual const char *GetDiffTimelineLabelAt(emote_uint32_t index) const = 0;
  virtual emote_uint32_t CountPlayingTimelines(void) const = 0;
  virtual const char *GetPlayingTimelineLabelAt(emote_uint32_t index) const = 0;
  virtual emote_uint32_t GetPlayingTimelineFlagsAt(emote_uint32_t index) const = 0;
  virtual bool IsLoopTimeline(const char *label) const = 0;
  virtual float GetTimelineTotalFrameCount(const char *label) const = 0;
  virtual void PlayTimeline(const char *label, emote_uint32_t flags = 0) = 0;
  virtual bool IsTimelinePlaying(const char *label = "") const = 0;
  virtual void StopTimeline(const char *label = "") = 0;
  virtual void SetTimelineBlendRatio(const char *label, float value, float frameCount = 0, float easing = 0, bool stopWhenBlendDone = false) = 0;
  virtual float GetTimelineBlendRatio(const char *label) const = 0;
  virtual void FadeInTimeline(const char *label, float frameCount = 0, float easing = 0) = 0;
  virtual void FadeOutTimeline(const char *label, float frameCount = 0, float easing = 0) = 0;
  
  virtual bool IsAnimating(void) const = 0;
  virtual void Skip(void) = 0;
  virtual void Pass(void) = 0;
  virtual void Step(void) = 0;
  virtual void Progress(float frameCount) = 0;
  virtual bool IsModified(void) const = 0;
  virtual void Render(void) = 0;

  virtual void SetStereovisionEnabled(bool state) = 0;
  virtual bool GetStereovisionEnabled(void) const = 0;
  virtual void SetStereovisionVolume(float volume) = 0;
  virtual float GetStereovisionVolume(void) const = 0;
  virtual void SetStereovisionParallaxRatio(float ratio) = 0;
  virtual float GetStereovisionParallaxRatio(void) const = 0;
  virtual void SetStereovisionRenderScreen(emote_uint32_t index) = 0;
  virtual emote_uint32_t GetStereovisionRenderScreen(void) const = 0;



  virtual bool IsCharaProfileAvailable(void) const = 0;
  virtual float GetCharaHeight(void) const = 0;
  virtual emote_uint32_t CountCharaProfiles(void) const = 0;
  virtual const char *GetCharaProfileLabelAt(emote_uint32_t profileIndex) const = 0;
  virtual float GetCharaProfile(const char *label) const = 0;

  virtual void SetOuterRot(float rot, float frameCount = 0, float easing = 0) = 0;
  virtual float GetOuterRot(void) const = 0;

  virtual void StartRecordAPILog(void) = 0;
  virtual void StopRecordAPILog(void) = 0;
  virtual bool IsRecordingAPILog(void) const = 0;
  virtual void StartReplayAPILog(void) = 0;
  virtual void StopReplayAPILog(void) = 0;
  virtual bool IsReplayingAPILog(void) const = 0;
  virtual void ClearAPILog(void) = 0;
  virtual const char *GetAPILog(void) const = 0;
  virtual void SetAPILog(const char *log) = 0;

  virtual emote_uint32_t GetSuitableClearColor(void) const = 0;
};


typedef IEmoteDevice* LPEMOTEDEVICE;
typedef IEmoteDevice* PEMOTEDEVICE;


typedef IEmotePlayer* LPEMOTEPLAYER;
typedef IEmotePlayer* PEMOTEPLAYER;


_EXPORT IEmoteDevice *EmoteCreate(const IEmoteDevice::InitParam &param);
_EXPORT const char *EmoteGetBuildDateTime(void);
_EXPORT const char *EmoteGetSDKVersion(void);


typedef void(*FP_EMOTE_FILTER_FUNC)(emote_uint8_t *image, emote_uint32_t imageSize);

_EXPORT void EmoteFilterTexture(emote_uint8_t *image, emote_uint32_t imageSize, FP_EMOTE_FILTER_FUNC filterFunc);
_EXPORT bool EmoteCheckValidObject(const emote_uint8_t *image, emote_uint32_t imageSize);


#endif // __EMOTE_DRIVER_H
