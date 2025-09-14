/**************************************************************************

Filename    :   AS3_Global.h
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_AS3_Global_H
#define INC_AS3_Global_H

#include "Abc/AS3_Abc_Type.h"

#define SF_AS3_VERSION_MULTITOUCH
#define SF_AS3_VERSION_XML
#define SF_AS3_VERSION_SOUND
#define SF_AS3_VERSION_NET
#define SF_AS3_VERSION_VIDEO
#define SF_AS3_VERSION_IME
#define SF_AS3_VERSION_REST

#ifdef GFX_AS3_SUPPORT

namespace Scaleform { namespace GFx { namespace AS3 
{
    
namespace Classes
{
    ///////////////////////////////////////////////////////////////////////////
    // Forward declaration.
    const ClassInfo* ClassRegistrationTable[] =
    {
//##markup##"obj_global_cpp$inheritance_table"       
//##begin##"obj_global_cpp$inheritance_table"

#ifndef SF_AS3_NO_FLASH
        &ISearchableTextCI,
        &ISimpleTextSelectionCI,
        &AccessibilityCI,
        &AccessibilityImplementationCI,
        &AccessibilityPropertiesCI,
        &ErrorCI,
        &DefinitionErrorCI,
        &EvalErrorCI,
        &RangeErrorCI,
        &ReferenceErrorCI,
        &SecurityErrorCI,
        &SyntaxErrorCI,
        &TypeErrorCI,
        &URIErrorCI,
        &VerifyErrorCI,
        &UninitializedErrorCI,
        &ArgumentErrorCI,
        &DateCI,
        &MathCI,
        &RegExpCI,
        &ActionScriptVersionCI,
        &AVM1MovieCI,
        &BitmapCI,
        &BitmapDataCI,
        &BitmapDataChannelCI,
        &BlendModeCI,
        &CapsStyleCI,
        &DisplayObjectCI,
        &DisplayObjectContainerCI,
        &FrameLabelCI,
        &GradientTypeCI,
        &GraphicsCI,
        &IBitmapDrawableCI,
        &InteractiveObjectCI,
        &InterpolationMethodCI,
        &JointStyleCI,
        &LineScaleModeCI,
        &LoaderCI,
        &LoaderInfoCI,
        &MorphShapeCI,
        &MovieClipCI,
        &PixelSnappingCI,
        &SceneCI,
        &ShapeCI,
        &SimpleButtonCI,
        &SpreadMethodCI,
        &SpriteCI,
        &StageCI,
        &StageAlignCI,
        &StageDisplayStateCI,
        &StageQualityCI,
        &StageScaleModeCI,
        &SWFVersionCI,
        &ShaderDataCI,
        &ShaderCI,
        &IllegalOperationErrorCI,
        &InvalidSWFErrorCI,
        &IOErrorCI,
        &EOFErrorCI,
        &MemoryErrorCI,
        &ScriptTimeoutErrorCI,
        &StackOverflowErrorCI,
        &ActivityEventCI,
        &AsyncErrorEventCI,
        &ContextMenuEventCI,
        &DataEventCI,
        &ErrorEventCI,
        &EventCI,
        &EventDispatcherCI,
        &EventPhaseCI,
        &FocusEventCI,
        &FullScreenEventCI,
        &HTTPStatusEventCI,
        &IEventDispatcherCI,
        &IOErrorEventCI,
        &KeyboardEventCI,
        &MouseEventCI,
        &NetStatusEventCI,
        &ProgressEventCI,
        &SecurityErrorEventCI,
        &StatusEventCI,
        &SyncEventCI,
        &TextEventCI,
        &TimerEventCI,
        &GestureEventCI,
        &TransformGestureEventCI,
        &PressAndTapGestureEventCI,
        &TouchEventCI,
        &GesturePhaseCI,
        &StageOrientationEventCI,
        &StageOrientationCI,
        &ExternalInterfaceCI,
        &BevelFilterCI,
        &BitmapFilterCI,
        &BitmapFilterQualityCI,
        &BitmapFilterTypeCI,
        &BlurFilterCI,
        &ColorMatrixFilterCI,
        &ConvolutionFilterCI,
        &DisplacementMapFilterCI,
        &DisplacementMapFilterModeCI,
        &DropShadowFilterCI,
        &GlowFilterCI,
        &GradientBevelFilterCI,
        &GradientGlowFilterCI,
        &ColorTransformCI,
        &Vector3DCI,
        &Matrix3DCI,
        &MatrixCI,
        &PerspectiveProjectionCI,
        &PointCI,
        &RectangleCI,
        &TransformCI,
        &URLRequestCI,
        &ApplicationDomainCI,
        &CapabilitiesCI,
        &LoaderContextCI,
        &SecurityCI,
        &SecurityDomainCI,
        &SecurityPanelCI,
        &SystemCI,
        &AntiAliasTypeCI,
        &CSMSettingsCI,
        &FontCI,
        &FontStyleCI,
        &FontTypeCI,
        &GridFitTypeCI,
        &StaticTextCI,
        &StyleSheetCI,
        &TextColorTypeCI,
        &TextDisplayModeCI,
        &TextFieldCI,
        &TextFieldAutoSizeCI,
        &TextFieldTypeCI,
        &TextFormatCI,
        &TextFormatAlignCI,
        &TextLineMetricsCI,
        &TextRendererCI,
        &TextSnapshotCI,
        &ContextMenuBuiltInItemsCI,
        &ContextMenuClipboardItemsCI,
        &ContextMenuItemCI,
        &ContextMenuCI,
        &KeyboardCI,
        &KeyLocationCI,
        &MouseCI,
        &MouseCursorCI,
        &ByteArrayCI,
        &DictionaryCI,
        &EndianCI,
        &IDataInputCI,
        &IDataOutputCI,
        &ProxyCI,
        &TimerCI,
        &SystemExCI,
        &FocusManagerCI,
        &ExtensionsCI,
        &MouseCursorEventCI,
        &InteractiveObjectExCI,
        &TextFieldExCI,
        &MouseEventExCI,
        &KeyboardEventExCI,
        &GamePadCI,
        &GamePadAnalogEventCI,
        &FocusEventExCI,
        &TextEventExCI,
        &IMEEventExCI,
        &IMEExCI,
        &IMECandidateListStyleCI,
#ifdef SF_AS3_VERSION_XML
        &XMLCI,
        &XMLListCI,
        &QNameCI,
        &XMLSocketCI,
        &XMLSignatureValidatorCI,
        &XMLDocumentCI,
        &XMLNodeCI,
        &XMLNodeTypeCI,
#endif // SF_AS3_VERSION_XML
#ifdef SF_AS3_VERSION_AIR
        &ClipboardCI,
        &ClipboardFormatsCI,
        &ClipboardTransferModeCI,
        &IconCI,
        &InteractiveIconCI,
        &DockIconCI,
        &NativeApplicationCI,
        &NativeDragActionsCI,
        &NativeDragManagerCI,
        &NativeDragOptionsCI,
        &NotificationTypeCI,
        &SystemTrayIconCI,
        &UpdaterCI,
        &FocusDirectionCI,
        &NativeMenuCI,
        &NativeMenuItemCI,
        &NativeWindowCI,
        &NativeWindowDisplayStateCI,
        &NativeWindowInitOptionsCI,
        &NativeWindowResizeCI,
        &NativeWindowSystemChromeCI,
        &NativeWindowTypeCI,
        &ScreenCI,
        &BrowserInvokeEventCI,
        &DRMAuthenticateEventCI,
        &DRMErrorEventCI,
        &DRMStatusEventCI,
        &FileListEventCI,
        &HTMLUncaughtScriptExceptionEventCI,
        &InvokeEventCI,
        &NativeDragEventCI,
        &NativeWindowBoundsEventCI,
        &NativeWindowDisplayStateEventCI,
        &OutputProgressEventCI,
        &ScreenMouseEventCI,
        &FileCI,
        &FileModeCI,
        &FileStreamCI,
        &HTMLHistoryItemCI,
        &HTMLHostCI,
        &HTMLLoaderCI,
        &HTMLPDFCapabilityCI,
        &HTMLWindowCreateOptionsCI,
        &URLRequestDefaultsCI,
        &PrintJobCI,
        &PrintJobOptionsCI,
        &PrintJobOrientationCI,
        &DeleteObjectSampleCI,
        &NewObjectSampleCI,
        &SampleCI,
        &StackFrameCI,
        &IURIDereferencerCI,
        &ReferencesValidationSettingCI,
        &RevocationCheckSettingsCI,
        &SignatureStatusCI,
        &SignerTrustSettingsCI,
        &CompressionAlgorithmCI,
#endif // SF_AS3_VERSION_AIR
#ifdef SF_AS3_VERSION_SOUND
        &ID3InfoCI,
        &MicrophoneCI,
        &SoundCI,
        &SoundChannelCI,
        &SoundLoaderContextCI,
        &SoundMixerCI,
        &SoundTransformCI,
#endif // SF_AS3_VERSION_SOUND
#ifdef SF_AS3_VERSION_IME
        &IMEEventCI,
        &IMECI,
        &IMEConversionModeCI,
#endif // SF_AS3_VERSION_IME
#ifdef SF_AS3_VERSION_VIDEO
        &CameraCI,
#endif // SF_AS3_VERSION_VIDEO
#ifdef SF_AS3_VERSION_NET
        &FileFilterCI,
        &FileReferenceCI,
        &FileReferenceListCI,
        &IDynamicPropertyOutputCI,
        &IDynamicPropertyWriterCI,
        &LocalConnectionCI,
        &NetConnectionCI,
        &ObjectEncodingCI,
        &ResponderCI,
        &SharedObjectCI,
        &SharedObjectFlushStatusCI,
        &SocketCI,
        &URLLoaderCI,
        &URLLoaderDataFormatCI,
        &URLRequestHeaderCI,
        &URLRequestMethodCI,
        &URLStreamCI,
        &URLVariablesCI,
#endif // SF_AS3_VERSION_NET
#ifdef SF_AS3_VERSION_MULTITOUCH
        &MultitouchCI,
        &MultitouchInputModeCI,
#endif // SF_AS3_VERSION_MULTITOUCH
#ifdef SF_AS3_VERSION_REST
        &IExternalizableCI,
#endif // SF_AS3_VERSION_REST
#endif

//##end##"obj_global_cpp$inheritance_table"
        NULL
    };
} // namespace Classes

}}} // namespace Scaleform { namespace GFx { namespace AS3

#endif // GFX_AS3_SUPPORT

#endif

