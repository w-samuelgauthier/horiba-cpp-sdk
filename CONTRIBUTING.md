# How to contribute

## Dependencies

In short: we use `CPM` to manage the [dependencies](https://github.com/cpm-cmake/CPM.cmake). Please refer to [Dependency
Setup](README_dependencies.md) for more details.

## Codestyle

We use `clang-format` to format the code and `clang-tidy` for static analysis. Both configuration files are located in
the root of the project. To format the code, run `cmake --build build --target fix-format`.

### Before submitting

Before submitting your code please do the following steps:

1. Add any changes you want
1. Add tests for the new changes
1. Edit the documentation if you have changed something significant
1. Run `cmake --build build --target fix-format` to format your changes.

## Other help

You can contribute by spreading a word about this library. It would also be a huge contribution to write a short article
on how you are using this project. You can also share your best practices with us.
