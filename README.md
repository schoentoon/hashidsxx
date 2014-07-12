hashidsxx
=========
A C++11 port of the Python *hashids* implementation. It generates Youtube-like hashes from one or many numbers. Use hashids when you do not want to expose your database ids to the user. Website: http://www.hashids.org/

hashidsxx requires a C++11 compiler and is single header, single source file. Simply compile the source file into an object file an link with it within your own project.

Basic usage
-----------

To encrypt a single integer

```c++
  hashidsxx::Hashids hash();
  std::cout << hash.encrypt({123}) << std::endl; // 'Mj3'
```

To decrypt a hash:

```c++
  hashidsxx::Hashids hash();
  std::vector<uint32_t> output = hash.decrypt("Mj3");
  for (uin32_t h : output) std::cout << h << std::endl; // 123
```

Custom salts, alphabets and minimum lengths are passed into the constructor of Hashids

```c++
  hashidsxx::Hashids hash("salt", "abcdefghijklmnopqrstuvwxyz", 16);
  std::cout << hash.encrypt(123456789) << std::endl; // Prints ogkzrxrk
```