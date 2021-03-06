/**
 * @file
 * This file contains the implementation of Func_module, which is
 * the function used to read commands (module) from a file.
 *
 * @brief Implementation of Func_module
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-04 18:03:37 +0200 (Fri, 04 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_module.cpp 1485 2012-05-04 16:03:37Z hoehna $
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_module.h"
#include "Module.h"
#include "ModuleSystem.h"
#include "Parser.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RevNullObject.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"
#include "Workspace.h"
#include "WorkspaceVector.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_module::Func_module( void ) : Procedure() {
    
}


/** Clone object */
Func_module* Func_module::clone( void ) const {
    
    return new Func_module( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_module::execute( void ) {
    
    /* Get the module */
    std::string moduleName = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
    const Module& mod = ModuleSystem::getModuleSystem().getModule( moduleName );
    
    Environment *execEnv = env;
    
    if ( args[1].getVariable()->getRevObject() != RevNullObject::getInstance() )
    {
        std::string ns = static_cast<const RlString &>( args[1].getVariable()->getRevObject() ).getValue();
    
         execEnv = env->getChildEnvironment( ns );
    }
    
//    WorkspaceVector<RevObject> *moduleArgs = new WorkspaceVector<RevObject>();
    for (size_t i = 2; i < args.size(); ++i)
    {
//        moduleArgs->push_back( args[i].getVariable()->getRevObject() );
        if ( args[i].getLabel() != "" )
        {
            if ( !execEnv->existsVariable( args[i].getLabel() ) )
            {
                execEnv->addVariable(args[i].getLabel(), args[i].getVariable() );
            }
            
        }
        else
        {
            std::cerr << "Empty ellipsis argument label.\n";
        }
    }
//    execEnv->addVariable("args", moduleArgs);
//    if ( !execEnv->existsVariable("namespace") )
//    {
//        execEnv->addVariable("namespace", new RlString(ns) );
//    }
    
    /* Initialize */
    const std::vector<std::string>& commandLines = mod.getCommandLines();
    std::string command = "";
    int lineNumber = 0;
    int result = 0;     // result from processing of last command
    RBOUT("Processing module \"" + moduleName + "\"");
    
    /* Command-processing loop */
    for ( std::vector<std::string>::const_iterator it = commandLines.begin(); it != commandLines.end(); ++it)
    {
        
        // Get a line
        const std::string& line = *it;
        lineNumber++;
        
        // If previous result was 1 (append to command), we do this
        if ( result == 1 )
            command += line;
        else
            command = line;
        
        // Process the line and record result
        result = Parser::getParser().processCommand( command, execEnv );
        if ( result == 2 ) {
            std::ostringstream msg;
            msg << "Problem processing line " << lineNumber << " in module \"" << moduleName << "\"";
            throw RbException( msg.str() );
        }
    }
    
    /* Return control */
    RBOUT("Processing of module \"" + moduleName + "\" completed");
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_module::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "namespace", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        
        rulesSet = true;
        
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_module::getClassType(void) {
    
    static std::string revType = "Func_module";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_module::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_module::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_module::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

