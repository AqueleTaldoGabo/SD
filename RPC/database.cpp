#include "database.h"

uint32_t ProductDatabase::createProduct(
    uint32_t sellerId,
    const std::string& title,
    const std::string& description,
    double price,
    const std::string& category) {

  ProductData product{
      nextId,
      sellerId,
      title,
      description,
      price,
      category,
      false
  };

  products.push_back(product);
  nextId++;

  return product.id;
}

std::vector<ProductData> ProductDatabase::listProducts() {
  return products;
}

bool ProductDatabase::markAsSold(uint32_t productId) {
  for (auto& product : products) {
    if (product.id == productId) {
      product.sold = true;
      return true;
    }
  }

  return false;
}
