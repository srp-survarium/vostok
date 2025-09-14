/**************************************************************************

PublicHeader:   GFx
Filename    :   AS3_Abc_Type.h
Content     :   
Created     :   Jan, 2010
Authors     :   Sergey Sikorskiy

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_AS3_Abc_Type_H
#define INC_AS3_Abc_Type_H

#include "Kernel/SF_Array.h"
#include "Kernel/SF_ArrayPaged.h"
#include "Kernel/SF_StringHash.h"
#include "Kernel/SF_Alg.h"
#include "../AS3_StringManager.h"
#include "GFx/GFx_PlayerStats.h"

//#define VERSION_AIR

#ifdef SF_BUILD_DEBUG
#define GFX_AS3_TRACE
#endif

///////////////////////////////////////////////////////////////////////////////
#if 0
    #define REPORT_NOT_IMPLEMENTED
#else
/*
    // Minimalistic version of VM.
    #define AS3_NO_FLASH
    #ifdef GFX_AS3_VERBOSE
        #undef GFX_AS3_VERBOSE
    #endif
*/
#endif

///////////////////////////////////////////////////////////////////////////////
#if defined(GFX_AS3_VERBOSE)

#define SF_DEBUG_ARG(x) , x
#define SF_DEBUG_CODE(x) x

#else

#define SF_DEBUG_ARG(x)
#define SF_DEBUG_CODE(x)

#endif

#define WARN_NOT_IMPLEMENTED(x) GetVM().GetUI().Output(FlashUI::Output_Warning, "The method " x " is not implemented\n");
#define WARN_NOT_IMPLEMENTED_STATIC(x) vm.GetUI().Output(FlashUI::Output_Warning, "The method " x " is not implemented\n");

#define WARN_NOT_SUPPORTED_MISSING_DEPENDENCY(x) GetVM().GetUI().Output(FlashUI::Output_Warning, "The method " x " is not supported because core dependencies have been removed\n");

#ifdef REPORT_NOT_IMPLEMENTED
//	#define NOT_IMPLEMENTED(x) GetVM().ThrowError(VM::Error(VM::eNotImplementedError, GetVM() SF_DEBUG_ARG(x)));
//    #define NOT_IMPLEMENTED_STATIC(x) vm.ThrowError(VM::Error(VM::eNotImplementedError, vm SF_DEBUG_ARG(x)));
#else
//    #define NOT_IMPLEMENTED(x) WARN_NOT_IMPLEMENTED(x)
//    #define NOT_IMPLEMENTED_STATIC(x) WARN_NOT_IMPLEMENTED_STATIC(x)
#endif


namespace Scaleform
{

///////////////////////////////////////////////////////////////////////////
enum { Mem_Stat = GFx::StatMV_VM };


///////////////////////////////////////////////////////////////////////////
// String utilities.

inline
String operator +(const char* l, const String& r)
{
    return String(l, r);
}

///////////////////////////////////////////////////////////////////////////
inline
String operator +(const char* l, const GFx::ASString& r)
{
    return String(l) + String(r.ToCStr(), r.GetSize());
}

inline
String operator +(const String& l, const GFx::ASString& r)
{
    return l + String(r.ToCStr(), r.GetSize());
}

} // namespace Scaleform


///////////////////////////////////////////////////////////////////////////
namespace Scaleform { namespace GFx { namespace AS3 
{

///////////////////////////////////////////////////////////////////////////
// Forward declaration.
class Object;
class Class;
class Value;
class VM;

/////////////////////////////////////////////////////////////////////////// 

// ***** SPtr<>

// SPtr is a smart pointer used for garbage-collectible classes within the VM.
// A special aspect of this class is that it uses lower bit of the pointer
// value as a marker to disable the next Release() call as needed for
// garbage collection of cycles. This bit is only applied by the garbage
// collector, allowing object destructor to be used instead of having an
// explicit finalize method, which would be error-prone.
//
// All member SPtr<> references to child objects must be referenced from
// the ForEachChild_GC() virtual function of that class for collector to
// see it properly.

template <typename T>
class SPtr
{
public:
    friend class Value;
    template <typename AT> friend class APtr;

    typedef T       ObjType;
    typedef SPtr<T> SelfType;

public:
    // Constructors.

    SPtr(ObjType* v = NULL)
        : pObject(v)
    {
        SF_ASSERT(v == NULL || (Scaleform::UPInt)v > 4);
        AddRef();
    }
    // Copy constructor.
    SPtr(const SPtr<ObjType>& other)
        : pObject(other.pObject)
    {
        AddRef();
    }
    ~SPtr()
    {
        Release();
    }

public:
    // Pick constructors.
    // We save on AddRef().

    SPtr(SelfType& other, PickType)
        : pObject(other.pObject)
    {
        other.pObject = NULL;
        // No AddRef() on purpose.
    }
    SPtr(Pickable<ObjType> v)
        : pObject(v.GetPtr())
    {
        SF_ASSERT(v.GetPtr() == NULL || (Scaleform::UPInt)v.GetPtr() > 4);
        // No AddRef() on purpose.
    }

public:
    // Assignment operator.
    SelfType& operator =(const SelfType& other)
    {
        return Set(other);
    }
    SelfType& operator =(ObjType* p)
    {
        return SetPtr(p);
    }
    SelfType& operator =(Pickable<ObjType> v)
    {
        return Pick(v);
    }

public:
    // Pickers.

    // Pick a value.
    SelfType& Pick(SelfType& other)
    {
        if (&other != this)
        {
            Release();

            pObject = other.pObject;
            other.pObject = NULL;
        }

        return *this;
    }

    SelfType& Pick(Pickable<ObjType> v)
    {
        if (v.GetPtr() != pObject)
        {
            Release();

            pObject = v.GetPtr();
        }

        return *this;
    }

    SelfType& Pick(ObjType* p)
    {
        if (p != pObject)
        {
            Release();

            pObject = p;
        }

        return *this;
    }

public:
    // Setters.

    SelfType& SetPtr(ObjType* p)
    {
        if (p != pObject)
        {
            SF_ASSERT(p == NULL || (Scaleform::UPInt)p > 4);
            Release();
            if (p)
                p->AddRef();

            pObject = p;
        }

        return *this;
    }
    SelfType& Set(const SelfType& other)
    {
        if (&other != this)
        {
            Release();
            other.AddRef();

            pObject = other.pObject;
        }

        return *this;
    }

public:
    // Getters.

    ObjType* operator->() const
    {
        return pObject;
    }
    ObjType& operator*() const
    {
        SF_ASSERT(pObject);
        return *pObject;
    }

    // Obtain pointer reference directly, for D3D interfaces
    ObjType*& GetRawRef() { return pObject; }
    ObjType** GetRawPtr() const { return &pObject; }
    ObjType*  GetPtr() const
    {
        return pObject;
    }
    // Conversion
    operator ObjType*() const
    {
        return pObject;
    }

private:
    void AddRef() const
    {
        if (pObject)
            pObject->AddRef();
    }
    void Release()
    {
        if (pObject)
        {
            if ((Scaleform::UPInt)pObject & 1)
                pObject = (ObjType*)((Scaleform::UPInt)pObject - 1);
            else
                pObject->Release();
        }
    }

private:
    mutable ObjType* pObject;
}; // SPtr

///////////////////////////////////////////////////////////////////////////
namespace ClassTraits
{
    class Traits;
}

typedef Pickable<ClassTraits::Traits> (*TTraitsFactory)(VM& vm);

struct ThunkInfo;
struct MemberInfo;

struct ClassInfo
{
    enum {
        TimeTypeOffset = 0,
        ObjectTypeOffset = 1,
        ImplTypeOffset = 2,
    };
    enum {
        TimeTypeMask = 1 << TimeTypeOffset,
        ObjectTypeMask = 1 << ObjectTypeOffset,
        ImplTypeMask = 1 << ImplTypeOffset,
    };

    enum TimeType {RunTime = 0, CompileTime = 1 << ObjectTypeOffset};
    enum ObjectType {SealedObject = 0, DynamicObject = 1 << TimeTypeOffset};
    enum ImplType {Cpp = 0, Abc = 1 << ImplTypeOffset};

    bool IsCompileTime() const { return (Flags & TimeTypeMask) != 0; }
    bool IsDynamicObject() const { return (Flags & ObjectTypeMask) != 0; }
    bool IsAbcObject() const { return (Flags & ImplTypeMask) != 0; }

    UPInt               Flags;
    const char*         Name;
    const char*         PkgName;
    const ClassInfo*    Parent;
    TTraitsFactory      Factory;
    UInt8               ClassMethodNum;
    UInt8               ClassMemberNum;
    UInt8               InstanceMethodNum;
    UInt8               InstanceMemberNum;
    const ThunkInfo*    ClassMethod;
    const MemberInfo*   ClassMember;
    const ThunkInfo*    InstanceMethod;
    const MemberInfo*   InstanceMember;
};

namespace Classes
{
#ifdef GFX_AS3_SUPPORT
    extern const ClassInfo* ClassRegistrationTable[];
#endif
}

///////////////////////////////////////////////////////////////////////////
extern const ClassInfo ObjectCI;
extern const ClassInfo StringCI;
extern const ClassInfo BooleanCI;
extern const ClassInfo int_CI;
extern const ClassInfo uintCI;
extern const ClassInfo NumberCI;
extern const ClassInfo NamespaceCI;
extern const ClassInfo ArrayCI;
extern const ClassInfo FunctionCI;
extern const ClassInfo FunctionCICpp;
extern const ClassInfo CatchCI;
extern const ClassInfo VectorCI;
extern const ClassInfo Vector_objectCI;
extern const ClassInfo Vector_intCI;
extern const ClassInfo Vector_uintCI;
extern const ClassInfo Vector_doubleCI;
extern const ClassInfo Vector_StringCI;

extern const ClassInfo VideoCI;
extern const ClassInfo NetStreamCI;

extern const ClassInfo ISearchableTextCI;
extern const ClassInfo ISimpleTextSelectionCI;
extern const ClassInfo AccessibilityCI;
extern const ClassInfo AccessibilityImplementationCI;
extern const ClassInfo AccessibilityPropertiesCI;
extern const ClassInfo ErrorCI;
extern const ClassInfo DefinitionErrorCI;
extern const ClassInfo EvalErrorCI;
extern const ClassInfo RangeErrorCI;
extern const ClassInfo ReferenceErrorCI;
extern const ClassInfo SecurityErrorCI;
extern const ClassInfo SyntaxErrorCI;
extern const ClassInfo TypeErrorCI;
extern const ClassInfo URIErrorCI;
extern const ClassInfo VerifyErrorCI;
extern const ClassInfo UninitializedErrorCI;
extern const ClassInfo ArgumentErrorCI;
extern const ClassInfo DateCI;
extern const ClassInfo MathCI;
extern const ClassInfo RegExpCI;
extern const ClassInfo ActionScriptVersionCI;
extern const ClassInfo AVM1MovieCI;
extern const ClassInfo BitmapCI;
extern const ClassInfo BitmapDataCI;
extern const ClassInfo BitmapDataChannelCI;
extern const ClassInfo BlendModeCI;
extern const ClassInfo CapsStyleCI;
extern const ClassInfo DisplayObjectCI;
extern const ClassInfo DisplayObjectContainerCI;
extern const ClassInfo FrameLabelCI;
extern const ClassInfo GradientTypeCI;
extern const ClassInfo GraphicsCI;
extern const ClassInfo IBitmapDrawableCI;
extern const ClassInfo InteractiveObjectCI;
extern const ClassInfo InterpolationMethodCI;
extern const ClassInfo JointStyleCI;
extern const ClassInfo LineScaleModeCI;
extern const ClassInfo LoaderCI;
extern const ClassInfo LoaderInfoCI;
extern const ClassInfo MorphShapeCI;
extern const ClassInfo MovieClipCI;
extern const ClassInfo PixelSnappingCI;
extern const ClassInfo SceneCI;
extern const ClassInfo ShapeCI;
extern const ClassInfo SimpleButtonCI;
extern const ClassInfo SpreadMethodCI;
extern const ClassInfo SpriteCI;
extern const ClassInfo StageCI;
extern const ClassInfo StageAlignCI;
extern const ClassInfo StageDisplayStateCI;
extern const ClassInfo StageQualityCI;
extern const ClassInfo StageScaleModeCI;
extern const ClassInfo SWFVersionCI;
extern const ClassInfo ShaderDataCI;
extern const ClassInfo ShaderCI;
extern const ClassInfo IllegalOperationErrorCI;
extern const ClassInfo InvalidSWFErrorCI;
extern const ClassInfo IOErrorCI;
extern const ClassInfo EOFErrorCI;
extern const ClassInfo MemoryErrorCI;
extern const ClassInfo ScriptTimeoutErrorCI;
extern const ClassInfo StackOverflowErrorCI;
extern const ClassInfo ActivityEventCI;
extern const ClassInfo AsyncErrorEventCI;
extern const ClassInfo ContextMenuEventCI;
extern const ClassInfo DataEventCI;
extern const ClassInfo ErrorEventCI;
extern const ClassInfo EventCI;
extern const ClassInfo EventDispatcherCI;
extern const ClassInfo EventPhaseCI;
extern const ClassInfo FocusEventCI;
extern const ClassInfo FullScreenEventCI;
extern const ClassInfo HTTPStatusEventCI;
extern const ClassInfo IEventDispatcherCI;
extern const ClassInfo IOErrorEventCI;
extern const ClassInfo KeyboardEventCI;
extern const ClassInfo MouseEventCI;
extern const ClassInfo NetStatusEventCI;
extern const ClassInfo ProgressEventCI;
extern const ClassInfo SecurityErrorEventCI;
extern const ClassInfo StatusEventCI;
extern const ClassInfo SyncEventCI;
extern const ClassInfo TextEventCI;
extern const ClassInfo TimerEventCI;
extern const ClassInfo GestureEventCI;
extern const ClassInfo TransformGestureEventCI;
extern const ClassInfo PressAndTapGestureEventCI;
extern const ClassInfo TouchEventCI;
extern const ClassInfo GesturePhaseCI;
extern const ClassInfo StageOrientationEventCI;
extern const ClassInfo StageOrientationCI;
extern const ClassInfo ExternalInterfaceCI;
extern const ClassInfo BevelFilterCI;
extern const ClassInfo BitmapFilterCI;
extern const ClassInfo BitmapFilterQualityCI;
extern const ClassInfo BitmapFilterTypeCI;
extern const ClassInfo BlurFilterCI;
extern const ClassInfo ColorMatrixFilterCI;
extern const ClassInfo ConvolutionFilterCI;
extern const ClassInfo DisplacementMapFilterCI;
extern const ClassInfo DisplacementMapFilterModeCI;
extern const ClassInfo DropShadowFilterCI;
extern const ClassInfo GlowFilterCI;
extern const ClassInfo GradientBevelFilterCI;
extern const ClassInfo GradientGlowFilterCI;
extern const ClassInfo ColorTransformCI;
extern const ClassInfo Vector3DCI;
extern const ClassInfo Matrix3DCI;
extern const ClassInfo MatrixCI;
extern const ClassInfo PerspectiveProjectionCI;
extern const ClassInfo PointCI;
extern const ClassInfo RectangleCI;
extern const ClassInfo TransformCI;
extern const ClassInfo URLRequestCI;
extern const ClassInfo ApplicationDomainCI;
extern const ClassInfo CapabilitiesCI;
extern const ClassInfo LoaderContextCI;
extern const ClassInfo SecurityCI;
extern const ClassInfo SecurityDomainCI;
extern const ClassInfo SecurityPanelCI;
extern const ClassInfo SystemCI;
extern const ClassInfo AntiAliasTypeCI;
extern const ClassInfo CSMSettingsCI;
extern const ClassInfo FontCI;
extern const ClassInfo FontStyleCI;
extern const ClassInfo FontTypeCI;
extern const ClassInfo GridFitTypeCI;
extern const ClassInfo StaticTextCI;
extern const ClassInfo StyleSheetCI;
extern const ClassInfo TextColorTypeCI;
extern const ClassInfo TextDisplayModeCI;
extern const ClassInfo TextFieldCI;
extern const ClassInfo TextFieldAutoSizeCI;
extern const ClassInfo TextFieldTypeCI;
extern const ClassInfo TextFormatCI;
extern const ClassInfo TextFormatAlignCI;
extern const ClassInfo TextLineMetricsCI;
extern const ClassInfo TextRendererCI;
extern const ClassInfo TextSnapshotCI;
extern const ClassInfo ContextMenuBuiltInItemsCI;
extern const ClassInfo ContextMenuClipboardItemsCI;
extern const ClassInfo ContextMenuItemCI;
extern const ClassInfo ContextMenuCI;
extern const ClassInfo KeyboardCI;
extern const ClassInfo KeyLocationCI;
extern const ClassInfo MouseCI;
extern const ClassInfo MouseCursorCI;
extern const ClassInfo ByteArrayCI;
extern const ClassInfo DictionaryCI;
extern const ClassInfo EndianCI;
extern const ClassInfo IDataInputCI;
extern const ClassInfo IDataOutputCI;
extern const ClassInfo ProxyCI;
extern const ClassInfo TimerCI;
extern const ClassInfo SystemExCI;
extern const ClassInfo FocusManagerCI;
extern const ClassInfo ExtensionsCI;
extern const ClassInfo MouseCursorEventCI;
extern const ClassInfo InteractiveObjectExCI;
extern const ClassInfo TextFieldExCI;
extern const ClassInfo MouseEventExCI;
extern const ClassInfo KeyboardEventExCI;
extern const ClassInfo GamePadCI;
extern const ClassInfo GamePadAnalogEventCI;
extern const ClassInfo FocusEventExCI;
extern const ClassInfo TextEventExCI;
extern const ClassInfo IMEEventExCI;
extern const ClassInfo IMEExCI;
extern const ClassInfo IMECandidateListStyleCI;
extern const ClassInfo XMLCI;
extern const ClassInfo XMLListCI;
extern const ClassInfo QNameCI;
extern const ClassInfo XMLSocketCI;
extern const ClassInfo XMLSignatureValidatorCI;
extern const ClassInfo XMLDocumentCI;
extern const ClassInfo XMLNodeCI;
extern const ClassInfo XMLNodeTypeCI;
extern const ClassInfo ClipboardCI;
extern const ClassInfo ClipboardFormatsCI;
extern const ClassInfo ClipboardTransferModeCI;
extern const ClassInfo IconCI;
extern const ClassInfo InteractiveIconCI;
extern const ClassInfo DockIconCI;
extern const ClassInfo NativeApplicationCI;
extern const ClassInfo NativeDragActionsCI;
extern const ClassInfo NativeDragManagerCI;
extern const ClassInfo NativeDragOptionsCI;
extern const ClassInfo NotificationTypeCI;
extern const ClassInfo SystemTrayIconCI;
extern const ClassInfo UpdaterCI;
extern const ClassInfo FocusDirectionCI;
extern const ClassInfo NativeMenuCI;
extern const ClassInfo NativeMenuItemCI;
extern const ClassInfo NativeWindowCI;
extern const ClassInfo NativeWindowDisplayStateCI;
extern const ClassInfo NativeWindowInitOptionsCI;
extern const ClassInfo NativeWindowResizeCI;
extern const ClassInfo NativeWindowSystemChromeCI;
extern const ClassInfo NativeWindowTypeCI;
extern const ClassInfo ScreenCI;
extern const ClassInfo BrowserInvokeEventCI;
extern const ClassInfo DRMAuthenticateEventCI;
extern const ClassInfo DRMErrorEventCI;
extern const ClassInfo DRMStatusEventCI;
extern const ClassInfo FileListEventCI;
extern const ClassInfo HTMLUncaughtScriptExceptionEventCI;
extern const ClassInfo InvokeEventCI;
extern const ClassInfo NativeDragEventCI;
extern const ClassInfo NativeWindowBoundsEventCI;
extern const ClassInfo NativeWindowDisplayStateEventCI;
extern const ClassInfo OutputProgressEventCI;
extern const ClassInfo ScreenMouseEventCI;
extern const ClassInfo FileCI;
extern const ClassInfo FileModeCI;
extern const ClassInfo FileStreamCI;
extern const ClassInfo HTMLHistoryItemCI;
extern const ClassInfo HTMLHostCI;
extern const ClassInfo HTMLLoaderCI;
extern const ClassInfo HTMLPDFCapabilityCI;
extern const ClassInfo HTMLWindowCreateOptionsCI;
extern const ClassInfo URLRequestDefaultsCI;
extern const ClassInfo PrintJobCI;
extern const ClassInfo PrintJobOptionsCI;
extern const ClassInfo PrintJobOrientationCI;
extern const ClassInfo DeleteObjectSampleCI;
extern const ClassInfo NewObjectSampleCI;
extern const ClassInfo SampleCI;
extern const ClassInfo StackFrameCI;
extern const ClassInfo IURIDereferencerCI;
extern const ClassInfo ReferencesValidationSettingCI;
extern const ClassInfo RevocationCheckSettingsCI;
extern const ClassInfo SignatureStatusCI;
extern const ClassInfo SignerTrustSettingsCI;
extern const ClassInfo CompressionAlgorithmCI;
extern const ClassInfo ID3InfoCI;
extern const ClassInfo MicrophoneCI;
extern const ClassInfo SoundCI;
extern const ClassInfo SoundChannelCI;
extern const ClassInfo SoundLoaderContextCI;
extern const ClassInfo SoundMixerCI;
extern const ClassInfo SoundTransformCI;
extern const ClassInfo IMEEventCI;
extern const ClassInfo IMECI;
extern const ClassInfo IMEConversionModeCI;
extern const ClassInfo CameraCI;
extern const ClassInfo FileFilterCI;
extern const ClassInfo FileReferenceCI;
extern const ClassInfo FileReferenceListCI;
extern const ClassInfo IDynamicPropertyOutputCI;
extern const ClassInfo IDynamicPropertyWriterCI;
extern const ClassInfo LocalConnectionCI;
extern const ClassInfo NetConnectionCI;
extern const ClassInfo ObjectEncodingCI;
extern const ClassInfo ResponderCI;
extern const ClassInfo SharedObjectCI;
extern const ClassInfo SharedObjectFlushStatusCI;
extern const ClassInfo SocketCI;
extern const ClassInfo URLLoaderCI;
extern const ClassInfo URLLoaderDataFormatCI;
extern const ClassInfo URLRequestHeaderCI;
extern const ClassInfo URLRequestMethodCI;
extern const ClassInfo URLStreamCI;
extern const ClassInfo URLVariablesCI;
extern const ClassInfo MultitouchCI;
extern const ClassInfo MultitouchInputModeCI;
extern const ClassInfo IExternalizableCI;

///////////////////////////////////////////////////////////////////////////
namespace Abc
{
    ///////////////////////////////////////////////////////////////////////////
    // Instruction offset within TOpCode (which is an OpCodeBuffer)
    typedef UPInt TCodeOffset;

    ///////////////////////////////////////////////////////////////////////////
    // NamespaceKind describes the type of namespace NamespaceInfo represents.
    enum NamespaceKind
    {
        NS_Undefined = -1,
        NS_Public,
        NS_Protected,
        NS_StaticProtected,
        NS_Private,  // Private NS shouldn't be interned (private namespaces don't match).
        NS_Explicit, // What is this for? Perhaps it identifies NSs with explicit URIs?
        NS_PackageInternal
    };

}

}}} // namespace Scaleform { namespace GFx { namespace AS3 {


#endif // INC_AS3_Abc_Type_H
