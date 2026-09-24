#include <capnp/ez-rpc.h>

#include "marketplace.capnp.h"
#include "database.h"

class MarketplaceImpl final
    : public Marketplace::Server {
public:
  ProductDatabase database;

  kj::Promise<void> createProduct(
      CreateProductContext context) override {

    auto params = context.getParams();

    uint32_t id = database.createProduct(
        params.getSellerId(),
        params.getTitle().cStr(),
        params.getDescription().cStr(),
        params.getPrice(),
        params.getCategory().cStr()
    );

    context.getResults().setProductId(id);

    return kj::READY_NOW;
  }

  kj::Promise<void> listProducts(
      ListProductsContext context) override {

    auto products = database.listProducts();

    auto results = context.getResults();

    auto output =
        results.initProducts(products.size());

    for (size_t i = 0; i < products.size(); i++) {
      output[i].setId(products[i].id);
      output[i].setSellerId(products[i].sellerId);
      output[i].setTitle(products[i].title);
      output[i].setDescription(products[i].description);
      output[i].setPrice(products[i].price);
      output[i].setCategory(products[i].category);
      output[i].setSold(products[i].sold);
    }

    return kj::READY_NOW;
  }

  kj::Promise<void> markAsSold(
      MarkAsSoldContext context) override {

    auto params = context.getParams();

    bool success =
        database.markAsSold(params.getProductId());

    context.getResults().setSuccess(success);

    return kj::READY_NOW;
  }
};

int main() {
  capnp::EzRpcServer server(
      kj::heap<MarketplaceImpl>(),
      "0.0.0.0",
      5923
  );

  auto& waitScope =
      server.getWaitScope();

  kj::NEVER_DONE.wait(waitScope);
}
