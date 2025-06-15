#pragma once

#include <vector>

enum class CommandType { INPUT };

/**
 * @brief A base command is anything that wants to be able to execute some functionality and then undo that functionality
 * The idea is to have a list of commands of different types and be able to execute or undo them at will
 * Execute will always be exectue() with no params the params will come form the constructor
 */
struct BaseCommand {
    virtual ~BaseCommand() = default;
    virtual void execute(float dt);
    virtual void undo();
};

std::vector<BaseCommand*> editorCommandList;