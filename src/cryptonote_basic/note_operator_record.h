#pragma once

#include <array>
#include "cryptonote_format_utils.h"

namespace cryptonote::note_registry
{
  constexpr std::size_t chunk_size = 240;
  constexpr std::size_t signing_key_size = 32;
  constexpr std::size_t encryption_key_size = 563;
  constexpr std::size_t signature_size = 64;
  constexpr std::size_t header_size = 1 + 8 + signing_key_size + encryption_key_size;
  constexpr std::size_t max_endpoint_size = 126;
  constexpr std::size_t max_record_size = header_size + 1 + max_endpoint_size + signature_size;
  constexpr unsigned confirmations = 6;

  inline bool add_record(std::vector<uint8_t> &extra, const std::string &record)
  {
    if (record.size() <= header_size + 1 + signature_size || record.size() > max_record_size)
      return false;
    const std::size_t count = (record.size() + chunk_size - 1) / chunk_size;
    for (std::size_t index = 0; index < count; ++index)
    {
      std::string chunk("NTOR\x01", 5);
      chunk.push_back(static_cast<char>(index));
      chunk.push_back(static_cast<char>(count));
      chunk.append(record, index * chunk_size, chunk_size);
      if (!add_extra_nonce_to_tx_extra(extra, chunk))
        return false;
    }
    return true;
  }

  inline std::string read_record(const std::vector<uint8_t> &extra)
  {
    std::vector<tx_extra_field> fields;
    if (!parse_tx_extra(extra, fields))
      return {};
    std::array<std::string, (max_record_size + chunk_size - 1) / chunk_size> chunks;
    std::size_t count = 0;
    for (const auto &field : fields)
    {
      const auto *nonce = boost::get<tx_extra_nonce>(&field);
      if (!nonce || nonce->nonce.compare(0, 5, "NTOR\x01", 5) != 0)
        continue;
      const auto &chunk = nonce->nonce;
      if (chunk.size() <= 7 || chunk.size() > 7 + chunk_size)
        return {};
      const auto index = static_cast<unsigned char>(chunk[5]);
      const auto total = static_cast<unsigned char>(chunk[6]);
      if (!total || total > chunks.size() || index >= total || (count && total != count) ||
          !chunks[index].empty() || (index + 1 < total && chunk.size() != 7 + chunk_size))
        return {};
      count = total;
      chunks[index] = chunk.substr(7);
    }
    std::string record;
    for (std::size_t i = 0; i < count; ++i)
    {
      if (chunks[i].empty())
        return {};
      record += chunks[i];
    }
    if (record.size() <= header_size + 1 + signature_size || record.size() > max_record_size)
      return {};
    return record;
  }
}
