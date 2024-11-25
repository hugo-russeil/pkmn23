#include "jsonHandler.h"

jsonHandler::jsonHandler() : type(Type::NULLTYPE) {}
jsonHandler::jsonHandler(Type t) : type(t) {}
jsonHandler::jsonHandler(int value) : type(Type::NUMBER), numberValue(static_cast<double>(value)) {}
jsonHandler::jsonHandler(double value) : type(Type::NUMBER), numberValue(value) {}
jsonHandler::jsonHandler(const std::string& value) : type(Type::STRING), stringValue(value) {}


// Implementation of the jsonHandler parser methods
jsonHandler jsonHandler::parse(const std::string& jsonString) {
    std::istringstream stream(jsonString);
    skipWhitespace(stream);
    char ch = stream.peek();
    if (ch == '{') {
        return parseObject(stream);
    } else if (ch == '[') {
        return parseArray(stream);
    }
    return jsonHandler();
}

jsonHandler jsonHandler::parseObject(std::istringstream& stream) {
    jsonHandler json(Type::OBJECT);
    stream.get(); // consume '{'
    skipWhitespace(stream);
    while (stream.peek() != '}') {
        std::string key = parseString(stream);
        skipWhitespace(stream);
        stream.get(); // consume ':'
        skipWhitespace(stream);
        json.object[key] = parseValue(stream);
        skipWhitespace(stream);
        if (stream.peek() == ',') {
            stream.get(); // consume ','
            skipWhitespace(stream);
        }
    }
    stream.get(); // consume '}'
    return json;
}

jsonHandler jsonHandler::parseArray(std::istringstream& stream) {
    jsonHandler json(Type::ARRAY);
    stream.get(); // consume '['
    skipWhitespace(stream);
    while (stream.peek() != ']') {
        json.array.push_back(parseValue(stream));
        skipWhitespace(stream);
        if (stream.peek() == ',') {
            stream.get(); // consume ','
            skipWhitespace(stream);
        }
    }
    stream.get(); // consume ']'
    return json;
}

jsonHandler jsonHandler::parseValue(std::istringstream& stream) {
    skipWhitespace(stream);
    char ch = stream.peek();
    if (ch == '"') {
        jsonHandler json(Type::STRING);
        json.stringValue = parseString(stream);
        return json;
    } else if (ch == '{') {
        return parseObject(stream);
    } else if (ch == '[') {
        return parseArray(stream);
    } else if (isdigit(ch) || ch == '-') {
        jsonHandler json(Type::NUMBER);
        stream >> json.numberValue;
        return json;
    } else if (ch == 't' || ch == 'f') {
        jsonHandler json(Type::BOOL);
        std::string boolStr;
        stream >> boolStr;
        json.boolValue = (boolStr == "true");
        return json;
    } else if (ch == 'n') {
        stream.ignore(4); // consume "null"
        return jsonHandler(Type::NULLTYPE);
    }
    return jsonHandler();
}

std::string jsonHandler::parseString(std::istringstream& stream) {
    std::string result;
    stream.get(); // consume '"'
    char ch;
    while ((ch = stream.get()) != '"') {
        result += ch;
    }
    return result;
}

void jsonHandler::skipWhitespace(std::istringstream& stream) {
    while (isspace(stream.peek())) {
        stream.get();
    }
}

std::string jsonHandler::toString() const {
    std::string result;
    switch (type) {
        case Type::OBJECT:
            result = "{";
            for (const auto& pair : object) {
                result += "\"" + pair.first + "\": " + pair.second.toString() + ",";
            }
            if (!object.empty()) {
                result.pop_back(); // remove trailing comma
            }
            result += "}";
            break;
        case Type::ARRAY:
            result = "[";
            for (const auto& value : array) {
                result += value.toString() + ",";
            }
            if (!array.empty()) {
                result.pop_back(); // remove trailing comma
            }
            result += "]";
            break;
        case Type::STRING:
            result = "\"" + stringValue + "\"";
            break;
        case Type::NUMBER:
            result = std::to_string(numberValue);
            break;
        case Type::BOOL:
            result = boolValue ? "true" : "false";
            break;
        case Type::NULLTYPE:
            result = "null";
            break;
    }
    return result;
}