#ifndef WORKSPACEUTILS_H
#define	WORKSPACEUTILS_H

#include "Workspace.h"

#include "RevObject.h"
#include "RevPtr.h"

#include "VariableSlot.h"
#include "RlFunction.h"
#include "ArgumentRules.h"
#include "MethodTable.h"

#include <map>
#include <vector>
#include <algorithm>
#include <stdbool.h>
#include <stdio.h>

#include <boost/foreach.hpp>

typedef std::vector<std::string> StringVector;
typedef std::vector<RevLanguage::Function *> FunctionVector;
typedef std::vector<RevLanguage::Argument> ArgumentVector;
typedef std::multimap<std::string, RevLanguage::Function*> FunctionMap;
typedef std::map<std::string, RevLanguage::VariableSlot* > VariableTable;
typedef std::map<std::string, RevLanguage::RevObject*> TypeTable;

class WorkspaceUtils {
public:

    //////////// functions //////////////////////

    StringVector getFunctions(void) {
        FunctionMap functionsMap = RevLanguage::Workspace::userWorkspace().getFunctionTable().getTableCopy(true);

        StringVector functions;
        for (FunctionMap::iterator it = functionsMap.begin(); it != functionsMap.end(); ++it) {
            functions.push_back(it->first);
        }

        return makeUnique(functions);
    }

    StringVector getFunctionParameters(std::string name) {

        StringVector result;
        if (!RevLanguage::Workspace::globalWorkspace().existsFunction(name)) {
            return result;
        }

        FunctionVector v = RevLanguage::Workspace::globalWorkspace().getFunctionTable().findFunctions(name);

        for (FunctionVector::iterator it = v.begin(); it != v.end(); it++) {
            const RevLanguage::ArgumentRules& argRules = (*it)->getArgumentRules();
            for (size_t i = 0; i < argRules.size(); i++) {
                result.push_back(argRules[i].getArgumentLabel());
            }
        }

        return makeUnique(result);
    }

    std::vector<StringVector> getFunctionSignatures(std::string name) {
        std::vector<StringVector> *signatures = new std::vector<StringVector>;

        if (!isFunction(name)) {
            return *signatures;
        }

        FunctionVector v = RevLanguage::Workspace::globalWorkspace().getFunctionTable().findFunctions(name);
        for (FunctionVector::iterator it = v.begin(); it != v.end(); it++) {
            StringVector *args = new StringVector;
            ArgumentVector av = (*it)->getArguments();
            for (ArgumentVector::iterator it1 = av.begin(); it1 != av.end(); it1++) {
                args->push_back((*it1).getLabel());
            }
            signatures->push_back(*args);
        }

        return *signatures;
    }

    bool isFunction(std::string name) {
        return RevLanguage::Workspace::globalWorkspace().existsFunction(name);
    }


    //////////// variables  //////////////////////

    StringVector getVariables(bool all = false) {

        StringVector objects;

        VariableTable v = RevLanguage::Workspace::userWorkspace().getVariableTable();

        for (VariableTable::iterator it = v.begin(); it != v.end(); ++it) {
            objects.push_back(it->first);
        }

        if (all) {

            v = RevLanguage::Workspace::globalWorkspace().getVariableTable();

            for (VariableTable::iterator it = v.begin(); it != v.end(); ++it) {
                objects.push_back(it->first);
            }
        }

        return makeUnique(objects);
    }
    
    StringVector getVariableMembers(std::string name);
    bool isVariable(std::string name);


    //////////// types //////////////////////

    StringVector getMethodParameters(std::string typeName, std::string methodName) {
        StringVector sv;
        // make sure type exists
        if (!isType(typeName)) {
            return sv;
        }
     
        RevLanguage::RevObject *type = RevLanguage::Workspace::globalWorkspace().getNewTypeObject(typeName);
        RevLanguage::MethodTable &methods = const_cast<RevLanguage::MethodTable&> (type->getMethods());

        std::multimap<std::string, RevLanguage::Function*> printTable = methods.getTableCopy(false);
        for (std::multimap<std::string, RevLanguage::Function *>::const_iterator i = printTable.begin(); i != printTable.end(); i++) {
            if (i->first == methodName) {
                const RevLanguage::ArgumentRules& argRules = i->second->getArgumentRules();
                for (size_t i = 0; i < argRules.size(); i++) {
                    sv.push_back(argRules[i].getArgumentLabel());
                }
            }
        }

        return makeUnique(sv);
    }

    StringVector getTypes(bool all = true) {

        StringVector objects;

        TypeTable t = RevLanguage::Workspace::userWorkspace().getTypeTable();

        for (TypeTable::iterator it = t.begin(); it != t.end(); ++it) {
            objects.push_back(it->first);
        }

        if (all) {

            t = RevLanguage::Workspace::globalWorkspace().getTypeTable();

            for (TypeTable::iterator it = t.begin(); it != t.end(); ++it) {
                objects.push_back(it->first);
            }
        }

        return makeUnique(objects);
    }

    StringVector getTypeMembers(std::string name) {
        StringVector sv;

        if (!isType(name)) {
            return sv;
        }

        RevLanguage::RevObject *type = RevLanguage::Workspace::globalWorkspace().getNewTypeObject(name);
        RevLanguage::MethodTable &methods = const_cast<RevLanguage::MethodTable&> (type->getMethods());

        std::multimap<std::string, RevLanguage::Function*> printTable = methods.getTableCopy(false);
        for (std::multimap<std::string, RevLanguage::Function *>::const_iterator i = printTable.begin(); i != printTable.end(); i++) {
            sv.push_back(i->first);
        }

        return sv;
    }

    bool isType(std::string name) {
        return RevLanguage::Workspace::globalWorkspace().existsType(name);
    }

    //////////// objects //////////////////////

    // hide the distinction of variables and types

    StringVector getObjects(bool all = true) {
        StringVector t = getTypes(all);
        StringVector v = getVariables(all);
        t.insert(t.end(), v.begin(), v.end());
        return makeUnique(t);
    }
    StringVector getObjectMembers(std::string name);
    bool isObject(std::string name);


private:

    StringVector makeUnique(StringVector v) {
        StringVector result;
        for (StringVector::iterator it = v.begin(); it < std::unique(v.begin(), v.end()); it++) {
            result.push_back(*it);
        }
        return result;
    }
};




#endif	/* WORKSPACEUTILS_H */

