#pragma once

#include <assert.h>
#include <memory>
#include <string>
#include <openssl/evp.h>

namespace evp
{

class Digest
{
 public:
  enum Type
  {
    SHA1 = NID_sha1,
    SHA256 = NID_sha256,
    MD5 = NID_md5,
  };

  explicit Digest(Type t)
    : ctx_(EVP_MD_CTX_create())
  {
    EVP_DigestInit_ex(ctx_, EVP_get_digestbynid(t), NULL);
  }

  ~Digest()
  {
    if (ctx_)
      EVP_MD_CTX_destroy(ctx_);
  }

  void update(const void* data, int len)
  {
    EVP_DigestUpdate(ctx_, data, len);
  }

  std::string digest()
  {
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int len = 0;
    EVP_DigestFinal_ex(ctx_, result, &len);
    assert(len == length());
    return std::string(reinterpret_cast<char*>(result), len);
  }

  int length() const
  {
    return EVP_MD_CTX_size(ctx_);
  }

  Digest(Digest&& rhs)
    : ctx_(rhs.ctx_)
  {
    rhs.ctx_ = nullptr;
  }

  Digest& operator=(Digest&& rhs)
  {
    Digest copy(std::move(rhs));
    swap(copy);
    return *this;
  }

  void swap(Digest& rhs)
  {
    std::swap(ctx_, rhs.ctx_);
  }

 private:
  EVP_MD_CTX* ctx_;

  Digest(const Digest&) = delete;
  void operator=(const Digest&) = delete;
};

}

