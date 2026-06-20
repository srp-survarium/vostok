#!/usr/bin/env python3
"""Generate libgfx.vcproj (VS2008, static lib) for the 212 Win32 GFx 4.2.22 TUs."""
from pathlib import Path

TU_LIST = (Path(__file__).resolve().parent / "libgfx_tus.txt").read_text().split()
# RelativePath from sources/vostok/libgfx/sources/ to the build SDK TU.
relpaths = ["..\\..\\..\\scaleform_build\\" + t.replace("/", "\\") for t in TU_LIST]

GUID = "{B9F3A1C2-7E4D-4A18-9C56-2D8F1B3E64A0}"

# Include dirs (semicolon list, $(SolutionDir)-relative). Mirror GFx_Lib.vsprops
# roots pointed at the build SDK + engine headers + stlport, plus the 3rdParty.
INCLUDES = ";".join([
    "$(SolutionDir)/stlport",
    "$(SolutionDir)",
    "$(SolutionDir)/vostok/libgfx/sources",
    "$(SolutionDir)/scaleform_build/Include",
    "$(SolutionDir)/scaleform_build/Src",
    "$(SolutionDir)/scaleform_build/3rdParty/zlib-1.2.7",
    "$(SolutionDir)/scaleform_build/3rdParty/jpeg-8d",
    "$(SolutionDir)/scaleform_build/3rdParty/libpng-1.5.13",
    "$(SolutionDir)/scaleform_build/3rdParty/expat-2.1.0/lib",
    "$(SolutionDir)/scaleform_build/3rdParty/pcre",
    "$(SolutionDir)/scaleform_build/3rdParty/glext",
    "$(SolutionDir)/scaleform_build/3rdParty/PlatformSDK",
])

OUTPUT = "$(SolutionDir)../binaries.prebuilt/Win32/libraries/shipping/libgfx.lib"

# Master Gold|Win32 compiler tool - mirror scaleform.vcproj's Master Gold config
# (the survarium module flags that already compile GFx TUs): /Od + /GL + MASTER_GOLD,
# GFx_Lib defines (WIN32/_WINDOWS/SF_BUILD_STATICLIB), force-include the engine
# preamble pch.h (the SDK TUs are upstream source we don't edit).
# SF_BUILD_SHIPPING: the shipped binary has ZERO AMP symbols (Scaleform::AMP::*,
# AmpServer::*), so the GFx Shipping config (which undefines SF_AMP_SERVER, gutting
# the Amp_*.cpp TUs) was used - without it the AMP socket TU pulls Winsock and the
# exe link breaks. MASTER_GOLD (the patch gate) and SF_BUILD_SHIPPING coexist.
CL_TOOL = f'''		<Tool
			Name="VCCLCompilerTool"
			Optimization="0"
			InlineFunctionExpansion="2"
			EnableIntrinsicFunctions="true"
			FavorSizeOrSpeed="1"
			OmitFramePointers="true"
			EnableFiberSafeOptimizations="true"
			AdditionalIncludeDirectories="{INCLUDES}"
			PreprocessorDefinitions="WIN32;_WINDOWS;SF_BUILD_STATICLIB;SF_BUILD_SHIPPING;NDEBUG;VOSTOK_STATIC_LIBRARIES;MASTER_GOLD"
			StringPooling="true"
			ExceptionHandling="0"
			BasicRuntimeChecks="0"
			RuntimeLibrary="0"
			BufferSecurityCheck="false"
			EnableEnhancedInstructionSet="2"
			FloatingPointModel="2"
			UsePrecompiledHeader="0"
			WarningLevel="3"
			DebugInformationFormat="1"
		/>'''


def config_block(name):
    return f'''		<Configuration
			Name="{name}"
			OutputDirectory="$(SolutionDir)../binaries/$(PlatformName)/intermediates/$(ConfigurationName)/$(ProjectName)"
			IntermediateDirectory="$(SolutionDir)../binaries/$(PlatformName)/intermediates/$(ConfigurationName)/$(ProjectName)"
			ConfigurationType="4"
			CharacterSet="2"
			WholeProgramOptimization="1"
			>
			<Tool Name="VCPreBuildEventTool" />
			<Tool Name="VCCustomBuildTool" />
			<Tool Name="VCXMLDataGeneratorTool" />
			<Tool Name="VCWebServiceProxyGeneratorTool" />
			<Tool Name="VCMIDLTool" />
{CL_TOOL}
			<Tool Name="VCManagedResourceCompilerTool" />
			<Tool Name="VCResourceCompilerTool" />
			<Tool Name="VCPreLinkEventTool" />
			<Tool
				Name="VCLibrarianTool"
				OutputFile="{OUTPUT}"
			/>
			<Tool Name="VCALinkTool" />
			<Tool Name="VCXDCMakeTool" />
			<Tool Name="VCBscMakeTool" />
			<Tool Name="VCFxCopTool" />
			<Tool Name="VCPostBuildEventTool" />
		</Configuration>'''


files = []
for rp in relpaths:
    files.append(f'''			<File
				RelativePath="{rp}"
				>
			</File>''')
files_block = "\n".join(files)

# pch.h is a project file too (for clangd/visibility), but it is /FI-forced, not built.
pch_file = '''			<File
				RelativePath=".\\pch.h"
				>
			</File>'''

vcproj = f'''<?xml version="1.0" encoding="windows-1251"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="9.00"
	Name="libgfx"
	ProjectGUID="{GUID}"
	RootNamespace="libgfx"
	Keyword="Win32Proj"
	TargetFrameworkVersion="131072"
	>
	<Platforms>
		<Platform Name="Win32" />
	</Platforms>
	<ToolFiles>
	</ToolFiles>
	<Configurations>
{config_block("Master Gold|Win32")}
	</Configurations>
	<References>
	</References>
	<Files>
		<Filter Name="core">
{pch_file}
		</Filter>
		<Filter Name="source">
{files_block}
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
'''

out = Path("/home/sheep/Projects/surv/vostok_4/sources/vostok/libgfx/sources/libgfx.vcproj")
out.write_text(vcproj)
print(f"Wrote {out} with {len(relpaths)} TUs")
