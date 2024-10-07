#pragma once
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>

namespace MSGE
{

namespace JsonUtils
{

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) 
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::vector<unsigned char> base64_decode(const std::string& encoded_string);

bool hasValidResType(const nlohmann::json& jsonResource, const std::string& resType);
    
}
    
} // namespace MSGE
