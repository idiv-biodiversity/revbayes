/**
 * @file
 * Test parser
 *
 */

#include "RevLanguageMain.h"
#include "IHelp.h"
#include "Parser.h"
#include "RbSettings.h"
#include "Workspace.h"
#include "RlUserInterface.h"
#include "RbVersion.h"
#include <iostream>
#include <string>
#include <cstdlib>

RevLanguageMain::RevLanguageMain(IHelp *help) {
    this->help = help;  
}


void RevLanguageMain::startRevLanguageEnvironment(std::vector<std::string> sourceFiles) {

    // Print a nifty message
    RbVersion version = RbVersion();
    RevLanguage::UserInterface::userInterface().output(version.getHeader(), false);
    RevLanguage::UserInterface::userInterface().output("", false);
    
    RevLanguage::Workspace::globalWorkspace().initializeGlobalWorkspace();
    
    // inject help class
    RevLanguage::Parser::getParser().setHelp(this->help);

    // Print an extra line to separate prompt from possible output from help call
    // RevLanguage::UserInterface::userInterface().output("\n");

#if defined DEBUG_PARSER
    std::cerr << "Global workspace after initialization:" << std::endl;
    RevLanguage::Workspace::globalWorkspace().printValue(std::cerr);
    std::cerr << std::endl;
#endif

    // process the command line arguments as source file names    
    std::string line;
    std::string commandLine;
    int result = 0;

    for(unsigned int i =0 ; i < sourceFiles.size(); i++){
        line = "source(\"" + sourceFiles[i] + "\")";
        std::cout << "RevBayes > " << line << std::endl;

        // Process the command line
        if (result == 1)
            commandLine += line;
        else
            commandLine = line;
        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());

        // We just hope for better input next time
        if (result == 2) {
            result = 0;
        }
    }
}


