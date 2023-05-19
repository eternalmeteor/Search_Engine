#include "../include/ProtocolParser.h"



string ProtocolParser::Json2string(json & j)
{
    return j.dump();
}
json ProtocolParser::doParse(const string & data)
{
    return json::parse(data);
}
json ProtocolParser::vector2json(vector<string> & s)
{
    return json(s);
}
void ProtocolParser::json2vector(json & j, vector<string> & v)
{
    v.clear();
    v = j.get<vector<string>>();
}
