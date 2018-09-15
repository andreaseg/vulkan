#ifndef VK_COMMON_HPP
#define VK_COMMON_HPP

#include "include.hpp"
#include <string>

std::string VkResult_to_string(VkResult result);

bool VkResult_is_err(VkResult result);

#endif // VK_COMMON_HPP