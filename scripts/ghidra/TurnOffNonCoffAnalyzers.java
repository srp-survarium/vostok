import ghidra.app.script.GhidraScript;

public class TurnOffNonCoffAnalyzers extends GhidraScript {

	public void run() throws Exception {
		String[] analyzers = {
			"Stack", 							// 65.215 secs
			"x86 Constant Reference Analyzer",  // 54.324 secs
			"Decompiler Switch Analysis", 		// 41.082 secs
		/// "Disassemble Entry Points",      	// 28.569 secs /* NO */
			"Windows x86 PE RTTI Analyzer", 	// 18.898 secs
			"Function ID",                  	// 11.467 secs
		/// "Scalar Operand References",        //  9.519 secs /* NO */
			"Function Start Search",            //  5.863 secs

			"Demangler Microsoft", 				//  3.636 secs
			"ASCII Strings", 					//  3.322 secs

			"Shared Return Calls",          	//  2.986 secs
			"Subroutine References",            //  2.696 secs
			"Reference",                        //  2.466 secs
			"Data Reference",                   //  2.274 secs

			"Function Start Search After Data", //  2.094 secs

			"Windows x86 PE Exception Handling",//  1.593 secs
			"Create Address Tables",            //  1.394 secs
			"Apply Data Archives",              //  1.085 secs

			"Call-Fixup Installer",             //  0.389 secs
			"WindowsResourceReference", 		//  0.354 secs
			"Embedded Media",                   //  0.210 secs
			"X86 Function Callee Purge",        //  0.161 secs
			"Call Convention ID",               //  0.147 secs
			"Create Function",                  //  0.053 secs
			"PDB Universal Reporting",          //  0.001 secs
			"External Entry References",		//  0.000 secs

			"Non-Returning Functions - Discovered",      //  3.508 secs
			"Function Start Search After Code",          //  2.547 secs

			"Function Start Pre Search",                 //  0.139 secs
			"Create Function",                           //  0.014 secs
			"Disassemble",                               //  0.010 secs
			"Non-Returning Functions - Known",           //  0.007 secs
			"Function Start Search delayed - One Time",  //  0.002 secs

			"Windows x86 Thread Environment Block (TEB) Analyzer",
			"PDB Universal.Import Source Line Info"
		};

		for (String analyzer : analyzers) {
			setAnalysisOption(currentProgram, analyzer, "false");
		}
	}
}
