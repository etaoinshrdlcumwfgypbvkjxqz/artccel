#ifndef ARTCCEL_CORE_UTIL_ENCODING_HPP
#define ARTCCEL_CORE_UTIL_ENCODING_HPP
#pragma once

#include <string> // import std::string, std::u16string, std::u32string, std::u8string
#include <string_view> // import std::string_view, std::u16string_view, std::u32string_view, std::u8string_view

namespace artccel::core::util {
auto c8s_compatrtoc8s(std::string_view c8s_compat) -> std::u8string;
auto c8srtoc8s_compat(std::u8string_view c8s) -> std::string;

auto c8srtoc16s(std::u8string_view c8s) -> std::u16string;
auto c8srtoc16s(char8_t c8s) -> std::u16string;
auto c16srtoc8s(std::u16string_view c16s) -> std::u8string;
auto c16srtoc8s(char16_t c16s) -> std::u8string;

auto mbsrtoc8s(std::string_view mbs) -> std::u8string;
auto mbsrtoc8s(char mbs) -> std::u8string;
auto mbsrtoc16s(std::string_view mbs) -> std::u16string;
auto mbsrtoc16s(char mbs) -> std::u16string;
auto mbsrtoc32s(std::string_view mbs) -> std::u32string;
auto mbsrtoc32s(char mbs) -> std::u32string;
auto c8srtombs(std::u8string_view c8s) -> std::string;
auto c8srtombs(char8_t c8s) -> std::string;
auto c16srtombs(std::u16string_view c16s) -> std::string;
auto c16srtombs(char16_t c16s) -> std::string;
auto c32srtombs(std::u32string_view c32s) -> std::string;
auto c32srtombs(char32_t c32s) -> std::string;
} // namespace artccel::core::util

#endif
