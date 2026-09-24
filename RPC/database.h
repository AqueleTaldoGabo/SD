#ifndef DATABASE_H
#define DATABASE_H

#include <cstdint>
#include <string>
#include <vector>

struct ProductData {
  uint32_t id;
  uint32_t sellerId;
  std::string title;
  std::string description;
  double price;
  std::string category;
  bool sold;
};

class ProductDatabase {
public:
  uint32_t createProduct(
      uint32_t sellerId,
      const std::string& title,
      const std::string& description,
      double price,
      const std::string& category
  );

  std::vector<ProductData> listProducts();

  bool markAsSold(uint32_t productId);

private:
  std::vector<ProductData> products;
  uint32_t nextId = 1;
};

#endif
