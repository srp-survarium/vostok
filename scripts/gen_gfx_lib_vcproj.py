#!/usr/bin/env python3
"""Generate the VS2008 static-lib .vcproj for one (or all) of the 8 sibling GFx libs.

Generalizes gen_libgfx_vcproj.py to the as2/as3/air/expat/sound_fmod (cpp) and
zlib/libpng/libjpeg (c) libs. cpp libs clone libgfx's Master Gold config exactly;
c libs use their own Msvc10 Release|Win32 flags (no /GL, no SF defines, no preamble).

    python3 scripts/gen_gfx_lib_vcproj.py            # all 8
    python3 scripts/gen_gfx_lib_vcproj.py libgfx_as2 # one
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gfx_libs as G

VOSTOK = G.VOSTOK_DIR


def cl_tool_cpp(name):
    inc = ";".join([G.CPP_INCLUDES[0], G.CPP_INCLUDES[1],
                    f"$(SolutionDir)/vostok/{name}/sources"] + G.CPP_INCLUDES[2:])
    return f'''		<Tool
			Name="VCCLCompilerTool"
			Optimization="0"
			InlineFunctionExpansion="2"
			EnableIntrinsicFunctions="true"
			FavorSizeOrSpeed="1"
			OmitFramePointers="true"
			EnableFiberSafeOptimizations="true"
			AdditionalIncludeDirectories="{inc}"
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


def cl_tool_c(name):
    cfg = G.LIBS[name]
    inc = ";".join(G.includes(name))
    # Optimization: "2" = MaxSpeed(/O2); None = compiler default (libpng).
    opt = cfg["opt"]
    opt_attr = f'Optimization="{opt}"\n\t\t\t' if opt is not None else ''
    # InlineFunctionExpansion "1" = /Ob1 (OnlyExplicitInline).
    return f'''		<Tool
			Name="VCCLCompilerTool"
			{opt_attr}InlineFunctionExpansion="{cfg["inline"]}"
			AdditionalIncludeDirectories="{inc}"
			PreprocessorDefinitions="{cfg["defines"]}"
			StringPooling="true"
			ExceptionHandling="0"
			RuntimeLibrary="0"
			RuntimeTypeInfo="false"
			UsePrecompiledHeader="0"
			WarningLevel="3"
			DebugInformationFormat="1"
		/>'''


def gen(name):
    cfg = G.LIBS[name]
    guid = G.GUIDS[name]
    tus = G.tus(name)
    proj_dir = VOSTOK / "sources" / "vostok" / name / "sources"
    proj_dir.mkdir(parents=True, exist_ok=True)

    # RelativePath from sources/vostok/<name>/sources/ to the build SDK TU.
    relpaths = ["..\\..\\..\\scaleform_build\\" + t.replace("/", "\\") for t in tus]
    cl = cl_tool_cpp(name) if cfg["kind"] == "cpp" else cl_tool_c(name)
    out = f"$(SolutionDir)../binaries.prebuilt/Win32/libraries/shipping/{name}.lib"

    config = f'''		<Configuration
			Name="Master Gold|Win32"
			OutputDirectory="$(SolutionDir)../binaries/$(PlatformName)/intermediates/$(ConfigurationName)/$(ProjectName)"
			IntermediateDirectory="$(SolutionDir)../binaries/$(PlatformName)/intermediates/$(ConfigurationName)/$(ProjectName)"
			ConfigurationType="4"
			CharacterSet="2"
			{'WholeProgramOptimization="1"' if cfg["kind"] == "cpp" else ''}
			>
			<Tool Name="VCPreBuildEventTool" />
			<Tool Name="VCCustomBuildTool" />
			<Tool Name="VCXMLDataGeneratorTool" />
			<Tool Name="VCWebServiceProxyGeneratorTool" />
			<Tool Name="VCMIDLTool" />
{cl}
			<Tool Name="VCManagedResourceCompilerTool" />
			<Tool Name="VCResourceCompilerTool" />
			<Tool Name="VCPreLinkEventTool" />
			<Tool
				Name="VCLibrarianTool"
				OutputFile="{out}"
			/>
			<Tool Name="VCALinkTool" />
			<Tool Name="VCXDCMakeTool" />
			<Tool Name="VCBscMakeTool" />
			<Tool Name="VCFxCopTool" />
			<Tool Name="VCPostBuildEventTool" />
		</Configuration>'''

    files = "\n".join(
        f'''			<File
				RelativePath="{rp}"
				>
			</File>''' for rp in relpaths)

    # cpp libs carry the engine preamble pch.h as a project file (clangd visibility).
    pch = ('''			<File
				RelativePath=".\\pch.h"
				>
			</File>
''' if cfg["kind"] == "cpp" else "")

    vcproj = f'''<?xml version="1.0" encoding="windows-1251"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="9.00"
	Name="{name}"
	ProjectGUID="{guid}"
	RootNamespace="{name}"
	Keyword="Win32Proj"
	TargetFrameworkVersion="131072"
	>
	<Platforms>
		<Platform Name="Win32" />
	</Platforms>
	<ToolFiles>
	</ToolFiles>
	<Configurations>
{config}
	</Configurations>
	<References>
	</References>
	<Files>
		<Filter Name="core">
{pch}		</Filter>
		<Filter Name="source">
{files}
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
'''
    (proj_dir / f"{name}.vcproj").write_text(vcproj)
    print(f"Wrote {proj_dir / f'{name}.vcproj'} ({len(relpaths)} TUs, kind={cfg['kind']})")


def main():
    names = sys.argv[1:] or list(G.LIBS)
    for n in names:
        gen(n)


if __name__ == "__main__":
    main()
