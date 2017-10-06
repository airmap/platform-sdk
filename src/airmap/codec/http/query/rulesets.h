#ifndef AIRMAP_CODEC_HTTP_QUERY_RULESETS_H_
#define AIRMAP_CODEC_HTTP_QUERY_RULESETS_H_

#include <airmap/rulesets.h>

#include <unordered_map>

namespace airmap {
namespace codec {
namespace http {
namespace query {

void encode(std::unordered_map<std::string, std::string>& query, const RuleSets::Search::Parameters& parameters);
void encode(std::unordered_map<std::string, std::string>& query, const RuleSets::GetRules::Parameters& parameters);
void encode(std::unordered_map<std::string, std::string>& query, const RuleSets::Evaluation::Parameters& parameters);

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_RULESETS_H_
