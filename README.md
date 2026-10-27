Iterations: 500000

Creation and destruction
---------------------------------------
raw pointer                    11.051 ns/op
UniquePtr                      11.532 ns/op
std::unique_ptr                35.435 ns/op
SharedPtr                      20.836 ns/op
std::shared_ptr                35.044 ns/op

Dereference
---------------------------------------
raw pointer                     3.139 ns/op
UniquePtr                       5.057 ns/op
std::unique_ptr                17.429 ns/op
SharedPtr                       4.374 ns/op
std::shared_ptr                 5.922 ns/op

Move round trip
---------------------------------------
UniquePtr                       8.681 ns/op
std::unique_ptr                55.485 ns/op
SharedPtr                       8.608 ns/op
std::shared_ptr                22.743 ns/op

Copy and destruction
---------------------------------------
raw pointer                     2.061 ns/op
SharedPtr                       5.320 ns/op
std::shared_ptr                16.696 ns/op
