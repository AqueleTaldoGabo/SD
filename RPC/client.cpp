#include <capnp/ez-rpc.h>

#include "marketplace.capnp.h"

#include <cstdint>
#include <iostream>
#include <string>

int main() {
  capnp::EzRpcClient client(
      "127.0.0.1",
      5923
  );

  auto& waitScope =
      client.getWaitScope();

  auto marketplace =
      client.getMain<Marketplace>();

  int option;

  do {
    std::cout << "\n===== MARKETPLACE =====\n";
    std::cout << "1 - Criar anuncio\n";
    std::cout << "2 - Listar produtos\n";
    std::cout << "3 - Marcar produto como vendido\n";
    std::cout << "0 - Sair\n";
    std::cout << "Escolha: ";

    std::cin >> option;
    std::cin.ignore();

    if (option == 1) {
      std::string title;
      std::string description;
      std::string category;
      double price;

      std::cout << "Titulo: ";
      std::getline(std::cin, title);

      std::cout << "Descricao: ";
      std::getline(std::cin, description);

      std::cout << "Preco: ";
      std::cin >> price;
      std::cin.ignore();

      std::cout << "Categoria: ";
      std::getline(std::cin, category);

      auto request =
          marketplace.createProductRequest();

      request.setSellerId(1);
      request.setTitle(title);
      request.setDescription(description);
      request.setPrice(price);
      request.setCategory(category);

      auto response =
          request.send().wait(waitScope);

      std::cout << "\nProduto criado com ID: "
                << response.getProductId()
                << "\n";
    }

    else if (option == 2) {
      auto request =
          marketplace.listProductsRequest();

      auto response =
          request.send().wait(waitScope);

      auto products =
          response.getProducts();

      if (products.size() == 0) {
        std::cout << "\nNenhum produto cadastrado.\n";
      }

      for (auto product : products) {
        std::cout << "\n----------------------\n";
        std::cout << "ID: "
                  << product.getId()
                  << "\n";

        std::cout << "Titulo: "
                  << product.getTitle().cStr()
                  << "\n";

        std::cout << "Descricao: "
                  << product.getDescription().cStr()
                  << "\n";

        std::cout << "Preco: R$ "
                  << product.getPrice()
                  << "\n";

        std::cout << "Categoria: "
                  << product.getCategory().cStr()
                  << "\n";

        std::cout << "Status: "
                  << (product.getSold()
                      ? "Vendido"
                      : "Disponivel")
                  << "\n";
      }
    }

    else if (option == 3) {
      uint32_t productId;

      std::cout << "ID do produto vendido: ";
      std::cin >> productId;

      auto request =
          marketplace.markAsSoldRequest();

      request.setProductId(productId);

      auto response =
          request.send().wait(waitScope);

      if (response.getSuccess()) {
        std::cout << "\nProduto marcado como vendido.\n";
      } else {
        std::cout << "\nProduto nao encontrado.\n";
      }
    }

  } while (option != 0);

  return 0;
}
