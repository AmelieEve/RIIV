#include "creationARFF.h"

constexpr auto rel = "@RELATION";
constexpr auto att = "@ATTRIBUTE";
constexpr auto dat = "@DATA";

#define space String(" ")
#define newLine String("\n")

String generateARFFHeader(String name, list<pair<String, String>> attributes)
{
    String header = rel + space + name + newLine;

    for (pair<String, String> attr : attributes) {
        header += att + space + attr.first + space + attr.second + newLine;
    }

    return header + dat + newLine;
}

String generateARFFLine(list<String> attributes)
{
    auto attrFront = attributes.begin();

    String content = *attrFront;
    for (int i = 1; i < attributes.size(); i++) {
        advance(attrFront, 1);
        content += String(",") + *attrFront;
    }

    return content + newLine;
}
