#ifndef JSON_HANDLER
#define JSON_HANDLER

#include <string>
#include <map>
#include <vector>
#include <sstream>

class jsonHandler {
public:
    enum class Type { OBJECT, ARRAY, STRING, NUMBER, BOOL, NULLTYPE };

    jsonHandler();
    jsonHandler(Type t);
    jsonHandler(int value);
    jsonHandler(double value);
    jsonHandler(const std::string& value);

    Type type;
    std::map<std::string, jsonHandler> object;
    std::vector<jsonHandler> array;
    std::string stringValue;
    double numberValue;
    bool boolValue;

    static jsonHandler parse(const std::string& jsonString);
    static jsonHandler parseObject(std::istringstream& stream);
    static jsonHandler parseArray(std::istringstream& stream);
    static jsonHandler parseValue(std::istringstream& stream);
    static std::string parseString(std::istringstream& stream);
    static void skipWhitespace(std::istringstream& stream);
    std::string toString() const;
};

#endif // JSON_HANDLER