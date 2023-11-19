#ifndef VALUE_H
#define VALUE_H


#include <memory>
#include <iostream>
#include <vector>
#include <variant>

class FunctionNode;
                                                                                        //for arrays later
using Value = std::variant<double, bool, std::nullptr_t, std::shared_ptr<FunctionNode>/*, std::shared_ptr<std::vector<Value>>*/>;

// struct Value : public std::variant<double, bool, std::nullptr_t, std::shared_ptr<FunctionNode>, std::shared_ptr<std::vector<Value>>> {};

using Null = std::nullptr_t;
using Array = std::shared_ptr<std::vector<Value>>;
using Function = std::shared_ptr<FunctionNode>;


#endif