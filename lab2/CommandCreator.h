//
// Created by Anna on 23.11.2020.
//

#ifndef LAB_2_COMMANDCREATOR_H
#define LAB_2_COMMANDCREATOR_H

#include "ICommandCreator.h"
#include "ICommand.h"
#include "CommandsFactory.h"

#define REGISTER_CREATOR(T, NAME) static WorkFlowFactory::CommandCreator<T> creator(NAME);

namespace WorkFlowFactory {

    template<class T>
    class CommandCreator : public WorkFlowFactory::ICommandCreator {
    public:
        [[nodiscard]] WorkFlowFactory::ICommand *create() const override {
            return new T();
        }

        explicit CommandCreator(const std::string &commandName) {
            WorkFlowFactory::CommandsFactory::instance().registerCommandCreator(commandName, this);
        }
    };
}

#endif //LAB_2_COMMANDCREATOR_H
