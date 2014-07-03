/**
 * @file
 * This file contains the declaration of the RevBayes main file which runs the RevLanguage environment.
 *
 *
 * @brief Declaration of the main
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-08-02
 *
 * $Id$
 */


#ifndef RevLanguageMain_H
#define RevLanguageMain_H

#include <string>
#include <vector>
#include "IHelp.h"

class RevLanguageMain {
public:
    RevLanguageMain(IHelp *help);
    void startRevLanguageEnvironment(std::vector<std::string> sourceFiles);
    void startRevLanguageEnvironment(std::vector<std::string> sourceFiles, std::string helpDir);
private:
    IHelp *help;

};

#endif
