@0x9f3a4e4c8d8b7a11;

struct Product {
  id @0 :UInt32;
  sellerId @1 :UInt32;
  title @2 :Text;
  description @3 :Text;
  price @4 :Float64;
  category @5 :Text;
  sold @6 :Bool;
}

interface Marketplace {
  createProduct @0 (
    sellerId :UInt32,
    title :Text,
    description :Text,
    price :Float64,
    category :Text
  ) -> (
    productId :UInt32
  );

  listProducts @1 () -> (
    products :List(Product)
  );

  markAsSold @2 (
    productId :UInt32
  ) -> (
    success :Bool
  );
}
