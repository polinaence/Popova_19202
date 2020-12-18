//
// Created by Anna on 23.11.2020.
//

#include "Replace.h"
#include <stdexcept>

#include "../CommandCreator.h"

REGISTER_CREATOR(Commands::Replace, "replace");

static const unsigned int FIRST_REPLACE_ARGS_INDEX = 0;
static const unsigned int SECOND_REPLACE_ARGS_INDEX = 1;

void Commands::Replace::execute(WorkFlow::ExecutionContext &executionWorkFlowContext, unsigned int id) {
    std::string replacedWord = executionWorkFlowContext.getArgumentsById(id)[FIRST_REPLACE_ARGS_INDEX];
    std::string newWord = executionWorkFlowContext.getArgumentsById(id)[SECOND_REPLACE_ARGS_INDEX];

    std::vector<std::string> vectorForReplace;
    for (const auto &curString : executionWorkFlowContext.getExecutionText()) {
        std::string tempString = curString;
        try {
            tempString.replace(tempString.find(replacedWord), replacedWord.size(), newWord);
        } catch (std::out_of_range &error) {
        }

        vectorForReplace.push_back(tempString);

    }

    executionWorkFlowContext.setExecutionText(vectorForReplace);

}

