use std::fs;
use std::io;
use std::io::Write;
use std::path;

use pdb::PDB;

use crate::data::Files;
use crate::pdb_parser::Formatter;
use crate::GenFlags;
use crate::{gen_headers, gen_sources};

pub fn dump_pdb(
    pdb_path: &path::Path,
    output_path: &path::Path,
    engine_path: &str,
    flags: GenFlags,
) -> crate::Result<()> {
    if output_path.as_os_str().as_encoded_bytes().is_empty() {
        panic!("output_path cannot be empty")
    }

    {
        let mut path = output_path.to_path_buf();
        path.push("sources");
        if path.exists() {
            println!("Removing {}", path.to_string_lossy());
            std::fs::remove_dir_all(&path)?;
        }

        path.pop();
        path.push("headers");
        if path.exists() {
            println!("Removing {}", path.to_string_lossy());
            std::fs::remove_dir_all(&path)?;
        }
    }

    let mut files = Files::default();

    Formatter::with(pdb_path, |fmt| {
        let file = fs::File::open(pdb_path)?;
        let mut pdb = PDB::open(file)?;

        let cache =
            gen_sources::dump_sources(&mut pdb, &fmt, output_path, engine_path, flags, &mut files)?;
        gen_headers::dump_headers(&mut pdb, &fmt, cache, output_path, flags, &mut files)?;

        Ok(())
    })?;

    generate_vs_solution(output_path, flags, &files)
}

pub fn generate_vs_solution(
    output_path: &path::Path,
    flags: GenFlags,
    files: &Files,
) -> crate::Result<()> {
    let name = match flags.contains(GenFlags::AS_BASE) {
        true => "xray_structure",
        false => "vostok_structure",
    };
    let sln_guid = uuid::Uuid::new_v4();
    let vcproj_guid = uuid::Uuid::new_v4();

    generate_vcproj(output_path, vcproj_guid, name, files)?;
    generate_sln(output_path, sln_guid, vcproj_guid, name)?;

    Ok(())
}

pub fn generate_vcproj(
    output_path: &path::Path,
    vcproj_guid: uuid::Uuid,
    name: &str,
    files: &Files,
) -> crate::Result<()> {
    let mut path = output_path.to_path_buf();
    path.push(name);
    path.set_extension("vcproj");
    println!("{}", path.to_string_lossy());

    let file = fs::File::create(path)?;
    let mut file = io::BufWriter::new(file);

    file.write_all(generate_project_header(vcproj_guid, name).as_bytes())?;
    generate_project_filters(files, &mut file)?;
    file.write_all(generate_project_footer().as_bytes())?;

    Ok(())
}

pub fn generate_sln(
    output_path: &path::Path,
    sln_guid: uuid::Uuid,
    vcproj_guid: uuid::Uuid,
    name: &str,
) -> crate::Result<()> {
    let mut path = output_path.to_path_buf();
    path.push(name);
    path.set_extension("sln");
    println!("{}", path.to_string_lossy());

    fs::write(path, generate_solution_source(sln_guid, vcproj_guid, name))?;

    Ok(())
}

//
//
//

pub fn generate_project_header(vcproj_guid: uuid::Uuid, name: &str) -> String {
    let vcproj_guid = vcproj_guid.to_string().to_uppercase();

    format!(
        r#"<?xml version="1.0" encoding="Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="9.00"
	Name="vostok_structure"
	ProjectGUID="{{{vcproj_guid}}}"
	RootNamespace="{name}"
	TargetFrameworkVersion="196613"
	>
	<Platforms>
		<Platform
			Name="Win32"
		/>
	</Platforms>
	<ToolFiles>
	</ToolFiles>
	<Configurations>
		<Configuration
			Name="Debug|Win32"
			OutputDirectory="$(SolutionDir)$(ConfigurationName)"
			IntermediateDirectory="$(ConfigurationName)"
			ConfigurationType="1"
			CharacterSet="2"
			>
		</Configuration>
	</Configurations>
	<References>
	</References>
	<Files>
"#
    )
}

pub fn generate_project_footer() -> String {
    format!(
        r#"
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
"#
    )
}

pub fn generate_project_filters(files: &Files, writer: &mut impl io::Write) -> crate::Result<()> {
    generate_project_filters_impl(files, writer, 2)
}

pub fn generate_project_filters_impl(
    files: &Files,
    writer: &mut impl io::Write,
    depth: usize,
) -> crate::Result<()> {
    for (folder, files) in &files.folders {
        if depth == 3 && folder.as_os_str().as_encoded_bytes() == b"others" {
            continue;
        }

        write_tabs(writer, depth)?;
        writeln!(writer, "<Filter")?;

        write_tabs(writer, depth + 1)?;
        write!(writer, r#"Name=""#)?;
        writer.write_all(folder.as_os_str().as_encoded_bytes())?;
        writeln!(writer, r#"""#)?;

        write_tabs(writer, depth + 1)?;
        writeln!(writer, ">")?;

        generate_project_filters_impl(files, writer, depth + 1)?;

        write_tabs(writer, depth)?;
        writeln!(writer, "</Filter>")?;
    }

    for path in &files.files {
        write_tabs(writer, depth)?;
        writeln!(writer, "<File")?;

        write_tabs(writer, depth + 1)?;
        write!(writer, r#"RelativePath=".\"#)?;
        writer.write_all(path.as_os_str().as_encoded_bytes())?;
        writeln!(writer, r#"""#)?;

        write_tabs(writer, depth + 1)?;
        writeln!(writer, ">")?;

        write_tabs(writer, depth)?;
        writeln!(writer, "</File>")?;
    }

    Ok(())
}

pub fn write_tabs(writer: &mut impl io::Write, depth: usize) -> crate::Result<()> {
    for _ in 0..depth {
        write!(writer, "\t")?;
    }
    Ok(())
}

//
//
//

pub fn generate_solution_source(
    sln_guid: uuid::Uuid,
    vcproj_guid: uuid::Uuid,
    name: &str,
) -> String {
    let sln_guid = sln_guid.to_string().to_uppercase();
    let vcproj_guid = vcproj_guid.to_string().to_uppercase();

    format!(
        r#"
Microsoft Visual Studio Solution File, Format Version 10.00
# Visual Studio 2008
Project("{{{sln_guid}}}") = "{name}", "{name}.vcproj", "{{{vcproj_guid}}}"
EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
EndGlobal
"#
    )
}
