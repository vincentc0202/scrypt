#ifndef VALUE_H
#define VALUE_H


#include <memory>
#include <iostream>
#include <vector>
#include <variant>



using Value = std::variant<double, bool>;
// using Array = std::shared_ptr<std::vector<Value>>;
// using FunctionPtr = std::shared_ptr<FunctionNode>;
//using Value = std::variant<double, bool, std::nullptr_t, std::shared_ptr<FunctionNode>, std::shared_ptr<std::vector<Value>>>;

#endif