#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DiscreteCharacterData.h"
#include "Ellipsis.h"
#include "Func_readCharacterData.h"
#include "NclReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlAminoAcidState.h"
#include "RlDiscreteCharacterData.h"
#include "RlDnaState.h"
#include "RlRnaState.h"
#include "RlStandardState.h"
#include "RlString.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"
#include "VectorRlPointer.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readCharacterData* Func_readCharacterData::clone( void ) const {
    
    return new Func_readCharacterData( *this );
}


/** Execute function */
RevObject* Func_readCharacterData::execute( void ) {
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    
    // check that the file/path name has been correctly specified
    RevBayesCore::RbFileManager myFileManager( fn.getValue() );
    if ( !myFileManager.testFile() && !myFileManager.testDirectory() )
        {
        std::string errorStr = "";
        formatError(myFileManager, errorStr);
        throw RbException("Could not find file or path with name \"" + fn.getValue() + "\"");
        }
        
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if ( myFileManager.isDirectory() )
        {
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
        }
    else 
        {
        vectorOfFileNames.push_back( myFileManager.getFullFileName() );
        }
    
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    RevBayesCore::NclReader& reader = RevBayesCore::NclReader::getInstance();
    reader.clearWarnings();
    
    // the return value
    VectorRlPointer<RevObject> *m = new VectorRlPointer<RevObject>();
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    size_t numFilesRead = 0;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++) {
        bool isInterleaved = false;
        std::string myFileType = "unknown";
        std::string dType = "unknown";
        if (reader.isNexusFile(*p) == true)
            myFileType = "nexus";
        else if (reader.isPhylipFile(*p, dType, isInterleaved) == true)
            myFileType = "phylip";
        else if (reader.isFastaFile(*p, dType) == true)
            myFileType = "fasta";
        
        int numMatricesReadForThisFile=0;
        if (myFileType != "unknown") {
            std::string suffix = "|" + dType;
            if ( myFileType == "phylip" ) {
                if (isInterleaved == true)
                    suffix += "|interleaved";
                else
                    suffix += "|noninterleaved";
            }
            else if ( myFileType == "fasta" )
                suffix += "|noninterleaved";
            else
                suffix += "|unknown";
            myFileType += suffix;
            
            // read the content of the file now
            std::vector<RevBayesCore::AbstractCharacterData*> m_i = reader.readMatrices( *p, myFileType );
            for (std::vector<RevBayesCore::AbstractCharacterData*>::iterator it = m_i.begin(); it != m_i.end(); it++) {
                
                dType = (*it)->getDatatype();

                // Assume success; correct below if failure
                numMatricesReadForThisFile++;
                
                if ( dType == "DNA" ) {
                    RevBayesCore::DiscreteCharacterData<RevBayesCore::DnaState> *coreM = static_cast<RevBayesCore::DiscreteCharacterData<RevBayesCore::DnaState> *>( *it );
                    DiscreteCharacterData<DnaState> *mDNA = new DiscreteCharacterData<DnaState>( coreM );
                    m->push_back( mDNA );
                }
                else if ( dType == "RNA" ) {
                    RevBayesCore::DiscreteCharacterData<RevBayesCore::RnaState> *coreM = static_cast<RevBayesCore::DiscreteCharacterData<RevBayesCore::RnaState> *>( *it );
                    DiscreteCharacterData<RnaState> *mRNA = new DiscreteCharacterData<RnaState>( coreM );
                    m->push_back( mRNA );
                }
                else if ( dType == "Protein" ) {
                    RevBayesCore::DiscreteCharacterData<RevBayesCore::AminoAcidState> *coreM = static_cast<RevBayesCore::DiscreteCharacterData<RevBayesCore::AminoAcidState> *>( *it );
                    DiscreteCharacterData<AminoAcidState> *mAA = new DiscreteCharacterData<AminoAcidState>( coreM );
                    m->push_back( mAA );
                }
                else if ( dType == "Standard" ) {
                    RevBayesCore::DiscreteCharacterData<RevBayesCore::StandardState> *coreM = static_cast<RevBayesCore::DiscreteCharacterData<RevBayesCore::StandardState> *>( *it );
                    DiscreteCharacterData<StandardState> *mSS = new DiscreteCharacterData<StandardState>( coreM );
                    m->push_back( mSS );
                }
                else {
                    numMatricesReadForThisFile--;
                    throw RbException("Unknown data type \"" + dType + "\".");
                }
            }
        }
        else {
            reader.addWarning("Unknown file type");
        }
    if (numMatricesReadForThisFile > 0)
        numFilesRead++;
    }
    
    
    // print summary of results of file reading to the user
    if (myFileManager.isDirectory() == true)
    {
        std::stringstream o2;
        if ( numFilesRead == 0 )
            o2 << "Failed to read any files from directory '" << fn.getValue() << "'";
        else if ( numFilesRead == 1 ) {
            if ( m->size() == 1 )
                o2 << "Successfully read one file with one character matrix from directory '" << fn.getValue() << "'";
            else
                o2 << "Successfully read one file with " << m->size() << " character matrices from directory '" << fn.getValue() << "'";
        }
        else
            o2 << "Successfully read " << numFilesRead << " files with " << m->size() << " character matrices from directory '" << fn.getValue() << "'";
        RBOUT(o2.str());
        std::set<std::string> myWarnings = reader.getWarnings();
        if ( vectorOfFileNames.size() - numFilesRead > 0 && myWarnings.size() > 0 )
        {
            std::stringstream o3;
            if (vectorOfFileNames.size() - numFilesRead == 1)
                o3 << "Did not read a file for the following ";
            else
                o3 << "Did not read " << vectorOfFileNames.size() - numFilesRead << " files for the following ";
            if (myWarnings.size() == 1)
                o3 << "reason:";
            else
                o3 << "reasons:";
            RBOUT(o3.str());
            for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
                RBOUT("* "+(*it));
        }
    }
    else
    {
        if (m->size() == 1)
            RBOUT("Successfully read one character matrix from file '" + fn.getValue() + "'");
        else if (m->size() > 1)
        {
            std::stringstream o3;
            o3 << "Successfully read " << m->size() << " character matrices from file '" << fn.getValue() << "'";
            RBOUT(o3.str());
        }
        else
        {
            std::set<std::string> myWarnings = reader.getWarnings();
            if ( myWarnings.size() > 0 )
            {
                std::stringstream o3;
                o3 << "Error reading file '" << fn.getValue() << "'";
                RBOUT(o3.str());
                for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
                    RBOUT("Error:   " + (*it));
            }
        }
    }
    
    return m;
}


/** Format the error exception string for problems specifying the file/path name */
void Func_readCharacterData::formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr) {
    
    bool fileNameProvided    = fm.isFileNamePresent();
    bool isFileNameGood      = fm.testFile();
    bool isDirectoryNameGood = fm.testDirectory();
    
    if ( fileNameProvided == false && isDirectoryNameGood == false )
        errorStr += "Could not read contents of directory \"" + fm.getFilePath() + "\" because the directory does not exist";
    else if (fileNameProvided == true && (isFileNameGood == false || isDirectoryNameGood == false)) {
        errorStr += "Could not read file named \"" + fm.getFileName() + "\" in directory named \"" + fm.getFilePath() + "\" ";
        if (isFileNameGood == false && isDirectoryNameGood == true)
            errorStr += "because the file does not exist";
        else if (isFileNameGood == true && isDirectoryNameGood == false)
            errorStr += "because the directory does not exist";
        else
            errorStr += "because neither the directory nor the file exist";
    }
}


/** Get argument rules */
const ArgumentRules& Func_readCharacterData::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) {
        argumentRules.push_back( new ArgumentRule( "file", true, RlString::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_readCharacterData::getClassName(void) { 
    
    static std::string rbClassName = "Func_readCharacterData";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readCharacterData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_readCharacterData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readCharacterData::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = VectorRlPointer<RevObject>::getClassTypeSpec();
    return returnTypeSpec;
}




