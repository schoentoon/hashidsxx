hashidsxx
=========
A C++11 port of the Python *hashids* implementation. It generates Youtube-like hashes from one or many numbers. Use hashids when you do not want to expose your database ids to the user. Website: http://www.hashids.org/

hashidsxx requires a C++11 compiler and is single header, single source file. Simply compile the source file into an object file an link with it within your own project.

Basic usage
-----------

To encode a single integer

```c++
  hashidsxx::Hashids hash();
  std::cout << hash.encode({123}) << std::endl; // 'Mj3'
```

To decode a hash:

```c++
  hashidsxx::Hashids hash();
  std::vector<uint64_t> output = hash.decode("Mj3");
  for (uin64_t h : output) std::cout << h << std::endl; // 123
```

Custom salts, alphabets and minimum lengths are passed into the constructor of Hashids

```c++
  hashidsxx::Hashids hash("salt", 16, "abcdefghijklmnopqrstuvwxyz");
  std::cout << hash.encode(123456789) << std::endl; // Prints oavlpogkzrxrkpxd
```
